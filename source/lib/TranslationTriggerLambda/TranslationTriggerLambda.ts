import * as path from 'path';
import * as iam from 'aws-cdk-lib/aws-iam';
import { Duration, Aws } from 'aws-cdk-lib';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import * as sns from 'aws-cdk-lib/aws-sns';
import { NagSuppressions } from 'cdk-nag';
import { Construct } from 'constructs';
import * as s3 from 'aws-cdk-lib/aws-s3';
/**
 * The properties for the TranslationTriggerLambda class.
 */
export interface TranslationTriggerLambdaProps{
    /**
     * Required. Lambda function runtime compatible with this Layer.
     *
     * @default - none
     */
    readonly runtime: lambda.Runtime;
    /**
     * Required. Lambda function architecture compatible with this Layer.
     *
     * @default - none
     */
    readonly architecture: lambda.Architecture;

    readonly layer: lambda.ILayerVersion;

    readonly snsTopic : sns.ITopic;

    readonly snsTextToImageTopicArn : sns.ITopic;

    readonly outputBucket:  s3.IBucket;
  
  }

export class TranslationTriggerLambda extends Construct {
    public readonly lambdafn: lambda.IFunction;

    constructor(scope: Construct, id: string, props: TranslationTriggerLambdaProps) {
        super(scope, id);

        const translation_lambda_role = new iam.Role(
            this,
            'translation_function_role',
            {
              assumedBy: new iam.ServicePrincipal('lambda.amazonaws.com'),
              inlinePolicies: {
                LambdaFunctionServiceRolePolicy: new iam.PolicyDocument({
                  statements: [
                    new iam.PolicyStatement({
                      actions: [
                        'logs:CreateLogGroup',
                        'logs:CreateLogStream',
                        'logs:PutLogEvents',
                        's3:GetObject',
                        's3:GetBucketLocation',
                        's3:ListBucket',
                        's3:PutObject',
                      ],
                      resources: [
                        `arn:${Aws.PARTITION}:logs:${Aws.REGION}:${Aws.ACCOUNT_ID}:log-group:/aws/lambda/*`,
                        `arn:${Aws.PARTITION}:s3:::${props.outputBucket.bucketName}/*`,
                      ],
                    }),
                  ],
                }),
              },
            },
          );

          translation_lambda_role.addToPolicy(
            new iam.PolicyStatement({
              effect: iam.Effect.ALLOW,
              actions: [
                'rekognition:DetectModerationLabels',
                'rekognition:DetectText',
                'rekognition:DetectLabels',
                'rekognition:DetectFaces',
              ],
              resources: ['*'],
            }),
          );
    
        // Lambda function triggered when there is a new translation txt file available
        const translationTriggerLambda = new lambda.Function(this, 'lambdaTranslation', {
            runtime: props.runtime,
            architecture: props.architecture,
            role: translation_lambda_role,
            functionName: 'LambdaTranslation',
            handler: 'TranslationTrigger.lambda_handler',
            tracing: lambda.Tracing.ACTIVE,
            layers: [props.layer],
            memorySize: 512,
            timeout: Duration.minutes(5),
            code: lambda.Code.fromAsset(path.join(__dirname, '../../functions/TranslationTrigger')),
            environment: {
                snsTopicArn: props.snsTopic.topicArn,
                OUTPUT_BUCKET:props.outputBucket.bucketName,
                snsTextToImageTopicArn: props.snsTextToImageTopicArn.topicArn,
            },
        });
    
        // Add Amazon Bedrock permissions to the IAM role for the Lambda function
        translationTriggerLambda.addToRolePolicy(new iam.PolicyStatement({
            effect: iam.Effect.ALLOW,
            actions: [
                'bedrock:InvokeModel',
                'bedrock:InvokeModelWithResponseStream',
            ],
            resources: [
                'arn:' + Aws.PARTITION + ':bedrock:' + Aws.REGION + '::foundation-model',
                'arn:' + Aws.PARTITION + ':bedrock:' + Aws.REGION + '::foundation-model/*',
            ],
        }));

        // Add Amazon comprehend permissions
        translationTriggerLambda.addToRolePolicy(new iam.PolicyStatement({
            effect: iam.Effect.ALLOW,
            actions: ['comprehend:DetectSentiment', 'comprehend:DetectToxicContent'],
            resources: ['*'],
        }));

        NagSuppressions.addResourceSuppressions(
            translation_lambda_role,
            [
                {
                id: 'AwsSolutions-IAM5',
                reason: 'AWSLambdaBasicExecutionRole is used.',
                },
            ],
            true,
        );

        props.snsTopic.grantPublish(translationTriggerLambda);
        props.snsTextToImageTopicArn.grantPublish(translationTriggerLambda);

        this.lambdafn = translationTriggerLambda;
    }
}