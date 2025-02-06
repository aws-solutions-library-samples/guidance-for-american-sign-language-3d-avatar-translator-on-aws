# Guidance for American Sign Language (ASL) 3D Avatar Translator on AWS

## Table of Contents

1. [Overview](#overview)
    - [Architecture](#architecture)
    - [Services in this guidance](#aws-services-in-this-guidance)
    - [Cost](#cost)
3. [High-level Workflow](#high-level-workflow)
4. [Reference Architecture](#reference-architecture)
5. [Prerequisites](#prerequisites-required)
    - [Operating System](#operating-system-required)
6. [Deployment Steps](#deployment-steps-required)
7. [Deployment Validation](#deployment-validation-required)
8. [Running the Guidance](#running-the-guidance-required)
9. [License](#License)
10. [Next Steps](#next-steps)
11. [Cleanup](#cleanup)
12. [FAQ, known issues, additional considerations, and limitations](#faq-known-issues-additional-considerations-and-limitations-optional)
13. [Notices](#notices)
14. [Authors](#authors)


Currently, this repository consists purely of documentation for a Spatial Computing/GenAI prototype solution that was 
presented at Amazon's re:Invent 2023 conference. In the coming months, it will have polished code posted, which 
includes the following major components:
   - A CDK-deployable project (includes lambda code for very basic GenAI-based ASL translation logic and message 
     passing)
   - Sample code for a web interface (for users to speak or type phrases to be translated)
   - An Unreal Engine 5.3 C++ project (displays ASL translations performed by a realistic 3D avatar)

Please monitor this repository for future updates as code becomes available.


## Overview

This sample code demonstrates a novel way to pre-process/transform multilingual phrases into an equivalent literal (or 
direct) form for translation into American Sign Language (ASL). This pre-processing step improves sign language 
translation fidelity by expressing user-provided (input) phrases more clearly than they were initially expressed. 
GenAI is applied to re-interpret these multilingual input phrases into simpler, more explicit English phrases across 
multiple iterations/passes. These resulting phases have a different (more charitable) kind of interpretation versus 
the resulting phrases produced by traditional non-GenAI-based translation tools. Finally, this prototype animates a 
realistic avatar in Unreal Engine (via <a href="https://www.unrealengine.com/en-US/metahuman" 
target="_blank">MetaHuman plugin</a>) to visually depict the ASL translation of those resulting phrases. ASL
translation in this prototype is based on a very loose/naïve interpretation of ASL rules and grammar, involving 
primarily hand and arm movements, all of which end users can refine. The main goals of this project are to essentially
improve the translation of existing robust ASL translation engines (through the use of GenAI), and to provide an 
engaging multimodal interface to view ASL translations.

![Alt text](assets/images/rainbow-example.png?raw=true "Sample Translation")
<video src="https://github.com/aws-solutions-library-samples/guidance-for-american-sign-language-3d-avatar-translator-on-aws/assets/156058749/a612dfd9-dba3-4ba3-af6d-baf187f13119"></video>

## High-level Workflow

1) An end user speaks (or types) a phrase in a spoken language of choice
2) That spoken phrase is transcribed directly
3) The transcribed phrase is translated via Generative AI into English, which is then simplified across multiple iterations 
   using carefully-crafted Bedrock prompts
4) an Avatar in [Unreal Engine](https://www.unrealengine.com/en-US) software animates ASL gestures ("signs") corresponding to the simplified transcription

## Architecture

<!-- [Alt text](assets/images/architecture.png?raw=true "Architecture") -->

![Reference Architecture](assets/images/american-sign-language-3d-avatar-translator-on-aws.7ad539a2a8f330eb3f8abcdd305401dc6a03c853.png?raw=true "Architecture") 

### Architecture Diagram Workflow
1. User authenticates to [Amazon Cognito](https://aws.amazon.com/cognito/) using an [Amazon CloudFront](https://aws.amazon.com/cloudfront/)-hosted website or web API (through Amazon Cognito-based JWT access token).
2. User types or speaks an input phrase in a chosen language, which [Amazon Transcribe](https://aws.amazon.com/transcribe/) transcribes. Transcription is stored in an [Amazon Simple Storage Service](https://aws.amazon.com/s3/) (Amazon S3) bucket.
3. User requests an action (like ASL translate,Change avatar, or Change background image) through the website or web API ([Amazon API Gateway](https://aws.amazon.com/api-gateway/) endpoint).
4. Based on the user-requested action, API Gateway routes its request to a corresponding [AWS Lambda](https://aws.amazon.com/lambda/) function for processing of that action.
5. For ASL Translation requests, a matching AWS Lambda function invokes [Amazon Bedrock](https://aws.amazon.com/bedrock/) API to form an ASL phrase for the provided input phrase and obtain a contextual 2D image (to be stored in an S3 bucket).
6. [Amazon Comprehend](https://aws.amazon.com/comprehend/) and Amazon Bedrock perform multi-lingual toxicity checks on the input phrase. [Amazon Rekognition](https://aws.amazon.com/rekognition/) performs visual toxicity checks on 2D-generated images. Toxicity check results are returned to respective Lambda functions.
7. All Lambda functions generate a JSON-based payload to capture a user-requested action for [Epic Games Unreal Engine](https://www.unrealengine.com/en-US/home). Each payload is sent to a corresponding [Amazon Simple Notification Service (Amazon SNS)](https://aws.amazon.com/sns/) topic: `Translation` or `Non-Translation`.
8. Each Amazon SNS-based payload is transmitted to its corresponding [Amazon Simple Queue Service (Amazon SQS)]() queue for later consumption by Unreal Engine.
9. Using the [AWS SDK](https://aws.amazon.com/sdk-for-cpp/), the Unreal Engine application polls and dequeues Amazon SQS action-based payloads from its queues. Background images are fetched from an S3 bucket for translation requests.
10. Based on each payload received, the Unreal Engine application performs a user-requested action and
displays resulting video output on that user’s system. This output provides an ASL-equivalent interpretation of an input phrase by displaying a [MetaHuman](https://www.unrealengine.com/marketplace/en-US/product/metahuman-plugin) 3D avatar animation with ASL-transformed text displayed.

### AWS services in this Guidance

| **AWS service**                                              | Role       |                                                                   |
| ------------------------------------------------------------ | ---------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [Amazon Transcribe](https://aws.amazon.com/transcribe/) | Core | Convert user speech to text. |
| [Amazon Bedrock](https://aws.amazon.com/bedrock/)      | Core       | Invoke Foundation model to translate natural language to ASL.|
| [Amazon API Gateway](https://aws.amazon.com/api-gateway/)    | Core       | Create API to invoke lambda function from user interface.  |
| [AWS Lambda](https://aws.amazon.com/lambda/)                 | Core       | Run custom code to generate ASL for simplified text.  |
| [Amazon Cognito](https://aws.amazon.com/pm/cognito/)          | Core       | Authenticate user to access ASL translator |
| [Amazon Comprehend](https://aws.amazon.com/comprehend/)          | Core       | Run moderation to detect toxicty on generated text |
| [Amazon Rekognition](https://aws.amazon.com/rekognition/)          | Core       | Run moderation to detect toxicty on generated image |
| [Amazon Cloudfront](https://aws.amazon.com/cloudfront/)          | Core       | Secure , faster user experience  |
| [Amazon Simple Storage Service (S3)](https://aws.amazon.com/pm/serv-s3/)          | Core       | Host user interface code  |
| [Amazon Simple Notification Service (SNS)](https://aws.amazon.com/sns/)          | Core       | Send the notification to Unreal Engine  |
| [Amazon Simple Queue Service (SQS)](https://aws.amazon.com/sqs/)          | Core       | Unreal Engine consumes the message from queue  |

   
### Cost

<!--
This section is for a high-level cost estimate. Think of a likely straightforward scenario with reasonable assumptions based on the problem the Guidance is trying to solve. If applicable, provide an in-depth cost breakdown table in this section.

Start this section with the following boilerplate text:
_You are responsible for the cost of the AWS services used while running this Guidance. As of <month> <year>, the cost for running this Guidance with the default settings in the <Default AWS Region (Most likely will be US East (N. Virginia)) > is approximately $<n.nn> per month for processing ( <nnnnn> records )._
Replace this amount with the approximate cost for running your Guidance in the default Region. This estimate should be per month and for processing/serving resonable number of requests/entities.
Suggest you keep this boilerplate text:
_We recommend creating a [Budget](https://docs.aws.amazon.com/cost-management/latest/userguide/budgets-managing-costs.html) through [AWS Cost Explorer](https://aws.amazon.com/aws-cost-management/aws-cost-explorer/) to help manage costs. Prices are subject to change. For full details, refer to the pricing webpage for each AWS service used in this Guidance._
-->

### Sample Cost Table

<!--
The following table provides a sample cost breakdown for deploying this Guidance with the default parameters in the US East (N. Virginia) Region for one month.

| AWS service  | Dimensions | Cost [USD] |
| ----------- | ------------ | ------------ |
| Amazon API Gateway | 1,000,000 REST API calls per month  | $ 3.50month |
| Amazon Cognito | 1,000 active users per month without advanced security feature | $ 0.00 |
-->

**TO DO: edit the table below with actual costs**

The following table provides a sample cost breakdown for deploying this guidance with the default parameters in the `us-east-1` (N. Virginia) Region for one month. This estimate is based on the AWS Pricing Calculator output for the full deployment as per the guidance.

| AWS service  | Dimensions | Cost [USD] |
| ----------- | ------------ | ------------ |
| Amazon Transcribe | 5,000 requests, 34 KB request size | <$1/month |
| Amazon Bedrock | 10 users  | <$1/month |
| Amazon API Gateway | 5,000 requests, 128 MB memory allocation, 25 s duration | <$1/month |
| AWS Lambda (event trigger) | 5,000 requests, 128 MB memory allocation, 5 s duration | <$1/month |
| Amazon Cognito | 1 GB storage  | <$2/month |
| Amazon Comprehend | 10 GB standard storage  | <$1/month |
| Amazon Rekognition | 200 input / 300 output tokens per request (5,000 requests) | $44/month |
| Amazon S3 | 200 input / 300 output tokens per request (5,000 requests) | $26/month |
| Amazon SNS | 730 hours x 1.125 USD/hour | $821/month |
| Amazon SQS | 730 hours x 1.125 USD/hour | $821/month |

## Prerequisites (required)

### Operating System (required)

- Talk about the base Operating System (OS) and environment that can be used to run or deploy this Guidance, such as *Mac, Linux, or Windows*. Include all installable packages or modules required for the deployment. 
- By default, assume Amazon Linux 2/Amazon Linux 2023 AMI as the base environment. All packages that are not available by default in AMI must be listed out.  Include the specific version number of the package or module.

**Example:**
“These deployment instructions are optimized to best work on **<Amazon Linux 2 AMI>**.  Deployment in another OS may require additional steps.”

- Include install commands for packages, if applicable.


### Third-party tools (If applicable)

*List any installable third-party tools required for deployment.*


### AWS account requirements (If applicable)

*List out pre-requisites required on the AWS account if applicable, this includes enabling AWS regions, requiring ACM certificate.*

**Example:** “This deployment requires you have public ACM certificate available in your AWS account”

**Example resources:**
- ACM certificate 
- DNS record
- S3 bucket
- VPC
- IAM role with specific permissions
- Enabling a Region or service etc.


### aws cdk bootstrap (if sample code has aws-cdk)

<If using aws-cdk, include steps for account bootstrap for new cdk users.>

**Example blurb:** “This Guidance uses aws-cdk. If you are using aws-cdk for first time, please perform the below bootstrapping....”

### Service limits  (if applicable)

<Talk about any critical service limits that affect the regular functioning of the Guidance. If the Guidance requires service limit increase, include the service name, limit name and link to the service quotas page.>

### Supported Regions


This Guidance uses the Amazon Bedrock service, which is not currently available in all AWS Regions. You must launch this solution in
an AWS Region where Amazon Bedrock is available. For the most current availability of AWS services by Region, refer to the [AWS Regional Services
List](https://aws.amazon.com/about-aws/global-infrastructure/regional-product-services/).

American Sign Language (ASL) 3D Avatar Translator on AWS is supported in the following AWS Regions (as of Feb 2025):


| **Region Name**  | | 
|-----------|------------|
|US East (Ohio) | Asia Pacific ( Seoul ) |
|US East (N. Virginia) | Europe (Paris) |


## Deployment Steps 
<!--
Deployment steps must be numbered, comprehensive, and usable to customers at any level of AWS expertise. The steps must include the precise commands to run, and describe the action it performs.

* All steps must be numbered.
* If the step requires manual actions from the AWS console, include a screenshot if possible.
* The steps must start with the following command to clone the repo. ```git clone xxxxxxx```
* If applicable, provide instructions to create the Python virtual environment, and installing the packages using ```requirement.txt```.
* If applicable, provide instructions to capture the deployed resource ARN or ID using the CLI command (recommended), or console action.

 
**Example:**

1. Clone the repo using command ```git clone xxxxxxxxxx```
2. cd to the repo folder ```cd <repo-name>```
3. Install packages in requirements using command ```pip install requirement.txt```
4. Edit content of **file-name** and replace **s3-bucket** with the bucket name in your account.
5. Run this command to deploy the stack ```cdk deploy``` 
6. Capture the domain name created by running this CLI command ```aws apigateway ............```
-->
### Deployment Instructions

Please refer to [Full Implementation Guide](https://implementationguides.kits.eventoutfitters.aws.dev/asl-3dava-0829/ai-ml/american-sign-language-3d-avatar-translator-on-aws.html) for detailed instructions for all deployment options.


## Deployment Validation  (required)

<Provide steps to validate a successful deployment, such as terminal output, verifying that the resource is created, status of the CloudFormation template, etc.>


**Examples:**

* Open CloudFormation console and verify the status of the template with the name starting with xxxxxx.
* If deployment is successful, you should see an active database instance with the name starting with <xxxxx> in        the RDS console.
*  Run the following CLI command to validate the deployment: ```aws cloudformation describe xxxxxxxxxxxxx```



## Running the Guidance (required)

<Provide instructions to run the Guidance with the sample data or input provided, and interpret the output received.> 

This section should include:

* Guidance inputs
* Commands to run
* Expected output (provide screenshot if possible)
* Output description



## Next Steps

Overall, this prototype illustrates an end-to-end workflow for American Sign Language (ASL) translation through the 
use of a 3D avatar. Ideally, generated pre-ASL output phrases should be forwarded to a robust ASL processing engine, 
which would then generate corresponding avatar animation data to be supplied to Unreal Engine.

Suggested future enhancements:

* LiveLink capability
  * Mechanism to dynamically feed external animation data into UE to control individual skeleton joints per animation 
    frame (versus a playing limited selection of pre-built UE animations)
* Animation Montage with Blending
  * Make ASL signing animations more natural/less abrupt/smooth, easier to decipher sentences, allow for multiple 
    animations at once across more parts of the skeleton rig (i.e. face, hands etc).
* Animations of additional body components (i.e. face)
* Extend architecture to support other variants of Sign Language
* Design/re-architect to support ASL translation processing performed outside of UE
* Integrate lambdas ASL "pre-processor" logic with a robust external ASL engine/API, compare before/after output.
* Application integration 
  * Stream ASL animations to major chat programs, batch offline processing of videos 
* Extend translation capability to other sign-language variants

## Cleanup

- Include detailed instructions, commands, and console actions to delete the deployed Guidance.
- If the Guidance requires manual deletion of resources, such as the content of an S3 bucket, please specify.


## FAQ, known issues, additional considerations, and limitations (optional)


**Known issues (optional)**

<If there are common known issues, or errors that can occur during the Guidance deployment, describe the issue and resolution steps here>


**Additional considerations (if applicable)**

<Include considerations the customer must know while using the Guidance, such as anti-patterns, or billing considerations.>

**Examples:**

- “This Guidance creates a public AWS bucket required for the use-case.”
- “This Guidance created an Amazon SageMaker notebook that is billed per hour irrespective of usage.”
- “This Guidance creates unauthenticated public API endpoints.”


Provide a link to the *GitHub issues page* for users to provide feedback.


**Example:** *“For any feedback, questions, or suggestions, please use the issues tab under this repo.”*

## License

This library is licensed under the MIT-0 License. See the [LICENSE](LICENSE) file.

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

* David Israel, Sr Spatial Consultant
* Daniel Zilberman, Sr Solutions Architect - Tech Solutions
* Alain Krok, Sr Prototyping Architect
