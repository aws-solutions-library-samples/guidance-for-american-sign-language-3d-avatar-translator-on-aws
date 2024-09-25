import boto3
import json
import os
import uuid

sns = boto3.client('sns')

def lambda_handler(event, context):
    print(event)

    data = json.loads(event['body'])
    
    sign_rate = data['sign_rate']
    
    output = {
            "Action": "CHANGE_SIGN_RATE",
            "Data": str(sign_rate)
        }
        
    try:
        topic = os.environ['snsTopicArn']
        response = sns.publish(
            TopicArn=topic,
            Message=json.dumps(output),
            MessageStructure='string',
            MessageDeduplicationId=str(uuid.uuid4()),
            MessageGroupId='5'
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