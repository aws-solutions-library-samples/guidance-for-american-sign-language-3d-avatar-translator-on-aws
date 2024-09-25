import {Certificate, CertificateValidation} from 'aws-cdk-lib/aws-certificatemanager';
import {
    AllowedMethods, CachePolicy, CachedMethods, OriginRequestPolicy, ResponseHeadersPolicy,
    SecurityPolicyProtocol, ViewerProtocolPolicy, Distribution, HttpVersion, CfnDistribution, CfnOriginAccessControl
} from 'aws-cdk-lib/aws-cloudfront';
import {S3Origin} from 'aws-cdk-lib/aws-cloudfront-origins';
import * as iam from 'aws-cdk-lib/aws-iam';
import {NagSuppressions} from 'cdk-nag';
import {Construct} from 'constructs';
import {ArnFormat, RemovalPolicy, Stack} from "aws-cdk-lib";
import {BlockPublicAccess, Bucket, BucketAccessControl, BucketEncryption, CfnBucketPolicy} from "aws-cdk-lib/aws-s3";
import {ARecord, IHostedZone, RecordTarget} from "aws-cdk-lib/aws-route53";
import {CloudFrontTarget} from "aws-cdk-lib/aws-route53-targets";
import * as cdk from 'aws-cdk-lib';

export interface CloudFrontDistributionForS3Props {
    hostingBucket: Bucket;
    loggingBucket: Bucket;
    env: string;
    // altDomainCname: string;
    // cert: Certificate;
}

export function CloudFrontDistributionForS3(
    scope: Construct,
    props: CloudFrontDistributionForS3Props,
): Distribution {
    const oac = new CfnOriginAccessControl(scope, 'OriginAccessControl', {
        originAccessControlConfig: {
            name: `AslMetahuman-${props.env}-origin-access-control`,
            originAccessControlOriginType: 's3',
            signingBehavior: 'always',
            signingProtocol: 'sigv4',
        },
    });

    const distribution = new Distribution(scope, 'Distribution', {
        //domainNames: [props.altDomainCname],
        //certificate: props.cert,
        defaultRootObject: 'index.html',
        httpVersion: HttpVersion.HTTP2_AND_3,
        defaultBehavior: {
            origin: new S3Origin(props.hostingBucket),
            allowedMethods: AllowedMethods.ALLOW_GET_HEAD_OPTIONS,
            viewerProtocolPolicy: ViewerProtocolPolicy.REDIRECT_TO_HTTPS,
            cachePolicy: CachePolicy.CACHING_OPTIMIZED,
            cachedMethods: CachedMethods.CACHE_GET_HEAD_OPTIONS,
            responseHeadersPolicy: ResponseHeadersPolicy.CORS_ALLOW_ALL_ORIGINS,
            originRequestPolicy: OriginRequestPolicy.CORS_S3_ORIGIN,
        },
        minimumProtocolVersion: SecurityPolicyProtocol.TLS_V1_2_2021,
        enableLogging: true,
        logBucket: props.loggingBucket,
        logFilePrefix: `cloudfront-logs/${props.env}/`,
        errorResponses: [
            {
                httpStatus: 403,
                responseHttpStatus: 200,
                responsePagePath: '/index.html',
            },
        ],
    });

    NagSuppressions.addResourceSuppressions(distribution, [
        {id: 'AwsSolutions-CFR1', reason: 'This is globally available.'},
    ]);

    const cfnDistribution = distribution.node.defaultChild as CfnDistribution;

    cfnDistribution.addPropertyOverride('DistributionConfig.Origins.0.S3OriginConfig.OriginAccessIdentity', '');
    //cfnDistribution.addPropertyOverride('DistributionConfig.0.OriginAccessControlId', oac.getAtt('Id'));

    cfnDistribution.addPropertyOverride('DistributionConfig.Origins.0.OriginAccessControlId', oac.getAtt('Id'))


    //const comS3PolicyOverride = props.hostingBucket.node.findChild('Policy').node.defaultChild as CfnBucketPolicy;
    //comS3PolicyOverride.addDependency(oac);

    //const statement = comS3PolicyOverride.policyDocument.statements[2];

    // if (statement._principal && statement._principal.CanonicalUser) {
    //     delete statement._principal.CanonicalUser;
    // }

    props.hostingBucket.addToResourcePolicy(
        new iam.PolicyStatement({
            effect: iam.Effect.ALLOW,
            principals: [new iam.ServicePrincipal('cloudfront.amazonaws.com')],
            actions: ['s3:GetObject'],
            resources: [`${props.hostingBucket.bucketArn}/*`],
            conditions: {
                StringEquals: {
                    'AWS:SourceArn': Stack.of(scope).formatArn({
                        service: 'cloudfront',
                        region: '',
                        resource: 'distribution',
                        resourceName: distribution.distributionId,
                        arnFormat: ArnFormat.SLASH_RESOURCE_NAME,
                    }),
                },
            },
        })
    );
    

    // comS3PolicyOverride.addPropertyOverride('PolicyDocument.Statement.2.Principal', {
    //     Service: 'amazonaws.com',
    // });

    // comS3PolicyOverride.addPropertyOverride('PolicyDocument.Statement.2.Condition', {
    //     StringEquals: {
    //         'AWS:SourceArn': Stack.of(scope).formatArn({
    //             service: 'cloudfront',
    //             region: '',
    //             resource: 'distribution',
    //             resourceName: distribution.distributionId,
    //             arnFormat: ArnFormat.SLASH_RESOURCE_NAME,
    //         }),
    //     },
    // });
    //cfnDistribution.addPropertyOverride('DistributionConfig.0.S3OriginConfig.OriginAccessIdentity', '');
    //cfnDistribution.addDeletionOverride('DistributionConfig.0.S3OriginConfig.OriginAccessIdentity');

    return distribution;
}

export interface AslMetahumanHostingProps {
    /**
     * The environment in which AslMetahuman is hosted.
     */
    env: string;
}

/**
 * Represents the AslMetahuman authentication construct.
 * @class
 * @constructor
 * @param {Construct} scope - The parent construct.
 * @param {string} id - The ID of the construct.
 * @param {AslMetahumanHostingProps} props - The properties of the construct.
 * @property {Bucket} hostingBucket - The S3 bucket for hosting.
 * @property {cloudfront.Distribution} distribution - The CloudFront distribution.
 * @property {Bucket} loggingBucket - The S3 bucket for logging.
 * @property {ARecord} domainName - The domain name.
 */
export class AslMetahumanHosting extends Construct {
    public readonly hostingBucket: Bucket;
    public readonly distribution: Distribution;
    public readonly loggingBucket: Bucket;
    public readonly domainName: ARecord;

    constructor(scope: Construct, id: string, props: AslMetahumanHostingProps) {
        super(scope, id);

        this.loggingBucket = new Bucket(this, 'LoggingBucket', {
            autoDeleteObjects: false,
            enforceSSL: true,
            encryption: BucketEncryption.S3_MANAGED,
            blockPublicAccess: BlockPublicAccess.BLOCK_ALL,
            publicReadAccess: false,
            accessControl: BucketAccessControl.LOG_DELIVERY_WRITE,  
        });

        this.loggingBucket.addToResourcePolicy(
            new iam.PolicyStatement({
                effect: iam.Effect.ALLOW,
                principals: [new iam.ServicePrincipal('delivery.logs.amazonaws.com')],
                actions: ['s3:PutObject'],
                resources: [`${this.loggingBucket.bucketArn}/*`],
                conditions: {
                    StringEquals: {
                        's3:x-amz-acl': 'bucket-owner-full-control',
                    },
                },
            })
        );
        

        this.hostingBucket = new Bucket(this, 'HostingBucket', {
            enforceSSL: true,
            autoDeleteObjects: true,
            removalPolicy: RemovalPolicy.DESTROY,
            encryption: BucketEncryption.S3_MANAGED,
            blockPublicAccess: BlockPublicAccess.BLOCK_ALL,
            publicReadAccess: false,
            serverAccessLogsBucket: this.loggingBucket,
            serverAccessLogsPrefix: `s3-access-logs/${props.env}/`
        });

        // const altDomainCname =  'demos.aws.dev';
        // const certificate = new Certificate(this, 'CloudFrontCertificate', {
        //     domainName: altDomainCname,
        // });

        this.distribution = CloudFrontDistributionForS3(this, {
            hostingBucket: this.hostingBucket,
            loggingBucket: this.loggingBucket,
            env: props.env,
            // altDomainCname: altDomainCname,
            // cert: certificate,
        });

        new cdk.CfnOutput(this, "domain name ", {
            value: this.distribution.domainName,
          });


      new cdk.CfnOutput(this, "CfnOutCloudFrontUrl", {
        value: `https://${this.distribution.distributionDomainName}`,
        description: "The CloudFront URL",
      });

      new cdk.CfnOutput(this, "distributionId", {
        value: this.distribution.distributionId,
        description: "CloudFront Distribution Id",
      });
    }
}

