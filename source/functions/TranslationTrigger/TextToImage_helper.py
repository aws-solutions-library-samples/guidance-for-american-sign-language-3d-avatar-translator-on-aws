import boto3
import os
import base64
import calendar
import time

EXPORT_BUCKET_NAME = os.environ['OUTPUT_BUCKET']
CONFIDENCE_MIN_THRESHOLD = 0.30

s3 = boto3.client('s3')
rekognition = boto3.client('rekognition')


def write_file_to_s3(img_encoded, image_name):
    time_stamp = calendar.timegm(time.gmtime())
    image_name += "_" + str(time_stamp) + ".jpeg"
    print(f"uploading file to s3 bucket: {EXPORT_BUCKET_NAME}, key: {image_name}")
    try:
        resp_img = s3.put_object(Body=base64.b64decode(img_encoded), Bucket=EXPORT_BUCKET_NAME,
                                 Key=image_name)
        print(f"image uploaded : {resp_img}")
    except Exception as e:
        print(f"Error occurred :::: {e}")
        return False
    return {
        "image_name": image_name,
        "bucket_name": EXPORT_BUCKET_NAME
    }


def check_image_moderation(bucket, img_key):
    print("Generated image moderation check ")
    response = {
        "isToxic": False,
        "confidence": 0
    }
    # Detects unsafe content in a specified JPEG or PNG format image.
    rekognition_response = rekognition.detect_moderation_labels(
        Image={
            'S3Object': {
                'Bucket': bucket,
                'Name': img_key}
        }
    )
    print('Detected labels for ' + img_key)
    for label in rekognition_response['ModerationLabels']:
        print(label['Name'] + ' : ' + str(label['Confidence']))
        print(label['ParentName'])
        if label['Confidence'] > CONFIDENCE_MIN_THRESHOLD:
            response['isToxic'] = True
            response['confidence'] = str(label['Confidence'])
            break
    return response
