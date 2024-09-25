import * as cdk from "aws-cdk-lib";
import { Construct } from "constructs";
import { NagSuppressions } from "cdk-nag";
import * as lambda from "aws-cdk-lib/aws-lambda";
import * as path from "path";
import * as s3 from "aws-cdk-lib/aws-s3";
import { Layer } from "./common/python-layer";
import { TranslationTriggerLambda } from "./TranslationTriggerLambda/TranslationTriggerLambda";
import { ActivityLambda } from "./ActivityLambda/ActivityLambda";
import { AslMetahumanDeployment, AslMetahumanDeploymentProps } from "./web";
import { AslMetahumanAuth } from "./auth";
import { AslMetahumanHosting } from "./hosting";
import { AslApiGateway } from "./AslApiGateway/AslApiGateway";
import { AslSnsSqs } from "./AslSnsSqs/AslSnsSqs";

export class AwsAslCdkStack extends cdk.Stack {
  constructor(scope: Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    // config for lambda functions
    const lambdaArchitecture = lambda.Architecture.ARM_64;
    const lambdaRuntime = lambda.Runtime.PYTHON_3_12;

    const fanOutTranslationActivity = new AslSnsSqs(
      this,
      "translationActivity",
      {
        snsTopicName: "TranslationActivityTopic",
        sqsQueueName: "TranslationActivityQueue",
      }
    );

    // Fanout pattern used for all other types of messages
    const fanOutOtherActivities = new AslSnsSqs(this, "otherActivities", {
      snsTopicName: "OtherActivitiesTopic",
      sqsQueueName: "OtherActivitiesQueue",
    });

    // bucket for storing server access logging
    const serverAccessLogBucket = new s3.Bucket(this, "serverAccessLogBucket", {
      blockPublicAccess: s3.BlockPublicAccess.BLOCK_ALL,
      encryption: s3.BucketEncryption.S3_MANAGED,
      enforceSSL: true,
      versioned: true,
      lifecycleRules: [
        {
          expiration: cdk.Duration.days(90),
        },
      ],
    });

    const generatedImagesBucket = new s3.Bucket(this, "ImagesBucket", {
      blockPublicAccess: s3.BlockPublicAccess.BLOCK_ALL,
      // This deletes the bucket when the stack is deleted (for easy cleanup).
      removalPolicy: cdk.RemovalPolicy.DESTROY,
      serverAccessLogsBucket: serverAccessLogBucket,
      autoDeleteObjects: true,
      enforceSSL: true,
    });

    const lambdaDepsLayer = new Layer(this, "lambdagenaidepslayer", {
      runtime: lambdaRuntime,
      architecture: lambdaArchitecture,
      path: path.join(__dirname, "../layers/langchain-common-deps"),
      description: "string",
    });

    // LAMBDA FUNCTION USED FOR ASL TRANSLATION
    const translateLambdafn = new TranslationTriggerLambda(
      this,
      "lambdatranslationtrigger",
      {
        architecture: lambdaArchitecture,
        runtime: lambdaRuntime,
        layer: lambdaDepsLayer.layer,
        snsTopic: fanOutTranslationActivity.snsTopic,
        snsTextToImageTopicArn: fanOutOtherActivities.snsTopic,
        outputBucket: generatedImagesBucket,
      }
    );



    // LAMBDA FUNCTION USED FOR CHANGING AVATAR REQUEST
    const changeAvatarLambdafn = new ActivityLambda(this, "lambdachgavatar", {
      functionName: "LambdaChangeAvatar",
      handler: "ChangeAvatar.lambda_handler",
      code: lambda.Code.fromAsset(
        path.join(__dirname, "../functions/ChangeAvatar")
      ),
      architecture: lambdaArchitecture,
      runtime: lambdaRuntime,
      snsTopic: fanOutOtherActivities.snsTopic,
    });

    // LAMBDA FUNCTION USED FOR STOPPING ALL ACTIVITIES ON THE FRONTEND
    const stopAllLambdafn = new ActivityLambda(this, "lambdastopall", {
      functionName: "LambdaStopAll",
      handler: "StopAllActivities.lambda_handler",
      code: lambda.Code.fromAsset(
        path.join(__dirname, "../functions/StopAllActivities")
      ),
      architecture: lambdaArchitecture,
      runtime: lambdaRuntime,
      snsTopic: fanOutOtherActivities.snsTopic,
    });

    // LAMBDA FUNCTION USED FOR CHANGING AVATAR REQUEST
    const changeSettingsLambdafn = new ActivityLambda(this, "lambdachgsttngs", {
      functionName: "LambdaChangeSettings",
      handler: "ChangeSettings.lambda_handler",
      code: lambda.Code.fromAsset(
        path.join(__dirname, "../functions/ChangeSettings")
      ),
      architecture: lambdaArchitecture,
      runtime: lambdaRuntime,
      snsTopic: fanOutOtherActivities.snsTopic,
    });

    const auth = new AslMetahumanAuth(this, "AslMetahumanAuth");

    const api = new AslApiGateway(this, "apigwasl", {
      cognito_user_pool: auth.userPool,
      cognito_user_pool_client: auth.userPoolClient,
      change_avatar_lmbda: changeAvatarLambdafn.lambdafn,
      translation_sentiment_lmbda: translateLambdafn.lambdafn,
      stop_all_activities_lmbda: stopAllLambdafn.lambdafn,
      change_settings_lmbda: changeSettingsLambdafn.lambdafn,
    });

    const hosting = new AslMetahumanHosting(this, "AslMetahumanHosting", {
      env: "dev",
    });

    const deployConf: AslMetahumanDeploymentProps = {
      hostingBucket: hosting.hostingBucket,
      distribution: hosting.distribution,
      userPool: auth.userPool,
      userPoolClient: auth.userPoolClient,
      identityPool: auth.identityPool,
      userPoolDomain: auth.userPoolDomain,
    };

    const deploy = new AslMetahumanDeployment(
      this,
      "AslMetahumanS3Deployment",
      deployConf
    );


    new cdk.CfnOutput(this, "IdentityPoolId", {
      value: auth.identityPool.identityPoolId,
    });

    new cdk.CfnOutput(this, "UserPool Id", {
      value: auth.userPool.userPoolId,
    });

    new cdk.CfnOutput(this, "Client Id", {
      value: auth.userPoolClient.userPoolClientId,
    });

    new cdk.CfnOutput(this, "Auth role", {
      value: auth.authorizedRole.roleName,
    });

    new cdk.CfnOutput(this, "domain name ", {
      value: hosting.distribution.domainName,
  });
  new cdk.CfnOutput(this, "CfnOutCloudFrontUrl", {
      value: `https://${hosting.distribution.distributionDomainName}`,
      description: "The CloudFront URL",
  });
  new cdk.CfnOutput(this, "distributionId", {
      value: hosting.distribution.distributionId,
      description: "CloudFront Distribution Id",
  });

    NagSuppressions.addResourceSuppressions(
      this,
      [
        {
          id: "AwsSolutions-IAM5",
          reason: "s3 bukcet notification is used.",
          appliesTo: [ 
                    "Action::s3:GetBucket*",
                    "Action::s3:GetObject*",
                    "Action::s3:List*",
                    "Action::s3:List*",
                    "Action::s3:*",
                    "Action::s3:DeleteObject*",
                    "Action::s3:Abort*",
                    "Resource::*",
                    "Resource::<AslMetahumanHostingHostingBucketC1462044.Arn>/*",
                    "Resource::arn:<AWS::Partition>:s3:::cdk-hnb659fds-assets-<AWS::AccountId>-<AWS::Region>/*"
                    
          ],
        },
        {
          id: "AwsSolutions-IAM4",
          reason: "AWSLambdaBasicExecutionRole is used.",
          appliesTo: [
            "Policy::arn:<AWS::Partition>:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole",
          ],
        },
        {
          id: "AwsSolutions-CFR4",
          reason: "No certificate is used.",
          appliesTo: ['Resource::AwsAslCdkStack/AslMetahumanHosting/Distribution/Resource'],
        
        },
        {
          id: "AwsSolutions-L1",
          reason: "aws amplify is using in built custom resource.",
          appliesTo: ['Resource::Custom::CDKBucketDeployment8693BB64968944B69AAFB0CC9EB8756C4096MiB/Resource'],
        },
      ],
      true
    );

      NagSuppressions.addResourceSuppressionsByPath(
        cdk.Stack.of(this),
        `${cdk.Stack.of(this).stackName}/Custom::CDKBucketDeployment8693BB64968944B69AAFB0CC9EB8756C4096MiB/Resource`,
        [
          {
            id: 'AwsSolutions-L1',
            reason: 'Lambda runtime version is managed upstream by CDK.',
          },
        ],
      );

      NagSuppressions.addResourceSuppressionsByPath(
        cdk.Stack.of(this),
        `${cdk.Stack.of(this).stackName}/AslMetahumanHosting/Distribution/Resource`,
        [
          {
            id: 'AwsSolutions-CFR4',
            reason: 'Lambda runtime version is managed upstream by CDK.',
          },
        ],
      );
  }
}
