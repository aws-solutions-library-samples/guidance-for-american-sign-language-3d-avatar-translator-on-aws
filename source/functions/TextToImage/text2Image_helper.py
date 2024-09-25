import boto3,os,base64,json
import requests as reqs
import calendar
import time


textToImage_bucket_name_bucket_name = os.environ['OUTPUT_BUCKET']

aws_region = boto3.Session().region_name
s3 = boto3.client('s3')

def write_file_to_s3(imgbase64encoded,image_name):
   
    current_GMT = time.gmtime()
    time_stamp = calendar.timegm(current_GMT)
    image_name = image_name+"_"+str(time_stamp)+".jpeg"
    
    print(f"uploading file to s3 bucket: {textToImage_bucket_name_bucket_name}, key: {image_name}")
    try:
        respImg= s3.put_object(Body=base64.b64decode(imgbase64encoded), Bucket=textToImage_bucket_name_bucket_name, Key=image_name)
        print(f"image uplaoded : {respImg}")
    except Exception as e:
        print(f"Error occured :::: {e}")
        return False
    return {
        "image_name":image_name,
        "bucket_name":textToImage_bucket_name_bucket_name
    }


def check_image_moderation(bucket,img_key):
    print("Generated image moderation check ")
    response={
        "isToxic":False,
        "confidence":0
    }
    rekognition=boto3.client('rekognition')
    #Detects unsafe content in a specified JPEG or PNG format image.
    rekognition_response = rekognition.detect_moderation_labels(
    Image={
        'S3Object':{
            'Bucket':bucket,
            'Name':img_key}
            }
    )
    print('Detected labels for ' + img_key)
    for label in rekognition_response['ModerationLabels']:
        print (label['Name'] + ' : ' + str(label['Confidence']))
        print (label['ParentName'])
        if(label['Confidence'] > 0.50):
            response['isToxic']=True
            response['confidence']=str(label['Confidence'])
            break
    return response
