import boto3
import json
import os
import uuid

sns_obj = boto3.client('sns')

def lambda_handler(event, context):
    print(event)
    
    output = {
            "Action": "STOP_ALL_ANIMATIONS"
        }
        
    try:
        topic = os.environ['snsTopicArn']
        response = sns_obj.publish(
            TopicArn=topic,
            Message=json.dumps(output),
            MessageStructure='string',
            MessageDeduplicationId=str(uuid.uuid4()),
            MessageGroupId='4'
        )

        dictionary = {
            'topic': topic,
            'result': 'success',
            'messageId': response['MessageId'],
            'sequenceNumber': response['SequenceNumber']
        }
        
        print(response)
    
        return {
            'statusCode': 200,
            'body': json.dumps(dictionary)
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'body': json.dumps(e)
        }