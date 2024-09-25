import { Construct } from 'constructs';
import {
    AdvancedSecurityMode,
    CfnUserPoolUser,
    UserPool,
    UserPoolClient, UserPoolDomain,
    CfnIdentityPoolRoleAttachment
} from "aws-cdk-lib/aws-cognito";
import {FederatedPrincipal, IRole, PolicyStatement, Role} from "aws-cdk-lib/aws-iam";
import {Aws} from "aws-cdk-lib";
import * as cdk from 'aws-cdk-lib';

import {IdentityPool, UserPoolAuthenticationProvider} from "@aws-cdk/aws-cognito-identitypool-alpha";
import { NagSuppressions } from 'cdk-nag';

/**
 * Represents the AslMetahuman authentication construct.
 * @class
 * @constructor
 * @param {Construct} scope - The parent construct.
 * @param {string} id - The ID of the construct.
 * @param {AslMetahumanAuthProps} props - The properties of the construct.
 */
export class AslMetahumanAuth extends Construct {
    userPool: UserPool;
    authorizedRole: Role;
    userPoolDomain: UserPoolDomain;
    identityPool: IdentityPool;
    userPoolClient: UserPoolClient;
    user: CfnUserPoolUser;

    constructor(scope: Construct, id: string) {
        super(scope, id);

        this.userPool = new UserPool(this, 'UserPool', {
            userPoolName: 'augmentability-user-pool',
            selfSignUpEnabled: true,
            passwordPolicy: {
                requireDigits: true,
                requireLowercase: true,
                requireUppercase: true,
                requireSymbols: true,
                minLength: 8,
            },
            signInAliases: {
                username: true,
                email: true,
            },
            advancedSecurityMode: AdvancedSecurityMode.ENFORCED,
        });

        this.userPoolClient = new UserPoolClient(this, 'UserPoolClient', {
            userPool: this.userPool,
            generateSecret: false,
            authFlows: {
                adminUserPassword: true,
                userSrp: true,
                userPassword: true,
                custom: true
            }
        });

        const uniqueStackIdPart = cdk.Fn.select(2, cdk.Fn.split('/', `${cdk.Aws.STACK_ID}`));

        this.userPoolDomain = this.userPool.addDomain('AslMetahumanUserPoolDomain', {
            cognitoDomain: {
                //domainPrefix: ('aslmetahuman').toLowerCase(),
                domainPrefix: uniqueStackIdPart,
            },
        });

        this.identityPool = new IdentityPool(this, 'AslMetahumanIdentityPool', {
            allowUnauthenticatedIdentities: false,
            authenticationProviders: {
                userPools: [
                    new UserPoolAuthenticationProvider({
                        userPool: this.userPool,
                        userPoolClient: this.userPoolClient,
                    }),
                ],
            },
            authenticatedRole:this.authorizedRole
        });

        this.authorizedRole = new Role(this, 'CognitoAuthorizedRole', {
            assumedBy: new FederatedPrincipal(
                'cognito-identity.amazonaws.com',
                {
                    StringEquals: {
                        'cognito-identity.amazonaws.com:aud': this.identityPool.identityPoolId,
                    },
                    'ForAnyValue:StringLike': {
                        'cognito-identity.amazonaws.com:amr': 'authenticated',
                    },
                },
                'sts:AssumeRoleWithWebIdentity',
            ),
        });

        this.authorizedRole.addToPolicy(new PolicyStatement({
            actions: [
                'transcribe:StartStreamTranscriptionWebSocket',
                'translate:TranslateText',
                'comprehend:DetectDominantLanguage',
                'polly:SynthesizeSpeech',
                'rekognition:DetectText',
                'rekognition:DetectLabels',
                'textract:DetectDocumentText',
            ],
            resources: ['*'],
            conditions: {
                StringEquals: {
                    'aws:RequestedRegion': Aws.REGION,
                },
            },
        }));

    
          
        NagSuppressions.addResourceSuppressions(
            this.authorizedRole,
            [
                {
                id: 'AwsSolutions-IAM5',
                reason: 'AWSLambdaBasicExecutionRole is used.',
                },
            ],
            true,
        );
    }
    
}
