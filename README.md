# Guidance for American Sign Language (ASL) 3D Avatar Translator on AWS

## Table of Contents

1. [Overview](#overview)
    - [Architecture](#architecture)
    - [Services in this guidance](#services-in-this-guidance)
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

1) an end user speaks (or types) a phrase in a spoken language of choice
2) that spoken phrase is transcribed directly
3) the transcribed phrase is translated via GenAI into English, which is then simplified across multiple iterations 
   using carefully-crafted Bedrock prompts
4) an Avatar in Unreal Engine animates ASL gestures ("signs") corresponding to the simplified transcription

## Architecture

<!-- [Alt text](assets/images/architecture.png?raw=true "Architecture") -->

![Reference Architecture](assets/images/american-sign-language-3d-avatar-translator-on-aws.7ad539a2a8f330eb3f8abcdd305401dc6a03c853.png?raw=true "Architecture") 

### Architecture Diagram Workflow
1. User authenticates to the provided sample web interface via [Amazon Cognito](https://aws.amazon.com/cognito/)
2. User speaks a phrase, which will be transcribed using [Amazon Transcribe](https://aws.amazon.com/transcribe/), or optionally types a phrase. 
   (Transcribed text will be displayed as it is spoken.)
3. User requests any of the following actions through the web interface (actions are triggered through [Amazon API Gateway](https://aws.amazon.com/api-gateway/):
    - Perform ASL Translation
      - Transcribed phrase text is sent to a [AWS Lambda](https://aws.amazon.com/lambda/) function for processing. This text is then simplified and 
        translated into English using [Amazon Bedrock](https://aws.amazon.com/bedrock/) managed service. During this process, processed text is analyzed 
        for toxicity using [Amazon Comprehend](https://aws.amazon.com/comprehend/).
    - Change the Avatar (which is performing ASL translation)
      - Different avatars can be selected and the selected avatar will then display in the [Unreal Engine](https://www.unrealengine.com/en-US) sample 
        application
    - Change the Background Image (representing the meaning of the translated phase):
      - A background image in Unreal Engine is generated using Amazon Bedrock. Once generated, a moderation check 
        will be performed on the generated image to detect unsafe content using [Amazon Rekognition](https://aws.amazon.com/rekognition/).
    - Stop all Active Activities (in Unreal Engine)
      - This option will suspend all queued activities (listed above) in Unreal Engine
    - Change Settings
      - This option can update custom settings (including ASL signing speed) in Unreal Engine
4. Each message that specifically relates to ASL translation is pushed to an [Amazon Simple Notification Service (SNS)](https://aws.amazon.com/sns/) 
   topic. Other message types (from other actions) are publish to another (separate) SNS topic.
5. SNS messages are then transmitted to two first-in-first-out (FIFO)-based [Amazon Simple Queue Service (SQS)](https://aws.amazon.com/sqs/) queues
6. The Unreal Engine sample application, which includes the MetaHuman plugin, is running on an [Amazon Elastic 
   Cloud Compute (EC2)](https://aws.amazon.com/ec2) instance or on a client-based machine. That application is subscribed to the SQS queue, 
   periodically polling it to consume messages (when ready) and processes them. 
7. User can remotely access the Amazon EC2 instance through Remote Desktop Protocol or [NiceDCV (Amazon DCV)](https://aws.amazon.com/blogs/aws/nice-desktop-cloud-visualization-dcv-is-now-amazon-dcv/) software to visualize the 3D avatar that is performing ASL translation.

**TO DO: update Services list below**

### AWS services in this Guidance

| **AWS Service** | **Role** | **Description** |
|-----------------|----------|-----------------|
| [Amazon Elastic Kubernetes Service](https://aws.amazon.com/eks/) ( EKS) | Core service | Manages the Kubernetes control plane and worker nodes for container orchestration. |
| [Amazon Elastic Compute Cloud](https://aws.amazon.com/ec2/) (EC2) | Core service | Provides the compute instances for EKS worker nodes and runs containerized applications. |
| [Amazon Virtual Private Cloud](https://aws.amazon.com/vpc/) (VPC) | Core Service | Creates an isolated network environment with public and private subnets across multiple Availability Zones. |
| [Amazon Elastic Container Registry](http://aws.amazon.com/ecr/) (ECR) | Supporting service | Stores and manages Docker container images for EKS deployments. |
| [Elastic Load Balancing](https://aws.amazon.com/elasticloadbalancing/) (NLB) | Supporting service | Distributes incoming traffic across multiple targets in the EKS cluster. |
| [Amazon Elastic Block Store](https://aws.amazon.com/ebs) (EBS) | Supporting service | Provides persistent block storage volumes for EC2 instances in the EKS cluster. |
| [AWS Identity and Access Management](https://aws.amazon.com/iam/) (IAM) | Supporting service | Manages access to AWS services and resources securely, including EKS cluster access. |
| [Amazon Managed Grafana](https://aws.amazon.com/grafana/) (AMG) | Observability service | Provides fully managed  service for metrics visualization and monitoring. |
| [Amazon Managed Service for Prometheus](https://aws.amazon.com/prometheus/) (AMP) | Observability service | Offers managed Prometheus-compatible monitoring for container metrics. |
| [AWS Certificate Manager](https://aws.amazon.com/certificate-manager/) (ACM) | Security service | Manages SSL/TLS certificates for secure communication within the cluster. |
| [Amazon CloudWatch](https://aws.amazon.com/cloudwatch/) | Monitoring service | Collects and tracks metrics, logs, and events from EKS and other AWS resources provisoned in the guidance |
| [AWS Systems Manager](https://aws.amazon.com/systems-manager/) | Management service | Provides operational insights and takes action on AWS resources. |
| [AWS Key Management Service](https://aws.amazon.com/kms/) (KMS) | Security service | Manages encryption keys for securing data in EKS and other AWS services. |
   
### Cost

<!--
This section is for a high-level cost estimate. Think of a likely straightforward scenario with reasonable assumptions based on the problem the Guidance is trying to solve. If applicable, provide an in-depth cost breakdown table in this section.

Start this section with the following boilerplate text:
_You are responsible for the cost of the AWS services used while running this Guidance. As of <month> <year>, the cost for running this Guidance with the default settings in the <Default AWS Region (Most likely will be US East (N. Virginia)) > is approximately $<n.nn> per month for processing ( <nnnnn> records )._
Replace this amount with the approximate cost for running your Guidance in the default Region. This estimate should be per month and for processing/serving resonable number of requests/entities.
Suggest you keep this boilerplate text:
_We recommend creating a [Budget](https://docs.aws.amazon.com/cost-management/latest/userguide/budgets-managing-costs.html) through [AWS Cost Explorer](https://aws.amazon.com/aws-cost-management/aws-cost-explorer/) to help manage costs. Prices are subject to change. For full details, refer to the pricing webpage for each AWS service used in this Guidance._
-->

### Sample Cost Table ( required )

The following table provides a sample cost breakdown for deploying this Guidance with the default parameters in the US East (N. Virginia) Region for one month.

| AWS service  | Dimensions | Cost [USD] |
| ----------- | ------------ | ------------ |
| Amazon API Gateway | 1,000,000 REST API calls per month  | $ 3.50month |
| Amazon Cognito | 1,000 active users per month without advanced security feature | $ 0.00 |

**TO DO: edit the table below with actual services /costs**

The following table provides a sample cost breakdown for deploying this guidance with the default parameters in the `us-east-1` (N. Virginia) Region for one month. This estimate is based on the AWS Pricing Calculator output for the full deployment as per the guidance.

| **AWS service** | Dimensions | Cost, month [USD] |
|-----------------|------------|-------------------|
| Amazon EKS | 1 cluster | $73.00 |
| Amazon VPC | 3 NAT Gateways | $98.67 |
| Amazon EC2 | 2 m6g.large instances | $112.42 |
| Amazon Managed Service for Prometheus (AMP) | Metric samples, storage, and queries | $100.60 |
| Amazon Managed Grafana (AMG) | Metrics visualization - Editor and Viewer users | $14.00 |
| Amazon EBS | gp2 storage volumes and snapshots | $17.97 |
| Application Load Balancer | 1 ALB for workloads | $16.66 |
| Amazon VPC | Public IP addresses | $3.65 |
| AWS Key Management Service (KMS) | Keys and requests | $7.00 |
| Amazon CloudWatch | Metrics | $3.00 |
| Amazon ECR | Data storage | $0.50 |
| **TOTAL** |  | **$447.47/month** |

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
