import * as iam from 'aws-cdk-lib/aws-iam';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import * as sns from 'aws-cdk-lib/aws-sns';
import { Construct } from 'constructs';
import { Duration, Aws } from 'aws-cdk-lib';
import { NagSuppressions } from 'cdk-nag';

/**
 * The properties for the TranslationTriggerLambda class.
 */
export interface ActivityLambdaProps{
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

    readonly functionName: string;
    readonly handler: string;
    readonly code: lambda.Code;

    readonly snsTopic : sns.ITopic;
  }

export class ActivityLambda extends Construct {
    public readonly lambdafn: lambda.IFunction;

    constructor(scope: Construct, id: string, props: ActivityLambdaProps) {
        super(scope, id);
  
        
        const activity_role = new iam.Role(
            this,
            'translation_function_role'+props.functionName,
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
                      ],
                      resources: [
                        `arn:${Aws.PARTITION}:logs:${Aws.REGION}:${Aws.ACCOUNT_ID}:log-group:/aws/lambda/*`,
                      ],
                    }),
                  ],
                }),
              },
            },
          );
    
        const changeAvatarLambdafn = new lambda.Function(this, 'changeavatarlambda', {
            runtime: props.runtime,
            architecture: props.architecture,
            functionName: props.functionName,
            role: activity_role,
            handler: props.handler,
            tracing: lambda.Tracing.ACTIVE,
            memorySize: 512,
            timeout: Duration.minutes(5),
            code: props.code,
            environment: {
                snsTopicArn: props.snsTopic.topicArn
            },
        });

        NagSuppressions.addResourceSuppressions(
            activity_role,
            [
                {
                id: 'AwsSolutions-IAM5',
                reason: 'AWSLambdaBasicExecutionRole is used.',
                },
            ],
            true,
        );

        props.snsTopic.grantPublish(changeAvatarLambdafn);

        this.lambdafn = changeAvatarLambdafn;
    }
}