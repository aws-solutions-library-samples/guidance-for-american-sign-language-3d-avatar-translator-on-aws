# Guidance for American Sign Language (ASL) 3D Avatar Translator on AWS

## Table of Contents

1. [Overview](#overview)
    - [Architecture](#architecture)
    - [Services in this Guidance](#aws-services-in-this-guidance)
    - [Cost](#cost)
2. [High-level Workflow](#high-level-workflow)
3. [Architecture](#architecture)
4. [Prerequisites](#prerequisites)
5. [Deployment and Validation Steps](#deployment-and-validation-steps)
6. [License](#License)
7. [Next Steps](#next-steps)
8. [Cleanup](#cleanup)
9. [Notices](#notices)
10. [Authors](#authors)

This repository contains documentation for a Spatial Computing/GenAI prototype solution that was presented at Amazon's
re:Invent 2023 conference. It includes the following major components within the <i>source</i> directory:

- A CDK-deployable project [see <i>CDK.json</i>, <i>lib</i> directory, and <i>bin</i> directory]
- Lambda code for basic GenAI-based ASL translation logic and request/message passing [see <i>functions</i> directory]
- Sample code for a CloudFront-based web interface (for users to speak or type phrases to be
  translated) [see <i>frontend</i> directory]
- An Unreal Engine 5.3 C++ project (displays ASL translations performed by a realistic 3D avatar) [see <i>ue</i> 
  directory]

Please monitor this repository for future updates as more code and documentation becomes available.

## Overview

This sample code/prototype demonstrates a novel way to pre-process/transform multilingual phrases into an equivalent
literal (or direct) form for translation into American Sign Language (ASL). This pre-processing step improves sign
language translation fidelity by expressing user-provided (input) phrases more clearly than they were initially
expressed. GenAI is applied to re-interpret these multilingual input phrases into simpler, more explicit English 
phrases across multiple iterations/passes. These resulting phases have a different (more charitable) kind of 
interpretation versus the resulting phrases produced by traditional non-GenAI-based translation tools. Finally, this 
prototype animates a realistic avatar in Unreal Engine (via <a href="https://www.unrealengine.com/en-US/metahuman"
target="_blank"> MetaHuman plugin</a>) to visually depict the ASL translation of those resulting phrases. ASL
translation in this prototype is based on a very loose/naïve interpretation of ASL rules and grammar, primarily
involving hand and arm movements - all of which end users can refine. The main goals of this project are to essentially
improve the translation of existing robust ASL translation engines (via GenAI), and to provide an
engaging multimodal interface to view ASL translations.

![Alt text](assets/images/rainbow-example.png?raw=true "Sample Translation")
<video src="https://github.com/aws-solutions-library-samples/guidance-for-american-sign-language-3d-avatar-translator-on-aws/assets/156058749/a612dfd9-dba3-4ba3-af6d-baf187f13119"></video>

## High-level Workflow

1) An end user speaks (or types) a phrase in a spoken language of choice
2) That spoken phrase is transcribed directly
3) The transcribed phrase is translated (comprehended) via Generative AI into English, which is then simplified across
   multiple iterations
   using carefully crafted Bedrock prompts
4) An Avatar in [Unreal Engine](https://www.unrealengine.com/en-US) software animates ASL gestures ("signs")
   corresponding to the simplified transcription

## Architecture

<!-- [Alt text](assets/images/architecture.png?raw=true "Architecture") -->

![Reference Architecture](assets/images/american-sign-language-3d-avatar-translator-on-aws.7ad539a2a8f330eb3f8abcdd305401dc6a03c853.png?raw=true "Architecture")

### Architecture Diagram Workflow

1. User authenticates to [Amazon Cognito](https://aws.amazon.com/cognito/) using
   an [Amazon CloudFront](https://aws.amazon.com/cloudfront/)-hosted website or web API (through Amazon Cognito-based
   JWT access token).
2. User types or speaks an input phrase in a chosen language,
   which [Amazon Transcribe](https://aws.amazon.com/transcribe/) transcribes. Transcription is stored in
   an [Amazon Simple Storage Service](https://aws.amazon.com/s3/) (Amazon S3) bucket.
3. User requests an action (like ASL translate, change avatar, or change background image) through the website or web
   API ([Amazon API Gateway](https://aws.amazon.com/api-gateway/) endpoint).
4. Based on the user-requested action, API Gateway routes its request to a
   corresponding [AWS Lambda](https://aws.amazon.com/lambda/) function for processing of that action.
5. For ASL Translation requests, a matching AWS Lambda function
   invokes [Amazon Bedrock](https://aws.amazon.com/bedrock/) API to form an ASL phrase for the provided input phrase and
   obtain a contextual 2D image (to be stored in an S3 bucket).
6. [Amazon Comprehend](https://aws.amazon.com/comprehend/) and Amazon Bedrock perform multilingual toxicity checks on
   the input phrase. [Amazon Rekognition](https://aws.amazon.com/rekognition/) performs visual toxicity checks on
   2D-generated images. Toxicity check results are returned to respective Lambda functions.
7. All Lambda functions generate a JSON-based payload to capture a user-requested action
   for [Epic Games Unreal Engine](https://www.unrealengine.com/en-US/home). Each payload is sent to a
   corresponding [Amazon Simple Notification Service (Amazon SNS)](https://aws.amazon.com/sns/) topic: `Translation` or
   `Non-Translation`.
8. Each Amazon SNS-based payload is transmitted to its corresponding [Amazon Simple Queue Service (Amazon SQS)]() queue
   for later consumption by Unreal Engine.
9. Using the [AWS SDK](https://aws.amazon.com/sdk-for-cpp/), the Unreal Engine application polls and dequeues Amazon SQS
   action-based payloads from its queues. Background images are fetched from an S3 bucket for translation requests.
10. Based on each payload received, the Unreal Engine application performs a user-requested action and
    displays resulting video output on that user’s system. This output provides an ASL-equivalent interpretation of an
    input phrase by displaying a [MetaHuman](https://www.unrealengine.com/marketplace/en-US/product/metahuman-plugin) 3D
    avatar animation with ASL-transformed text displayed.

### AWS Services in this Guidance

| **AWS Service**                                                          | Role |                                                               |
|--------------------------------------------------------------------------|------|---------------------------------------------------------------|
| [Amazon Transcribe](https://aws.amazon.com/transcribe/)                  | Core | Convert user speech to text.                                  |
| [Amazon Bedrock](https://aws.amazon.com/bedrock/)                        | Core | Invoke foundation model to translate natural language to ASL. |
| [Amazon API Gateway](https://aws.amazon.com/api-gateway/)                | Core | Create API to invoke lambda functions from user interface.    |
| [AWS Lambda](https://aws.amazon.com/lambda/)                             | Core | Run custom code to generate ASL for simplified text.          |
| [Amazon Cognito](https://aws.amazon.com/pm/cognito/)                     | Core | Authenticate user to access ASL translator                    |
| [Amazon Comprehend](https://aws.amazon.com/comprehend/)                  | Core | Run moderation to detect toxicity on generated text            |
| [Amazon Rekognition](https://aws.amazon.com/rekognition/)                | Core | Run moderation to detect toxicity on generated image           |
| [Amazon CloudFront](https://aws.amazon.com/cloudfront/)                  | Core | Fast and secure web-hosted user experience                    |
| [Amazon Simple Storage Service (S3)](https://aws.amazon.com/pm/serv-s3/) | Core | Host user interface code, store generated images              |
| [Amazon Simple Notification Service (SNS)](https://aws.amazon.com/sns/)  | Core | Send the notification to Unreal Engine                        |
| [Amazon Simple Queue Service (SQS)](https://aws.amazon.com/sqs/)         | Core | Queues notifications for Unreal Engine to consume             |

### Cost

You are responsible for the cost of the AWS services used while running this Guidance.

### Sample Cost Table

The following table provides a sample cost breakdown for deploying this Guidance with the default parameters in the
`us-east-1` (N. Virginia) Region for one month. This estimate is based on
the [AWS Pricing Calculator output](https://calculator.aws/#/estimate?id=7957a979c145d350d81966d353295c26bec7d8c8) for
the full deployment of this Guidance. As of *February 2025*, an average cost of running this Guidance in the `us-east-1`
is around *$1718/month*:

| AWS service                | Dimensions                                                 | Cost [USD]      |
|----------------------------|------------------------------------------------------------|-----------------|
| Amazon Transcribe          | 5,000 requests, 34 KB request size                         | <$1/month       |
| Amazon Bedrock             | 10 users                                                   | <$1/month       |
| Amazon API Gateway         | 5,000 requests, 128 MB memory allocation, 25 s duration    | <$1/month       |
| AWS Lambda (event trigger) | 5,000 requests, 128 MB memory allocation, 5 s duration     | <$1/month       |
| Amazon Cognito             | 1 GB storage                                               | <$2/month       |
| Amazon Comprehend          | 10 GB standard storage                                     | <$1/month       |
| Amazon Rekognition         | 200 input / 300 output tokens per request (5,000 requests) | $44/month       |
| Amazon S3                  | 200 input / 300 output tokens per request (5,000 requests) | $26/month       |
| Amazon SNS                 | 730 hours x 1.125 USD/hour                                 | $821/month      |
| Amazon SQS                 | 730 hours x 1.125 USD/hour                                 | $821/month      |
| **TOTAL**                  |                                                            | **$1718/month** |

## Prerequisites

Verify that your environment satisfies the following prerequisites:

1. Contains an [AWS account](https://aws.amazon.com/premiumsupport/knowledge-center/create-and-activate-aws-account/)
2. Has <i>`AdministratorAccess`</i> policy granted to your AWS account (for production, we recommend restricting access as
   needed and following the principle of least privilege)
3. Provides console and programmatic access
4. [AWS CLI](https://aws.amazon.com/cli/) installed and configured to use with your AWS account
5. [NodeJS 22+](https://nodejs.org/en/download/) installed
6. [Typescript 3.8+](https://www.typescriptlang.org/download) installed
7. [AWS CDK CLI](https://docs.aws.amazon.com/cdk/latest/guide/getting_started.html) installed
8. [Docker](https://docs.docker.com/get-docker/) installed
9. [Python 3+](https://www.python.org/downloads/) installed

You must explicitly enable access to models before they can be used with the Amazon Bedrock service. Please follow these
steps in the [Amazon Bedrock User Guide](https://docs.aws.amazon.com/bedrock/latest/userguide/model-access.html) to
enable access to the models used in this solution:

- stability.stable-diffusion-xl
- anthropic.claude-v2

### Service Quotas

Your AWS account has default quotas, also known as service limits,
described [here](https://docs.aws.amazon.com/general/latest/gr/aws_service_limits.html). This Guidance can be installed
and tested within the default quotas for each of the services used. You can request increases for some quotas. Note that
not all quotas can be increased.

To operate this Guidance at scale, it is important to monitor your usage of AWS services and configure alarm settings to
notify you when a quota is close to being exceeded. You can find details on visualizing your service quotas and setting
alarms [here](https://docs.aws.amazon.com/AmazonCloudWatch/latest/monitoring/CloudWatch-Quotas-Visualize-Alarms.html).

### Supported Regions

This Guidance uses the Amazon Bedrock service, which is not currently available in all AWS regions. You must launch this
solution in
an AWS region where Amazon Bedrock is available. For the most current availability of AWS services by Region, refer to
the [AWS Regional Services
List](https://aws.amazon.com/about-aws/global-infrastructure/regional-product-services/).

American Sign Language (ASL) 3D Avatar Translator on AWS is supported in the following AWS regions (as of Feb 2025):

| **Region Name**        |                | 
|------------------------|----------------|
| US East (Ohio)         | us-east-2      |
| Asia Pacific (Seoul)   | ap-northeast-2 |
| US East (N. Virginia)  | us-east-1      |
| Europe (Paris)         | eu-west-3      |

## Deployment and Validation Steps

### Deployment

This project is built using [Cloud Development Kit (CDK)](https://aws.amazon.com/cdk/). Please
see [Getting Started With the AWS CDK](https://docs.aws.amazon.com/cdk/v2/guide/getting_started.html) for additional
details and prerequisites.
<!--
1. Clone this repository.

   ```shell
   $ git clone https://github.com/aws-solutions-library-samples/guidance-for-american-sign-language-3d-avatar-translator-on-aws.git
   ```
2. Enter the code sample directory.

   ```shell
   $ cd source
   ```
3. Install dependencies.

   ```shell
   $ npm install
   ```
4. Boostrap AWS CDK resources on the AWS account.

   ```shell
   $ cdk bootstrap
   ```
5. Deploy the sample in your account. To protect you against unintended changes that affect your security posture,
the AWS CDK Toolkit prompts you to approve security-related changes before deploying them. You will need to answer
yes to get the stack deployed.

   ```shell
   $ cdk deploy
   ```
6. Update config.js file with following cdk outputs. This is required to set up the UI with the backend services.

* "IdentityPoolId":"AwsAslCdkStackIdentityPoolId",
* "userPoolId": "AwsAslCdkStack.UserPoolId",
* "userPoolWebClientId": "AwsAslCdkStack.ClientId",
* "apipath": "AwsAslCdkStack.apigwurl"

Build frontend code base. These commands will create build in /frontend/dist folder.

    ```shell
    $ cd frontend
    $ npm install
    $ npm run-script build
    ```

Re-run cdk deploy after updating the file to provide the correct configuration

### Validation

1. Create a user in the Cognito user pool. Go to the [Amazon Cognito page](https://console.aws.amazon.com/cognito/home)
in the AWS console, then select the created user pool. Under ```users```, select ```Create user``` and fill in the form
2. Access the webapp (either locally or cloud hosted) and sign in using the user credentials you just created. 
For hosted ui please follow these steps 
Click on App Integration -> Select App client name -> Click ```View Hosted UI```

Sign In with the username and password. If asked, update the password

3. Copy the CloudFront url from the CDK Output of your stack and paste it on the browser.
AwsAslCdkStack.CfnOutCloudFrontUrl = "https://<cloudfrontdomainname>.cloudfront.net"
4. Enter the username and password that was created in the cognito pool.
5. Click on ```Live transcription```
6. Send text or change avatar from the UI.
-->

For detailed instructions on deployment and validation of this Guidance please refer to
this [Implementation Guide](https://implementationguides.kits.eventoutfitters.aws.dev/asl-3dava-0829/ai-ml/american-sign-language-3d-avatar-translator-on-aws.html)

## Next Steps

Overall, this early-stage prototype illustrates an end-to-end workflow for an 'approximated' American Sign Language
(ASL) translation via 3D Avatar. Ideally, generated pre-ASL (comprehended) output phrases should be 
forwarded to a robust ASL processing engine, which would then generate corresponding avatar animation data to be 
supplied to Unreal Engine.

Suggested future enhancements:

* LiveLink capability
    * Mechanism to dynamically feed external animation data into UE to control individual skeleton joints per animation
      frame (versus a playing limited selection of pre-built UE animations)
* Animation Montage with Blending
    * Make ASL signing animations more natural/less abrupt/smooth, easier to decipher sentences, allow for multiple
      animations at once across more parts of the skeleton rig (i.e. face, hands etc).
* Animations of additional body components (i.e. face)
* Extend architecture and translation capability to support other sign language variants
* Design/re-architect to support ASL translation processing performed outside of UE
* Integrate lambda-based ASL "pre-processor" logic with a robust external ASL engine/API, compare before/after output.
* Application integration
    * Stream ASL animations to major chat programs, perform batch offline processing of videos

## Cleanup

Do not forget to delete the Guidance stack to avoid unexpected charges. A sample command to uninstall it is
shown below:

```
    $ cdk destroy AwsAslCdkStack
```

Then in the AWS Console, delete the Amazon CloudWatch logs, empty and delete the S3 buckets.

## License

This library is licensed under the MIT-0 License. Please see the [LICENSE](LICENSE) file.

## Notices

Customers are responsible for making their own independent assessment of the information in this document. This
document: (a) is for informational purposes only, (b) represents AWS current product offerings and practices, which
are subject to change without notice, and (c) does not create any commitments or assurances from AWS and its
affiliates, suppliers or licensors. AWS products or services are provided “as is” without warranties,
representations, or conditions of any kind, whether express or implied. AWS responsibilities and liabilities to its
customers are controlled by AWS agreements, and this document is not part of, nor does it modify, any agreement
between AWS and its customers.

## Authors

We would like to acknowledge the contributions of these editors and reviewers.

* Alexa Perlov, Prototyping Architect
* Alain Krok, Sr Prototyping Architect
* Daniel Zilberman, Sr Solutions Architect - Tech Solutions
* David Israel, Sr Spatial Architect
* Dinesh Sajwan, Sr Prototyping Architect
* Michael Tran, Sr Prototyping Architect