import * as path from 'path';
import * as iam from 'aws-cdk-lib/aws-iam';
import { Duration, Aws } from 'aws-cdk-lib';
import * as s3 from 'aws-cdk-lib/aws-s3';
import * as lambda from 'aws-cdk-lib/aws-lambda';
import * as sns from 'aws-cdk-lib/aws-sns';
import { NagSuppressions } from 'cdk-nag';
import { Construct } from 'constructs';

/**
 * The properties for the TranslationTriggerLambda class.
 */
export interface TextToImageLambdaProps{
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

    readonly outputBucket:  s3.IBucket;
  }

export class TextToImageLambda extends Construct {
    public readonly lambdafn: lambda.IFunction;

    constructor(scope: Construct, id: string, props: TextToImageLambdaProps) {
        super(scope, id);

        const texttoimage_lambda_role = new iam.Role(
            this,
            'generate_image_function_role',
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
    
        // Lambda function triggered when there is a new translation txt file available
        const textToImageLambda = new lambda.Function(this, 'lambdaTextToImage', {
            runtime: props.runtime,
            architecture: props.architecture,
            functionName: 'LambdaTextToImage',
            role: texttoimage_lambda_role,
            handler: 'text_to_image.lambda_handler',
            tracing: lambda.Tracing.ACTIVE,
            layers: [props.layer],
            memorySize: 512,
            timeout: Duration.minutes(5),
            code: lambda.Code.fromAsset(path.join(__dirname, '../../functions/TextToImage')),
            environment: {
                snsTopicArn: props.snsTopic.topicArn,
                OUTPUT_BUCKET: props.outputBucket.bucketName
            },
        });

        textToImageLambda.addToRolePolicy(new iam.PolicyStatement({
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
    
        textToImageLambda.addToRolePolicy(
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

        NagSuppressions.addResourceSuppressions(
            texttoimage_lambda_role,
            [
                {
                id: 'AwsSolutions-IAM5',
                reason: 'AWSLambdaBasicExecutionRole is used.',
                },
            ],
            true,
        );

        props.snsTopic.grantPublish(textToImageLambda);
        props.outputBucket.grantReadWrite(textToImageLambda);

        this.lambdafn = textToImageLambda;
    }
}