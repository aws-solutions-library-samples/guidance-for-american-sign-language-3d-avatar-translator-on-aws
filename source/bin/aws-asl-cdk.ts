#!/usr/bin/env node
import 'source-map-support/register';
import * as cdk from 'aws-cdk-lib';
import { AwsSolutionsChecks } from 'cdk-nag'
import { AwsAslCdkStack } from '../lib/aws-asl-cdk-stack';

const app = new cdk.App();
cdk.Aspects.of(app).add(new AwsSolutionsChecks({ verbose: true }))
new AwsAslCdkStack(app, 'AwsAslCdkStack', {
  description: "This stack includes resources needed to deploy a static website with a processing workflow to translate audio input"
});