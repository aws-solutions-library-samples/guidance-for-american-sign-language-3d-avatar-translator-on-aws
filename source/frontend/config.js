var appConfig = {
    "IdentityPoolId": "<AwsAslCdkStack.IdentityPoolId>"
}

var amplifyConfig = {
    "Auth": {
        "region": "us-east-1",
        "userPoolId": "<AwsAslCdkStack.UserPoolId>",
        "userPoolWebClientId": "<AwsAslCdkStack.ClientId>",
        "mandatorySignIn": true,
        "cookieStorage": {
            "domain": window.location.hostname,
            "path": "/",
            "expires": 30,
            "secure": true
      }
    }
}

var httpApi = {
    "apipath": "<AwsAslCdkStack.apigwurl>"
 }