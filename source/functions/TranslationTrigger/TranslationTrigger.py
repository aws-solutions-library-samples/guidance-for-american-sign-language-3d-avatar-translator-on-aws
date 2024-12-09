import boto3
import json
import os
import re
import uuid
from TextToImage_generator import generate_image

GENERAL_TOXICITY_THRESHOLD = 0.3

TOXICITY_THRESHOLDS = {
    "GRAPHIC": GENERAL_TOXICITY_THRESHOLD,
    "HARASSMENT_OR_ABUSE": GENERAL_TOXICITY_THRESHOLD,
    "HATE_SPEECH": 0.24,
    "INSULT": 0.5,
    "PROFANITY": GENERAL_TOXICITY_THRESHOLD,
    "SEXUAL": 0.55,
    "VIOLENCE_OR_THREAT": GENERAL_TOXICITY_THRESHOLD,
}
NEGATIVE_BEDROCK_RESPONSES = ["do not wish to", "thoughtless remark", "Let's move our conversation",
                              "I cannot provide that type", "harmful or unethical", "I cannot provide a simplified",
                              "not feel comfortable", "in the way you requested", "I apologize,",
                              "offensive stereotype", "refrain from simplifying", 'I do not feel comfortable',
                              'in the manner you requested']
UNKNOWN_BEDROCK_RESPONSES = ["have enough context", "provide more details", "you are asking me to do", 'AI assistant',
                             'created by Anthropic', 'not actually simplify sentences',
                             'Unfortunately I do not have any', 'thoughtful discussion about the',
                             'without simplifying them', 'do not have the capability',
                             'to be helpful, harmless, and honest', 'you would like me to simplify',
                             'Could you provide some example']
LATE_RESPONSE_MARKER = ":"
UNETHICAL_MESSAGE_RESPONSE = "Pay Attention! Unethical Input."
UNKNOWN_MESSAGE_RESPONSE = "Please Repeat Again!"

SIMPLIFICATION_PROMPT_OLD = "Human:simplify these sentences (after the colon) into English in six non-hyphenated non-polysemous words and remove all contractions:"
SIMPLIFICATION_PROMPT_FORMATTED = "Human: Simplify into English using the same original tense for these sentences (after the colon) into one shortened English sentence using non-hyphenated non-polysemous non-contracted words and remove all contracted words and apostrophes in the resulting sentence. Keep main subject.:\"%s\""
ASL_CONVERSION_ZERO_PROMPT_FORMATTED = "Human: Provide the tense (present, past, or future) in one word of the following:\"%s\""
ASL_CONVERSION_ONE_PROMPT_FORMATTED = "Human: Use only the present tense for the provided text (after the colon) and provide a combined sentence. Make all words non-plural. Do not repeat sentences. Make all words non-affixed words and remove ed-endings and remove ing-endings. Include all provided subjects.:\"%s\""
ASL_CONVERSION_TWO_PROMPT_FORMATTED = "Human: put the verbs after all objects after all main subjects in the following text:\"%s\""
NON_ASL_WORDS_TO_IGNORE_LIST = ["as", "although", "be", "for", "and", "nor", "but", "or", "yet", "so", "about", "above",
                                "across", "after", "against", "along", "among", "around", "at", "before", "behind",
                                "before", "because",
                                "below", "beneath", "beside", "between", "beyond", "by", "down", "during", "except",
                                "for", "from", "if",
                                "lest", "once", "then", "that", "though", "till", "when", "where", "in", "inside",
                                "into", "near", "of",
                                "off", "on", "over", "past", "since", "through", "throughout", "till", "to", "toward",
                                "under", "until", "up",
                                "upon", "with", "within", "without", "a", "an", "the", "am", "is", "are", "was", "were"]
STOP_SEQUENCES = ["\\n\\nHuman:"]

BEDROCK_IMAGE_PARAMETERS = {"cfg_scale": 4.4, "seed": 416330048, "steps": 34}
BEDROCK_IMAGE_DESCRIPTION_SUFFIX = "photorealistic, painted, 8k quality"
BEDROCK_IMAGE_NAME_PREFIX = "aslImage"

MODEL_ID = "anthropic.claude-v2"
CONTENT_TYPE = "application/json"

MAX_TOKENS_TO_SAMPLE = 200
"""
Temperature - Tunes the degree of randomness in generation. Lower temperatures mean less random generations.
Top P - If set to float less than 1, only the smallest set of most probable tokens with probabilities that add up to top_p or higher are kept for generation.
Top K - Can be used to reduce repetitiveness of generated tokens. The higher the value, the stronger a penalty is applied to previously present tokens, proportional to how many times they have already appeared in the prompt or prior generation.
"""
TEMPERATURE = 0.7
TOP_P = 0.65
TOP_K = 80

HTTP_STATUS_OK = 200
HTTP_STATUS_FAILURE = 500

sns_obj = boto3.client('sns')
bedrock_obj = boto3.client(service_name="bedrock-runtime", region_name="us-east-1")
comprehend_obj = boto3.client(service_name="comprehend", region_name="us-east-1")


def run_bedrock_translation(inner_prompt):
    prompt = ("\n\n%s .\n\nAssistant:" % inner_prompt)
    body = json.dumps({"prompt": prompt, "stop_sequences": STOP_SEQUENCES, "max_tokens_to_sample": MAX_TOKENS_TO_SAMPLE,
                       "temperature": TEMPERATURE,
                       "top_p": TOP_P,
                       "top_k": TOP_K})
    try:
        response = bedrock_obj.invoke_model(body=body, modelId=MODEL_ID, accept=CONTENT_TYPE, contentType=CONTENT_TYPE)
        response_body = json.loads(response.get('body').read())
        completion = response_body.get("completion").replace("\"", "").replace("\n", "").replace("\.",
                                                                                                 """).replace("\,", """)
        if LATE_RESPONSE_MARKER in completion:
            completion = completion[completion.index(LATE_RESPONSE_MARKER) + len(LATE_RESPONSE_MARKER):]
        # print(body)
        # print(response_body)
        return completion
    except Exception as e:
        print(f"Failed to call bedrock: {e}")
        return ""


def is_toxic(text):
    return is_bedrock_toxic(text) or is_comprehend_toxic(text)


def is_comprehend_toxic(text):
    response = comprehend_obj.detect_toxic_content(
        TextSegments=[{"Text": text}],
        LanguageCode='en'
    )
    labels = response['ResultList'][0]['Labels']
    for label in labels:
        print(label['Name'], label['Score'], TOXICITY_THRESHOLDS[label['Name']])
        if label['Score'] >= TOXICITY_THRESHOLDS[label['Name']]:
            return True
    return False


def is_bedrock_toxic(text):
    for negative_bedrock_response in NEGATIVE_BEDROCK_RESPONSES:
        if negative_bedrock_response.lower() in text.lower():
            return True
    return False


def is_failed_simplification(text):
    for unknown_response in UNKNOWN_BEDROCK_RESPONSES:
        if unknown_response.lower() in text.lower():
            return True
    return False


def is_int(number):
    try:
        int(number)
    except ValueError:
        return False
    return True

def process_message(message, iterations):

    simplified_text = message
    asl_text = ""
    tense = "present"

    # No simplification requested - rely on Comprehend. TODO: language detect!
    #
    if iterations == 0:
        if is_comprehend_toxic(message):
            simplified_text = UNETHICAL_MESSAGE_RESPONSE
            asl_text = simplified_text
            tense = "present"
    else:
        last_simplified_text = simplified_text
        simplification_failure = False
        for i in range(0, iterations):
            simplification_prompt = SIMPLIFICATION_PROMPT_FORMATTED % simplified_text
            simplified_text = run_bedrock_translation(simplification_prompt)
            print("\nSimplified message (iteration %d): %s" % (i + 1, simplified_text))
            if is_failed_simplification(simplified_text):
                simplification_failure = True
                if i == 0:
                    simplified_text = UNKNOWN_MESSAGE_RESPONSE
                else:
                    simplified_text = last_simplified_text
                break
            elif is_toxic(simplified_text):
                simplification_failure = True
                simplified_text = UNETHICAL_MESSAGE_RESPONSE
                break
            last_simplified_text = simplified_text
        if not simplification_failure:
            asl_prompt = ASL_CONVERSION_ZERO_PROMPT_FORMATTED % simplified_text
            tense = run_bedrock_translation(asl_prompt)
            regex = re.compile('[^a-zA-Z]')
            tense  = regex.sub('',  tense)
            asl_prompt = ASL_CONVERSION_ONE_PROMPT_FORMATTED % simplified_text
            asl_text = run_bedrock_translation(asl_prompt)
            asl_prompt = ASL_CONVERSION_TWO_PROMPT_FORMATTED % asl_text
            asl_text = run_bedrock_translation(asl_prompt)
            asl_text = " ".join(list(
                filter(lambda w: w not in NON_ASL_WORDS_TO_IGNORE_LIST, asl_text.lower().strip(".").split(" "))))

        print("Output: pre-ASL/ASL:", [simplified_text, tense, asl_text])

    if simplified_text != UNETHICAL_MESSAGE_RESPONSE:
        sentiment = comprehend_obj.detect_sentiment(Text=message, LanguageCode='en')['Sentiment']
    else:
        # Do we already know if the message was toxic? If so, avoid re-pinging comprehend
        #
        sentiment = "SHOCKED"

    return (simplified_text, asl_text, tense, sentiment)

def lambda_handler(event, context):
    print("boto3 version " + boto3.__version__)
    print("Received event: " + json.dumps(event, indent=2))

    print(event)

    data = json.loads(event['body'])
    iterations = 0
    if is_int(data['iterations']):
        iterations = int(data['iterations'])
    message = data['message']

    if message is None:
        return {
            'statusCode': 200,
            'body': json.dumps('Failed to get message from S3 bucket')
        }
    else:
        print("Initial message %s" % message)

    try:

        (simplified_text, asl_text, tense, sentiment) = process_message(message, iterations)

        output = {
            "Action": "ANIMATE_SENTENCE",
            "Data": simplified_text,
            "kwargs": {
                "sentiment": sentiment,
                "asl_text" : asl_text,
                "tense" : tense
            }
        }

        topic = os.environ['snsTopicArn']
        response = sns_obj.publish(
            TopicArn=topic,
            Message=json.dumps(output),
            MessageStructure='string',
            MessageDeduplicationId=str(uuid.uuid4()),
            MessageGroupId='1'
        )

        dictionary = {
            'topic': topic,
            'result': 'success',
            'messageId': response['MessageId'],
            'sequenceNumber': response['SequenceNumber']
        }

        print(response)

        if simplified_text != UNETHICAL_MESSAGE_RESPONSE:
            print("generate image")
            # generate image
            image_input_details = {
                "imagename": BEDROCK_IMAGE_NAME_PREFIX,
                "inputText": f'{simplified_text} {BEDROCK_IMAGE_DESCRIPTION_SUFFIX}',
                "bedrockParameters": BEDROCK_IMAGE_PARAMETERS
            }
            print(image_input_details)
            image_response = generate_image(image_input_details)
            print(image_response)

        return {
            'statusCode': HTTP_STATUS_OK,
            'body': json.dumps(dictionary)
        }
    except Exception as e:
        return {
            'statusCode': HTTP_STATUS_FAILURE,
            'body': str(e)
        }
