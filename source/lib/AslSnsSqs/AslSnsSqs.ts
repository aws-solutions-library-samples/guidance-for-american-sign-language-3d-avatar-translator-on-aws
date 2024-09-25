import * as sns from 'aws-cdk-lib/aws-sns';
import * as sqs from 'aws-cdk-lib/aws-sqs';
import * as kms from 'aws-cdk-lib/aws-kms';
import { PolicyStatement, AnyPrincipal, Effect, AccountPrincipal } from 'aws-cdk-lib/aws-iam';
import { Duration, Stack } from 'aws-cdk-lib';
import * as subsc from 'aws-cdk-lib/aws-sns-subscriptions';
import { Construct } from 'constructs';
import { NagSuppressions } from 'cdk-nag';

/**
 * The properties for the AslApiGateway class.
 */
export interface AslSnsSqsProps{
    readonly sqsQueueName: string,
    readonly snsTopicName: string,
  }

export class AslSnsSqs extends Construct {
    public readonly snsTopic: sns.Topic;

    
    constructor(scope: Construct, id: string, props: AslSnsSqsProps) {
        super(scope, id);

        // Create a SQS Queue.
        // A dead-letter queue is optional but it helps capture any failed messages.
        const deadLetterQueue = new sqs.Queue(this, props.sqsQueueName+'Dlq', {
            queueName: props.sqsQueueName+'Dlq.fifo',
            fifo: true,
            retentionPeriod: Duration.days(7),
            enforceSSL: true
        });
        const activityQueue = new sqs.Queue(this, props.sqsQueueName, {
            queueName: props.sqsQueueName+'.fifo',
            visibilityTimeout: Duration.seconds(60),
            fifo: true,
            deadLetterQueue: {
                maxReceiveCount: 1,
                queue: deadLetterQueue
            },
            enforceSSL: true
        });
    
        // Create a SNS Topic.
        const activityEventTopic = new sns.Topic(this, props.snsTopicName, {
            topicName: props.snsTopicName+'.fifo',
            fifo: true,
            masterKey: kms.Alias.fromAliasName(scope, 'aws-managed-key'+props.snsTopicName, 'alias/aws/sns'),
        });

        // Apply topic policy to enforce only the topic owner can publish and subscribe to this topic
        activityEventTopic.addToResourcePolicy(
            new PolicyStatement({
            sid: 'TopicOwnerOnlyAccess',
            resources: [`${activityEventTopic.topicArn}`],
            actions: [
                'SNS:Publish',
                'SNS:RemovePermission',
                'SNS:SetTopicAttributes',
                'SNS:DeleteTopic',
                'SNS:ListSubscriptionsByTopic',
                'SNS:GetTopicAttributes',
                'SNS:Receive',
                'SNS:AddPermission',
                'SNS:Subscribe',
            ],
            principals: [new AccountPrincipal(Stack.of(activityEventTopic).account)],
            effect: Effect.ALLOW,
            conditions: {
                StringEquals: {
                'AWS:SourceOwner': Stack.of(activityEventTopic).account,
                },
            },
            }),
        );
  
        // Apply Topic policy to enforce encryption of data in transit
        activityEventTopic.addToResourcePolicy(
            new PolicyStatement({
            sid: 'HttpsOnly',
            resources: [`${activityEventTopic.topicArn}`],
            actions: [
                'SNS:Publish',
                'SNS:RemovePermission',
                'SNS:SetTopicAttributes',
                'SNS:DeleteTopic',
                'SNS:ListSubscriptionsByTopic',
                'SNS:GetTopicAttributes',
                'SNS:Receive',
                'SNS:AddPermission',
                'SNS:Subscribe',
            ],
            principals: [new AnyPrincipal()],
            effect: Effect.DENY,
            conditions: {
                Bool: {
                'aws:SecureTransport': 'false',
                },
            },
            }),
        );

        const dlQueueSnsSub = new sqs.Queue(this, props.snsTopicName+'Dlq', {
            queueName: props.snsTopicName+'Dlq.fifo',
            fifo:true,
            retentionPeriod: Duration.days(7),
            enforceSSL: true
        });

        NagSuppressions.addResourceSuppressions(
            dlQueueSnsSub,
            [
                {
                id: 'AwsSolutions-SQS3',
                reason: 'The queue is used as a DLQ for the SQS subscription, seems to be a CDK issue.',
                },
            ],
            true,
        );
    
        // Bind the SQS Queue to the SNS Topic.
        const sqsSubscription = new subsc.SqsSubscription(activityQueue, {
            rawMessageDelivery: true,
            deadLetterQueue: dlQueueSnsSub,
        });
        activityEventTopic.addSubscription(sqsSubscription);

        this.snsTopic = activityEventTopic;
    
    }
}