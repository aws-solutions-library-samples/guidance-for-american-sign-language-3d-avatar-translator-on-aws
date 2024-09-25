import * as cdk from 'aws-cdk-lib';
import { Duration } from 'aws-cdk-lib';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import * as logs from 'aws-cdk-lib/aws-logs';
import * as iam from 'aws-cdk-lib/aws-iam';
import * as cognito from "aws-cdk-lib/aws-cognito";
import * as apigwv2 from 'aws-cdk-lib/aws-apigatewayv2';
import { HttpLambdaIntegration } from 'aws-cdk-lib/aws-apigatewayv2-integrations';
import { HttpUserPoolAuthorizer } from 'aws-cdk-lib/aws-apigatewayv2-authorizers';
import { Construct } from 'constructs';
import { NagSuppressions } from 'cdk-nag';

/**
 * The properties for the AslApiGateway class.
 */
export interface AslApiGatewayProps{
    readonly cognito_user_pool: cognito.IUserPool,
    readonly cognito_user_pool_client: cognito.IUserPoolClient,
    readonly translation_sentiment_lmbda: lambda.IFunction,
    //readonly change_background_lmbda: lambda.IFunction,
    readonly change_avatar_lmbda: lambda.IFunction,
    readonly stop_all_activities_lmbda: lambda.IFunction,
    readonly change_settings_lmbda: lambda.IFunction
  }

export class AslApiGateway extends Construct {
    public readonly api: apigwv2.HttpApi;

    constructor(scope: Construct, id: string, props: AslApiGatewayProps) {
        super(scope, id);

        const accessLogs = new logs.LogGroup(this, 'APIGW-AccessLogs');

        const authorizer = new HttpUserPoolAuthorizer("AslAuthorizer", props.cognito_user_pool, {
            userPoolClients: [props.cognito_user_pool_client]
        });

        const api = new apigwv2.HttpApi(this, 
            "HttpApi",
            {
                corsPreflight: {
                    allowHeaders: ["Authorization", "content-type", "x-amz-date", "x-api-key"],
                    allowMethods:[apigwv2.CorsHttpMethod.GET, apigwv2.CorsHttpMethod.HEAD, apigwv2.CorsHttpMethod.OPTIONS, apigwv2.CorsHttpMethod.POST],
                    allowOrigins:["*"],
                    maxAge:Duration.days(10)
                },
                 
            } 
        );

        const stage = api.defaultStage?.node.defaultChild as apigwv2.CfnStage;
        stage.accessLogSettings = {
            destinationArn: accessLogs.logGroupArn,
            format: JSON.stringify({
                requestId: '$context.requestId',
                userAgent: '$context.identity.userAgent',
                sourceIp: '$context.identity.sourceIp',
                requestTime: '$context.requestTime',
                requestTimeEpoch: '$context.requestTimeEpoch',
                httpMethod: '$context.httpMethod',
                path: '$context.path',
                status: '$context.status',
                protocol: '$context.protocol',
                responseLength: '$context.responseLength',
                domainName: '$context.domainName'
            })
        };

        const role = new iam.Role(this, 'ApiGWLogWriterRole', {
            assumedBy: new iam.ServicePrincipal('apigateway.amazonaws.com')
          })
      
        const policy = new iam.PolicyStatement({
            actions: [
                'logs:CreateLogGroup',
                'logs:CreateLogStream',
                'logs:DescribeLogGroups',
                'logs:DescribeLogStreams',
                'logs:PutLogEvents',
                'logs:GetLogEvents',
                'logs:FilterLogEvents'
            ],
            resources: ['*']
        });
      
        role.addToPolicy(policy);
        accessLogs.grantWrite(role);

        NagSuppressions.addResourceSuppressions(
            role,
            [
                {
                id: 'AwsSolutions-IAM5',
                reason: 'AWSLambdaBasicExecutionRole is used.',
                },
            ],
            true,
        );

        // CHANGE_AVATAR_REQUEST
        api.addRoutes({
            integration: new HttpLambdaIntegration('ChangeAvatarIntegration', props.change_avatar_lmbda),
            path: '/change_avatar',
            authorizer: authorizer,
            methods: [ apigwv2.HttpMethod.POST],
        }
        );

        // ASL_TRANSLATION
        api.addRoutes({
            integration: new HttpLambdaIntegration('AslTranslationIntegration', props.translation_sentiment_lmbda),
            path: '/translate',
            authorizer: authorizer,
            methods: [ apigwv2.HttpMethod.POST],
        }
        );

        // STOP_ALL_ANIMATIONS
        api.addRoutes({
            integration: new HttpLambdaIntegration('StopAllActivitiesIntegration', props.stop_all_activities_lmbda),
            path: '/stop_all',
            authorizer: authorizer,
            methods: [ apigwv2.HttpMethod.POST],
        }
        );

        // CHANGE_SETTINGS
        api.addRoutes({
            integration: new HttpLambdaIntegration('ChangeSettingsIntegration', props.change_settings_lmbda),
            path: '/change_settings',
            authorizer: authorizer,
            methods: [ apigwv2.HttpMethod.POST],
        }
        );

        new cdk.CfnOutput(this, "API URL", {
            value: api.url || "",
          });
    
    }
}