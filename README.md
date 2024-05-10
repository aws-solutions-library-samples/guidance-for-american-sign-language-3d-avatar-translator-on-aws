# Guidance for American Sign Language (ASL) 3D Avatar Translator on AWS

![Alt text](assets/images/rainbow-example.png?raw=true "Sample Translation")
<video src="assets/videos/translation-example.mp4"></video>

Currently, this repository consists purely of documentation for a Spatial Computing/GenAI prototype solution that was 
presented at Amazon's re:Invent 2023 conference. In the coming months, it will have polished code posted, which 
includes the following major components:
   - A CDK-deployable project with lambda code
   - Sample code for a web frontend interface
   - An Unreal Engine 5.3 C++ project

Please check back for future updates.

## Table of Contents

1. [Overview](#overview-required)
    - [Cost](#cost)
2. [High-level Workflow](#High-level Workflow)
2. [Prerequisites](#prerequisites-required)
    - [Operating System](#operating-system-required)
3. [Deployment Steps](#deployment-steps-required)
4. [Deployment Validation](#deployment-validation-required)
5. [Running the Guidance](#running-the-guidance-required)
6. [Next Steps](#next-steps-required)
7. [Cleanup](#cleanup-required)

***Optional***

8. [FAQ, known issues, additional considerations, and limitations](#faq-known-issues-additional-considerations-and-limitations-optional)
9. [Revisions](#revisions-optional)
10. [Notices](#notices-optional)
11. [Authors](#authors-optional)

## Overview

This prototype demonstrates a novel way to pre-process/transform multilingual phrases into an equivalent 
literal (or direct) form for visual translation into American Sign Language (ASL). This pre-processing step improves 
sign language translation fidelity by expressing phrases more clearly than they were initially expressed. GenAI is 
applied to re-interpret these multilingual input phrases into 'simpler form' English phrases across multiple 
iterations/passes. Resulting phases have a different (more charitable) kind of interpretation versus phrases 
produced by traditional translation tools. Finally, this prototype animates an avatar in Unreal Engine (MetaHuman 
plug-in) to depict the ASL translation of those resulting phrases. Visual ASL translation in this prototype is based 
on a loose/naïve interpretation of ASL rules and grammar, involving primarily hand and arm movements, which end users 
can refine.

### Architecture Diagram

![Alt text](assets/images/architecture.png?raw=true "Architecture")

### Architecture Diagram Workflow
1. User authenticates to the web application using Amazon Cognito
2. Logged in user can record his audio voice which will be transcribed using Amazon Transcribe. The output text will
   be displayed on the page.
3. The user can can perform multiple actions through the user interface. All the actions listed below will be 
   triggered through an Amazon API gateway:
    - ASL translation: the transcribed text is sent to a Lambda function for processing. The text is simplified and 
      translated to english using Amazon Bedrock. Message is analyzed for toxicity using Amazon Comprehend
    - Avatar change: a different avatar can be selected for display in the end application
    - Change background image: the end application background image can be generated using Amazon Bedrock. A 
      moderation step will be performed on the generated image to detect unsafe content using Amazon Rekognition.
    - Stop all activities: will stop all the queued activities on the Unreal Engine application side
    - Change settings: will update settings like the sign rate on the Unreal Engine application side
4. All messages related to translation are published to an Amazon Simple Notification Service topic. Other messages 
   from other actions are publish to a second topic.
5. Messages are then transmitted to two FIFO Amazon Simple Queue Service (SQS) queues
6. An Unreal Engine application with the Metahuman plugin, running on an Amazon Elastic Cloud Compute (EC2) instance 
   is subscribed to the SQS queue to consume messages, and process them 
7. The user can access remotely the Amazon EC2 instance through Remote Desktop Protocol / NiceDCV and visualize the 
   3D avatar
   
### Cost ( required )

This section is for a high-level cost estimate. Think of a likely straightforward scenario with reasonable assumptions based on the problem the Guidance is trying to solve. If applicable, provide an in-depth cost breakdown table in this section.

Start this section with the following boilerplate text:

_You are responsible for the cost of the AWS services used while running this Guidance. As of <month> <year>, the cost for running this Guidance with the default settings in the <Default AWS Region (Most likely will be US East (N. Virginia)) > is approximately $<n.nn> per month for processing ( <nnnnn> records )._

Replace this amount with the approximate cost for running your Guidance in the default Region. This estimate should be per month and for processing/serving resonable number of requests/entities.

Suggest you keep this boilerplate text:
_We recommend creating a [Budget](https://docs.aws.amazon.com/cost-management/latest/userguide/budgets-managing-costs.html) through [AWS Cost Explorer](https://aws.amazon.com/aws-cost-management/aws-cost-explorer/) to help manage costs. Prices are subject to change. For full details, refer to the pricing webpage for each AWS service used in this Guidance._

### Sample Cost Table ( required )

The following table provides a sample cost breakdown for deploying this Guidance with the default parameters in the US East (N. Virginia) Region for one month.

| AWS service  | Dimensions | Cost [USD] |
| ----------- | ------------ | ------------ |
| Amazon API Gateway | 1,000,000 REST API calls per month  | $ 3.50month |
| Amazon Cognito | 1,000 active users per month without advanced security feature | $ 0.00 |

### High-level Workflow

1) an end user speaks (or types) a phrase in a spoken language of choice
2) that spoken phrase is transcribed directly
3) the transcribed phrase is translated via GenAI into English, which is then simplified across multiple iterations 
   using carefully-crafted Bedrock prompts
4) an Avatar in Unreal Engine animates ASL gestures ("signs") corresponding to the simplified transcription

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

### Supported Regions (if applicable)

<If the Guidance is built for specific AWS Regions, or if the services used in the Guidance do not support all Regions, please specify the Region this Guidance is best suited for>


## Deployment Steps (required)

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



## Next Steps (required)

Overall, this prototype illustrates an end-to-end workflow for American Sign Language (ASL) translation through the 
use of a 3D avatar. Ideally, generated pre-ASL output phrases should be forwarded to a robust ASL processing engine, 
which would then generate corresponding avatar animation data to be supplied to Unreal Engine.

## Cleanup (required)

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

## Revisions (optional)

Document all notable changes to this project.

Consider formatting this section based on Keep a Changelog, and adhering to Semantic Versioning.


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

David Israel
Daniel Zilberman
Alain Krok
