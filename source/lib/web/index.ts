import * as fs from 'fs';
import {IdentityPool} from '@aws-cdk/aws-cognito-identitypool-alpha';
import {Stack} from 'aws-cdk-lib';
import {Distribution} from 'aws-cdk-lib/aws-cloudfront';
import {UserPool, UserPoolClient, UserPoolDomain}  from 'aws-cdk-lib/aws-cognito';
import {Bucket, IBucket} from 'aws-cdk-lib/aws-s3';
import {BucketDeployment, Source} from 'aws-cdk-lib/aws-s3-deployment';

import { Construct } from "constructs";
import path = require('path');

export interface AslMetahumanDeploymentProps {
    hostingBucket: Bucket;
    distribution: Distribution;
    userPool: UserPool;
    userPoolClient: UserPoolClient;
    identityPool: IdentityPool;
    userPoolDomain: UserPoolDomain;
}

export interface RuntimeConfig {
    Auth: { [key: string]: string };
    oauth: { [key: string]: string | string[] };
}

export class AslMetahumanDeployment extends Construct {
    public readonly runtimeConfig: RuntimeConfig;

    constructor(scope: Construct, id: string, props: AslMetahumanDeploymentProps) {
        super(scope, id);

        this.runtimeConfig = this.getRuntimeConfig(props);
        const awsExports = Source.jsonData('aws-export.json', this.runtimeConfig);

        const appPath = './frontend';
        const buildPath = `${appPath}/dist`;

        try {
            let len = fs.readdirSync(buildPath).length;
            if (!len) {
                console.error('No files in dist folder');
                process.exit(1);
            }
        } catch (e) {
            console.error(e);
            process.exit(1);
        }

        //const reactApp = Source.asset(buildPath);
        const reactApp = Source.asset(path.join(__dirname, '../../frontend'));
        

        new BucketDeployment(this, 'UserInterfaceDeployment', {
            sources: [reactApp, awsExports],
            destinationBucket: props.hostingBucket,
            distribution: props.distribution,
            memoryLimit: 4096,
        });
    }

    public getRuntimeConfig(props: AslMetahumanDeploymentProps): RuntimeConfig {
        return {
            Auth: {
                region: Stack.of(this).region,
                identityPoolId: props.identityPool.identityPoolId,
                userPoolId: props.userPool.userPoolId,
                userPoolWebClientId: props.userPoolClient.userPoolClientId,
            },
            oauth: {
                domain: [props.userPoolDomain.domainName, 'auth', Stack.of(this).region, 'amazoncognito.com'].join('.'),
                scope: ['email', 'openid', 'aws.signin.user.admin'],
                redirectSignIn: '',
                redirectSignOut: '',
                responseType: 'code',
            },
        };
    }
}

