/*
Original Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
Modifications Copyright 2022 Amazon.com, Inc. or its affiliates. All Rights Reserved.
SPDX-License-Identifier: Apache-2.0
*/


const audioUtils = require("./audioUtils"); // for encoding audio data as PCM
const crypto = require("crypto"); // tot sign our pre-signed URL
const v4 = require("./aws-signature-v4"); // to generate our pre-signed URL
const marshaller = require("@aws-sdk/eventstream-marshaller"); // for converting binary event stream messages to and from JSON
const util_utf8_node = require("@aws-sdk/util-utf8-node"); // utilities for encoding and decoding UTF8
const mic = require("microphone-stream"); // collect microphone input as a stream of raw bytes


AWS.config.region = amplifyConfig.Auth.region;
aws_amplify.Amplify.configure(amplifyConfig);


aws_amplify.Amplify.Auth.currentAuthenticatedUser()
    .then((user) => {
        console.log('IdentityPoolId',  appConfig.IdentityPoolId);
        console.log('userPoolId',  appConfig.userPoolId);
        aws_amplify.Amplify.Auth.currentSession()
            .then((data) => {
                AWS.config.credentials = new AWS.CognitoIdentityCredentials({
                    IdentityPoolId: appConfig.IdentityPoolId,
                    Logins: {
                        ["cognito-idp." +
                        amplifyConfig.Auth.region +
                        ".amazonaws.com/" +
                        amplifyConfig.Auth.userPoolId]: data.getIdToken().getJwtToken(),
                    },
                });
                AWS.config.credentials.get(function (err) {
                    if (err) console.log(err);
                });
                if (
                    window.location.pathname == "/" ||
                    window.location.pathname == "/index.html"
                )
                    window.location.pathname = "/index-landing.html";
            })
            .catch((err) => {
                console.log(err);
            });
    })
    .catch((error) => {
        console.log(error);
        if (
            window.location.pathname != "/" &&
            window.location.pathname != "/index.html"
        )
            window.location.pathname = "/";
    });

function escapeHtml(unsafeText) {
    const div = document.createElement('div');
    const text = document.createTextNode(unsafeText);
    div.appendChild(text);
    return div.innerHTML;
}

function callApiOnClick(avatar) {
    aws_amplify.Amplify.Auth.currentAuthenticatedUser()
        .then((user) => {
            console.log("user ::",user);  
            aws_amplify.Amplify.Auth.currentSession()
                .then((data) => {
                    const accessToken = data.getAccessToken().getJwtToken();
                    const apiUrl = httpApi.baseurl+'/change_avatar';
                    fetch(apiUrl, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json',
                            'Authorization': accessToken,
                        },
                        body: JSON.stringify({"avatar": avatar})
                    })
                        .then(response => response.json())
                        .then(apiData => {
                            console.log('API Response:', apiData);
                        })
                        .catch(apiError => {
                            console.error('API Error:', apiError);
                        });
                })
                .catch((sessionError) => {
                    console.log(sessionError);
                });
        })
        .catch((authError) => {
            console.log(authError);
        });
}

function callTranscriptAPI() {
    var iterations = $("#simplification_iterations").val();
    var transcription = $("#transcript").val();
    if (iterations === "" || transcription === "") {
        return;
    }
    aws_amplify.Amplify.Auth.currentAuthenticatedUser()
        .then((user) => {
            aws_amplify.Amplify.Auth.currentSession()
                .then((data) => {
                    const accessToken = data.getAccessToken().getJwtToken();
                    const apiUrl = httpApi.baseurl+'/translate';
                    fetch(apiUrl, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json',
                            'Authorization': accessToken,
                        },
                        body: JSON.stringify({"message": transcription, "iterations": iterations})
                    })
                        .then(response => response.json())
                        .then(apiData => {
                            console.log('Transcript API Response:', apiData);

                        })
                        .catch(apiError => {
                            console.error('Transcript API Error:', apiError);
                        });
                })
                .catch((sessionError) => {
                    console.log(sessionError);
                });
        })
        .catch((authError) => {
            console.log(authError);
        });
}

function callStopApi() {
    aws_amplify.Amplify.Auth.currentAuthenticatedUser()
        .then((user) => {
            aws_amplify.Amplify.Auth.currentSession()
                .then((data) => {
                    const accessToken = data.getAccessToken().getJwtToken();
                    const apiUrl = httpApi.baseurl+'/stop_all';
                    fetch(apiUrl, {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json',
                            'Authorization': accessToken,
                        },
                    })
                        .then(response => response.json())
                        .then(apiData => {
                            console.log('Stop API Response:', apiData);
                        })
                        .catch(apiError => {
                            console.error('Stop API Error:', apiError);
                        });
                })
                .catch((sessionError) => {
                    console.log(sessionError);
                });
        })
        .catch((authError) => {
            console.log(authError);
        });
}

$("#sign-in").click(function () {
    var username = document.getElementById("username").value;
    var pass = document.getElementById("pwd").value;
    try {
        const user = aws_amplify.Amplify.Auth.signIn(username, pass);
        const myTimeout = setTimeout(function () {
            location.reload();
        }, 2000);
    } catch (error) {
        console.log(error);
        
    }
});


/**
 * Variables initilization
 */

// converter between binary event streams messages and JSON
const eventStreamMarshaller = new marshaller.EventStreamMarshaller(
    util_utf8_node.toUtf8,
    util_utf8_node.fromUtf8
);

let languageCode;
let region = amplifyConfig.Auth.region;
let sampleRate;
let inputSampleRate;
let transcription = "";
let socket;
let micStream;
let socketError = false;
let transcribeException = false;
let translation = "";
let translation2 = "";
let translated;
let detectedText = "";

const pollyVoiceMap = new Map();
pollyVoiceMap.set("ar", "Zeina");
pollyVoiceMap.set("arb", "Zeina");
pollyVoiceMap.set("zh", "Zhiyu");
pollyVoiceMap.set("zh-TW", "Zhiyu");
pollyVoiceMap.set("da", "Mads");
pollyVoiceMap.set("nl", "Ruben");
pollyVoiceMap.set("en", "Matthew");
pollyVoiceMap.set("fr", "Mathieu");
pollyVoiceMap.set("fr-CA", "Gabrielle");
pollyVoiceMap.set("de", "Vicki");
pollyVoiceMap.set("hi-IN", "Kajal");
pollyVoiceMap.set("en-IN", "Kajal");
pollyVoiceMap.set("is", "Karl");
pollyVoiceMap.set("it", "Bianca");
pollyVoiceMap.set("ja", "Takumi");
pollyVoiceMap.set("ko", "Seoyeon");
pollyVoiceMap.set("no", "Liv");
pollyVoiceMap.set("pl", "Jacek");
pollyVoiceMap.set("pt", "Cristiano");
pollyVoiceMap.set("ro", "Carmen");
pollyVoiceMap.set("ru", "Maxim");
pollyVoiceMap.set("es", "Conchita");
pollyVoiceMap.set("es-MX", "Mia");
pollyVoiceMap.set("es-US", "Lupe");
pollyVoiceMap.set("sv", "Astrid");
pollyVoiceMap.set("tr", "Filiz");
pollyVoiceMap.set("cy", "Gwyneth");
pollyVoiceMap.set("en-US", "Joanna");
pollyVoiceMap.set("en-GB", "Amy");
pollyVoiceMap.set("en-AU", "Olivia");
pollyVoiceMap.set("fr-FR", "Lea");
pollyVoiceMap.set("pt-BR", "Camila");
pollyVoiceMap.set("de-DE", "Vicki");
pollyVoiceMap.set("ja-JP", "Takumi");
pollyVoiceMap.set("it-IT", "Bianca");
pollyVoiceMap.set("ko-KR", "Seoyeon");
pollyVoiceMap.set("zh-CN", "Zhiyu");
pollyVoiceMap.set("es-US", "Lupe");

const pollyNeuralVoices = [
    "Vicki",
    "Gabrielle",
    "Lupe",
    "Olivia",
    "Amy",
    "Emma",
    "Brian",
    "Aria",
    "Ayanda",
    "Ivy",
    "Joanna",
    "Kendra",
    "Kimberly",
    "Salli",
    "Joey",
    "Justin",
    "Kevin",
    "Matthew",
    "Gabrielle",
    "Vicki",
    "Bianca",
    "Seoyeon",
    "Camila",
    "Lucia",
    "Lupe",
    "Lea",
    "Takumi",
    "Kajal",
    "Zhiyu",
];

const webcamElement = document.getElementById("webcam");
const canvasElement = document.getElementById("canvas");
const snapSoundElement = document.getElementById("snapSound");
const queryString = window.location.search;

if (queryString != "") {
    var webcam = new Webcam(
        webcamElement,
        "user",
        canvasElement,
        snapSoundElement
    );
    $(".md-modal").addClass("md-show");
    webcam
        .start()
        .then((result) => {
            cameraStarted();
        })
        .catch((err) => {
            displayError();
        });
}

//  Check to see if the browser allows mic access
if (!window.navigator.mediaDevices.getUserMedia) {
    showError(
        "We support the latest versions of Chrome, Firefox, Safari, and Edge. Update your browser and try your request again."
    );
}

/**
 * Click and Change events handling
 */

$("#start-button").click(function () {
    $("#error").hide(); // hide any existing errors
    $("#transcript").val("");
    transcription = "";
    toggleStartStop(); // disable start and enable stop button

    setLanguage(); // set the language and region from the dropdowns
    window.navigator.mediaDevices
        .getUserMedia({
            // first we get the microphone input from the browser (as a promise)
            video: false,
            audio: true,
        })
        .then(streamAudioToWebSocket) // we convert the mic stream to binary event stream messages when the promise resolves
        .catch(function (error) {
            showError(
                "There was an error streaming your audio to Amazon Transcribe. Please try again."
            );
            console.log(error);
            toggleStartStop();
        });
    document.getElementById("listening-animation").style.display = "flex";
});

$("#start-conversation-button").click(function () {
    $("#error").hide(); // hide any existing errors
    document.getElementById("audioSource").src =
        "data:audio/mpeg;base64,SUQzBAAAAAABEVRYWFgAAAAtAAADY29tbWVudABCaWdTb3VuZEJhbmsuY29tIC8gTGFTb25vdGhlcXVlLm9yZwBURU5DAAAAHQAAA1N3aXRjaCBQbHVzIMKpIE5DSCBTb2Z0d2FyZQBUSVQyAAAABgAAAzIyMzUAVFNTRQAAAA8AAANMYXZmNTcuODMuMTAwAAAAAAAAAAAAAAD/80DEAAAAA0gAAAAATEFNRTMuMTAwVVVVVVVVVVVVVUxBTUUzLjEwMFVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVf/zQsRbAAADSAAAAABVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVf/zQMSkAAADSAAAAABVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
    document.getElementById("audioPlayback").load();
    toggleStartStopConversation(); // disable start and enable stop button
    setLanguage(); // set the language and region from the dropdowns
    window.navigator.mediaDevices
        .getUserMedia({
            // first we get the microphone input from the browser (as a promise)
            video: false,
            audio: true,
        })
        .then(streamAudioToWebSocket) // we convert the mic stream to binary event stream messages when the promise resolves
        .catch(function (error) {
            showError(
                "There was an error streaming your audio to Amazon Transcribe. Please try again."
            );
            toggleStartStopConversation();
        });
    document.getElementById("listening-animation").style.display = "flex";
});

$("#stop-button").click(function () {
    closeSocket();
    callTranscriptAPI();
    toggleStartStop();
    document.getElementById("listening-animation").style.display = "none";
});

$("#reset-button").click(function () {
    // In case user is recording, abort and clear
    
    closeSocket();
    document.getElementById("start-button").style.display = "block";
    document.getElementById("stop-button").style.display = "none";
    $("#transcript").val("");
    transcription = "";
    $("#translate").val("");
    translation = "";
});

$("#stop-all-button").click(function () {
    callStopApi();
});
$("#send-button").click(function () {
    callTranscriptAPI();
});
document.getElementById('avatar-taro').addEventListener('click', function () {
    callApiOnClick("Taro")
});
document.getElementById('avatar-hana').addEventListener('click', function () {
    callApiOnClick("Hana")
});
document.getElementById('avatar-ada').addEventListener('click', function () {
    callApiOnClick("Ada")
});
document.getElementById('avatar-bernice').addEventListener('click', function () {
    callApiOnClick("Bernice")
});

$("#reset-button-2").click(function () {
    $("#translate2").val("");
    translation2 = "";
});

$("#play-button").click(function () {
    speakText();
});

$("#translate-play-button").click(function () {
    translateSpeakText();
});

$("#switch-language-button").click(function () {
    if (
        document.getElementById("stop-conversation-button").style.display ===
        "block"
    )
        // if listening
        document.getElementById("stop-conversation-button").click();
    let language0 = document.getElementById("language").selectedIndex;
    let translateTo0 = document.getElementById("translateTo").selectedIndex;
    document.getElementById("language").selectedIndex = translateTo0;
    document.getElementById("translateTo").selectedIndex = language0;
    disableLanguage();
    formatTranslate();
    const myTimeout = setTimeout(function () {
        resetListenTextAreas();
    }, 1000);
});

$("#language").change(function () {
    if (
        typeof document.getElementById("stop-button") != "undefined" &&
        document.getElementById("stop-button") != null
    )
        if (document.getElementById("stop-button").style.display === "block") {
            // if feature 1 or 2
            // if listening
            document.getElementById("stop-button").click(); // stop listening
        }

    if (
        typeof document.getElementById("stop-conversation-button") != "undefined" &&
        document.getElementById("stop-conversation-button") != null
    )
        if (
            document.getElementById("stop-conversation-button").style.display ===
            "block"
        ) {
            // if feature 3
            // if listening
            document.getElementById("stop-conversation-button").click(); // stop listening
        }

    const myTimeout = setTimeout(function () {
        resetListenTextAreas();
        resetSpeakTextArea();
    }, 1000);

    disableLanguage();

    if (document.getElementById("textToSynth")) {
        if ($("#language").find(":selected").val() == "auto") {
            document.getElementById("textToSynth").style.display = "none";
            document.getElementById("textToSynth-not-ready").style.display = "block";
        } else {
            document.getElementById("textToSynth").style.display = "block";
            document.getElementById("textToSynth-not-ready").style.display = "none";
        }
    }
});

$("#translateTo").change(function () {
    if (
        typeof document.getElementById("stop-button") != "undefined" &&
        document.getElementById("stop-button") != null
    )
        if (document.getElementById("stop-button").style.display === "block") {
            // if feature 1 or 2
            // if listening
            document.getElementById("stop-button").click(); // stop listening
        }

    if (
        typeof document.getElementById("stop-conversation-button") != "undefined" &&
        document.getElementById("stop-conversation-button") != null
    )
        if (
            document.getElementById("stop-conversation-button").style.display ===
            "block"
        ) {
            // if feature 3
            // if listening
            document.getElementById("stop-conversation-button").click(); // stop listening
        }

    const myTimeout = setTimeout(function () {
        resetListenTextAreas();
        resetSpeakTextArea();
    }, 1000);

    formatTranslate();
});

$("#stop-conversation-button").click(function () {
    closeSocket();
    toggleStartStopConversation();
    document.getElementById("listening-animation").style.display = "none";
    setTimeout(() => {
        conversationSpeak();
    }, 1000);
});

$("#hide-container").click(function () {
    if (
        typeof document.getElementById("text-container") != "undefined" &&
        document.getElementById("text-container") != null
    )
        document.getElementById("text-container").style.display = "none";
});

$("#show-text").click(function () {
    document.getElementById("text-container").style.display = "block";
});

$("#translateTextTo").change(function () {
    var urlParams = new URLSearchParams(queryString);
    var translate = new AWS.Translate();
    var target_language = $("#translateTextTo")
        .find(":selected")
        .val()
        .substring(0, 2);
    var params = {
        SourceLanguageCode: urlParams.get("language").substring(0, 2),
        TargetLanguageCode: target_language,
        Text: detectedText,
    };
    translate.translateText(params, function (err, data) {
        if (err) console.log(err, err.stack);
        else {
            document.getElementById("detectedText").value = data["TranslatedText"];
            if (target_language == "he" || target_language == "ar") {
                $("#detectedText").css("direction", "rtl");
            } else {
                $("#detectedText").css("direction", "ltr");
            }
        }
    });
});

$("#dyslexic-button").click(function () {
    if (
        document.getElementById("detectedText").style.fontFamily !=
        "OpenDyslexicRegular"
    )
        document.getElementById("detectedText").style.fontFamily =
            "OpenDyslexicRegular";
    else
        document.getElementById("detectedText").style.fontFamily =
            window.getComputedStyle(
                document.createElement("detectedText")
            ).fontFamily;
});

$("#btn-increase").click(function () {
    document.getElementById("detectedText").style.fontSize = "larger";
});

$("#btn-decrease").click(function () {
    document.getElementById("detectedText").style.fontSize = "smaller";
});

$("#btn-orig").click(function () {
    document.getElementById("detectedText").style.fontSize = "initial";
});

/**
 * Some more initialization tasks
 */

let streamAudioToWebSocket = function (userMediaStream) {
    micStream = new mic(); //let's get the mic input from the browser, via the microphone-stream module

    micStream.on("format", function (data) {
        inputSampleRate = data.sampleRate;
    });

    micStream.setStream(userMediaStream);

    let url = createPresignedUrl(); // Pre-signed URLs are a way to authenticate a request (or WebSocket connection, in this case)
    socket = new WebSocket(url); // open up our WebSocket connection
    socket.binaryType = "arraybuffer";
    let sampleRate = 0;
    socket.onopen = function () {
        // when we get audio data from the mic, send it to the WebSocket if possible
        micStream.on("data", function (rawAudioChunk) {
            let binary = convertAudioToBinaryMessage(rawAudioChunk); // the audio stream is raw audio bytes. Transcribe expects PCM with additional metadata, encoded as binary
            if (socket.readyState === socket.OPEN) socket.send(binary);
        });
    };
    wireSocketEvents(); // handle messages, errors, and close events
};

function setLanguage() {
    languageCode = $("#language").find(":selected").val();
    sampleRate = 44100;
}

function disableLanguage() {
    var currentLanguage;
    currentLanguage = $("#language").find(":selected").val().substring(0, 2);
    $("#translateTo option").each(function (index, element) {
        if (currentLanguage == element.value) {
            $(this).prop("disabled", true);
        } else {
            $(this).prop("disabled", false);
        }
    });
}

function formatTranslate() {
    var currentLanguage = $("#translateTo").find(":selected").val();
    if (currentLanguage == "he" || currentLanguage == "ar") {
        $("#translate").css("direction", "rtl");
    } else {
        $("#translate").css("direction", "ltr");
    }
}

function resetListenTextAreas() {
    $("#transcript").val("");
    transcription = "";
    $("#translate").val("");
    translation = "";
}

function resetSpeakTextArea() {
    $("#translate2").val("");
    translation2 = "";
}

/**
 * Functions for "Live transcription and text-to-speeech", "Live transcription and text-to-speeech with translation" features
 */

function speakText() {
    setLanguage();

    if (typeof pollyVoiceMap.get(languageCode) === "undefined") {
        document.getElementById("result").textContent =
            "The language you selected is not available for text-to-speech";
        return;
    }

    var voiceId = pollyVoiceMap.get(languageCode);

    // Create the JSON parameters for getSynthesizeSpeechUrl
    var speechParams = {
        OutputFormat: "mp3",
        SampleRate: "16000",
        Text: "",
        TextType: "text",
        VoiceId: voiceId,
        Engine: "standard",
    };

    if (pollyNeuralVoices.includes(voiceId)) speechParams.Engine = "neural";

    speechParams.Text = document.getElementById("textEntry").value;

    // Create the Polly service object and presigner object
    var polly = new AWS.Polly({apiVersion: "2016-06-10"});
    var signer = new AWS.Polly.Presigner(speechParams, polly);

    // Create presigned URL of synthesized speech file
    signer.getSynthesizeSpeechUrl(speechParams, function (error, url) {
        if (error) {
            document.getElementById("result").textContent =
                "Oops! Something went wrong.";
        } else {
            document.getElementById("audioSource").src = url;
            if (document.getElementById("audioPlayback").style.display === "none")
                document.getElementById("audioPlayback").style.display = "block";
            document.getElementById("audioPlayback").load();
        }
    });
}

function translateSpeakText() {
    $("#translate2").val("");
    setLanguage();

    if (typeof pollyVoiceMap.get(languageCode) === "undefined") {
        document.getElementById("result").textContent =
            "The language you selected is not available for text-to-speech";
        return;
    }

    var voiceId = pollyVoiceMap.get(languageCode);

    // Create the JSON parameters for getSynthesizeSpeechUrl
    var speechParams = {
        OutputFormat: "mp3",
        SampleRate: "16000",
        Text: "",
        TextType: "text",
        VoiceId: voiceId,
        Engine: "standard",
    };

    if (pollyNeuralVoices.includes(voiceId)) speechParams.Engine = "neural";

    translation2 = "";

    translateInputReverse(
        document.getElementById("textEntry").value,
        function (translated) {
            translation2 += translated + "\n";
            $("#translate2").val(translation2 + "\n");
            $("#translate2").scrollTop($("#translate2")[0].scrollHeight);
            speechParams.Text = translation2;

            // Create the Polly service object and presigner object
            var polly = new AWS.Polly({apiVersion: "2016-06-10"});
            var signer = new AWS.Polly.Presigner(speechParams, polly);

            // Create presigned URL of synthesized speech file
            signer.getSynthesizeSpeechUrl(speechParams, function (error, url) {
                if (error) {
                    document.getElementById("result").textContent =
                        "Oops! Something went wrong.";
                } else {
                    document.getElementById("audioSource").src = url;
                    if (document.getElementById("audioPlayback").style.display === "none")
                        document.getElementById("audioPlayback").style.display = "block";
                    document.getElementById("audioPlayback").load();
                }
            });
        }
    );
}

/**
 * Functions for "Real-time conversation translation" feature
 */

function conversationSpeak() {
    if (
        typeof pollyVoiceMap.get($("#translateTo").find(":selected").val()) ===
        "undefined"
    ) {
        document.getElementById("result").textContent =
            "The language you selected is not available for text-to-speech";
        return;
    }

    var voiceId = pollyVoiceMap.get($("#translateTo").find(":selected").val());

    // Create the JSON parameters for getSynthesizeSpeechUrl
    var speechParams = {
        OutputFormat: "mp3",
        SampleRate: "16000",
        Text: translation,
        TextType: "text",
        VoiceId: voiceId,
        Engine: "standard",
    };

    if (pollyNeuralVoices.includes(voiceId)) speechParams.Engine = "neural";

    // Create the Polly service object and presigner object
    var polly = new AWS.Polly({apiVersion: "2016-06-10"});
    var signer = new AWS.Polly.Presigner(speechParams, polly);

    // Create presigned URL of synthesized speech file
    signer.getSynthesizeSpeechUrl(speechParams, function (error, url) {
        if (error) {
            document.getElementById("result").textContent =
                "Oops! Something went wrong.";
        } else {
            document.getElementById("audioSource").src = url;
            if (document.getElementById("audioPlayback").style.display === "none")
                document.getElementById("audioPlayback").style.display = "block";
            document.getElementById("audioPlayback").load();
        }
    });
}

function wireSocketEvents() {
    // handle inbound messages from Amazon Transcribe
    socket.onmessage = function (message) {
        //convert the binary event stream message to JSON
        let messageWrapper = eventStreamMarshaller.unmarshall(Buffer(message.data));
        let messageBody = JSON.parse(
            String.fromCharCode.apply(String, messageWrapper.body)
        );
        if (messageWrapper.headers[":message-type"].value === "event") {
            handleEventStreamMessage(messageBody);
        } else {
            transcribeException = true;
            showError(messageBody.Message);
            toggleStartStop();
        }
    };

    socket.onerror = function () {
        socketError = true;
        showError("WebSocket connection error. Try again.");
        toggleStartStop();
    };

    socket.onclose = function (closeEvent) {
        micStream.stop();

        // the close event immediately follows the error event; only handle one.
        if (!socketError && !transcribeException) {
            if (closeEvent.code != 1000) {
                showError(
                    "</i><strong>Streaming Exception</strong><br>" + closeEvent.reason
                );
                toggleStartStop();
            }
        }
    };
}

let handleEventStreamMessage = function (messageJson) {
    let results = messageJson.Transcript.Results;
    if (results.length > 0) {
        if (results[0].Alternatives.length > 0) {
            let languageCodeFromStream = "";
            if (
                typeof results[0].LanguageCode != "undefined" &&
                languageCodeFromStream != results[0].LanguageCode
            ) {
                languageCodeFromStream = results[0].LanguageCode;
                let languageSelectionElement = document.getElementById("language");
                languageSelectionElement.value = languageCodeFromStream;
                document.getElementById("textToSynth").style.display = "block";
                document.getElementById("textToSynth-not-ready").style.display = "none";
            }
            let transcript = results[0].Alternatives[0].Transcript;

            // fix encoding for accented characters
            transcript = decodeURIComponent(escape(transcript));

            // update the textarea with the latest result
            $("#transcript").val(transcription + transcript + "\n");
            $("#transcript").scrollTop($("#transcript")[0].scrollHeight);

            // if this transcript segment is final, add it to the overall transcription
            if (!results[0].IsPartial) {
                $("#transcript").scrollTop($("#transcript")[0].scrollHeight);
                transcription += transcript + "\n";
            }
        }
    }
};

let closeSocket = function () {
    if (typeof socket !== 'undefined' && socket.readyState === socket.OPEN) {
        micStream.stop();
        // Send an empty frame so that Transcribe initiates a closure of the WebSocket after submitting all transcripts
        let emptyMessage = getAudioEventMessage(Buffer.from(new Buffer([])));
        let emptyBuffer = eventStreamMarshaller.marshall(emptyMessage);
        socket.send(emptyBuffer);
    }
};

function translateInput(text, callback) {
    var source_language = "auto";
    var target_language = $("#translateTo")
        .find(":selected")
        .val()
        .substring(0, 2);

    var urlParams = new URLSearchParams(window.location.search);
    if (urlParams.has("language")) {
        var target_language = urlParams.get("language");
    }

    var translate = new AWS.Translate();
    var params = {
        SourceLanguageCode: source_language,
        TargetLanguageCode: target_language,
        Text: text,
    };
    translate.translateText(params, function (err, data) {
        callback(data.TranslatedText);
    });
}

function translateInputReverse(text, callback) {
    var source_language = $("#translateTo")
        .find(":selected")
        .val()
        .substring(0, 2);
    var target_language = $("#language").find(":selected").val().substring(0, 2);

    var translate = new AWS.Translate();
    var params = {
        SourceLanguageCode: source_language,
        TargetLanguageCode: target_language,
        Text: text,
    };
    translate.translateText(params, function (err, data) {
        callback(data.TranslatedText);
    });
}

function toggleStartStop() {
    if (document.getElementById("start-button").style.display === "none") {
        document.getElementById("start-button").style.display = "block";
    } else {
        document.getElementById("start-button").style.display = "none";
    }

    if (document.getElementById("stop-button").style.display === "none") {
        document.getElementById("stop-button").style.display = "block";
    } else {
        document.getElementById("stop-button").style.display = "none";
    }
}

function toggleStartStopConversation(disableStart = false) {
    if (
        document.getElementById("start-conversation-button").style.display ===
        "none"
    ) {
        document.getElementById("start-conversation-button").style.display =
            "block";
    } else {
        document.getElementById("start-conversation-button").style.display = "none";
    }

    if (
        document.getElementById("stop-conversation-button").style.display === "none"
    ) {
        document.getElementById("stop-conversation-button").style.display = "block";
    } else {
        document.getElementById("stop-conversation-button").style.display = "none";
    }
}

function showError(message) {
    $("#error").text(escapeHtml('<i class="fa fa-times-circle"></i> ' + message));
    $("#error").show();
}

function convertAudioToBinaryMessage(audioChunk) {
    let raw = mic.toRaw(audioChunk);
    if (raw == null) return;

    // downsample and convert the raw audio bytes to PCM
    let downsampledBuffer = audioUtils.downsampleBuffer(
        raw,
        inputSampleRate,
        sampleRate
    );
    let pcmEncodedBuffer = audioUtils.pcmEncode(downsampledBuffer);

    // add the right JSON headers and structure to the message
    let audioEventMessage = getAudioEventMessage(Buffer.from(pcmEncodedBuffer));

    //convert the JSON object + headers into a binary event stream message
    let binary = eventStreamMarshaller.marshall(audioEventMessage);
    return binary;
}

function getAudioEventMessage(buffer) {
    // wrap the audio data in a JSON envelope
    return {
        headers: {
            ":message-type": {
                type: "string",
                value: "event",
            },
            ":event-type": {
                type: "string",
                value: "AudioEvent",
            },
        },
        body: buffer,
    };
}

function createPresignedUrl() {
    let endpoint = "transcribestreaming." + region + ".amazonaws.com:8443";

    if (languageCode == "auto") {
        // get a preauthenticated URL that we can use to establish our WebSocket
        return v4.createPresignedURL(
            "GET",
            endpoint,
            "/stream-transcription-websocket",
            "transcribe",
            crypto.createHash("sha256").update("", "utf8").digest("hex"),
            {
                key: AWS.config.credentials.accessKeyId,
                secret: AWS.config.credentials.secretAccessKey,
                sessionToken: AWS.config.credentials.sessionToken,
                protocol: "wss",
                expires: 15,
                region: region,
                query:
                    "&identify-language=true&language-options=zh-CN,en-US,fr-FR,de-DE,it-IT,ja-JP,ko-KR,pt-BR,es-US" +
                    "&media-encoding=pcm&sample-rate=" +
                    sampleRate,
            }
        );
    } else {
        // get a preauthenticated URL that we can use to establish our WebSocket
        return v4.createPresignedURL(
            "GET",
            endpoint,
            "/stream-transcription-websocket",
            "transcribe",
            crypto.createHash("sha256").update("", "utf8").digest("hex"),
            {
                key: AWS.config.credentials.accessKeyId,
                secret: AWS.config.credentials.secretAccessKey,
                sessionToken: AWS.config.credentials.sessionToken,
                protocol: "wss",
                expires: 15,
                region: region,
                query:
                    "language-code=" +
                    languageCode +
                    "&media-encoding=pcm&sample-rate=" +
                    sampleRate,
            }
        );
    }
}

/**
 * Click events handling and functions for "Help me see objects", "Help me read labels & signs" and "Help me read documents" features
 */

$("#cameraFlip").click(function () {
    webcam.flip();
    webcam.start();
});

$("#closeError").click(function () {
    $("#webcam-switch").prop("checked", false).change();
});

$("#take-photo").click(function () {
    let picture = webcam.snap();
    document.getElementById("cameraFlip").style.display = "none";
    if (document.getElementById("translateTextTo"))
        document.getElementById("translateTextTo").selectedIndex = 0;
    ProcessImage(picture);
    document.getElementById("audioSource").src =
        "data:audio/mpeg;base64,SUQzBAAAAAABEVRYWFgAAAAtAAADY29tbWVudABCaWdTb3VuZEJhbmsuY29tIC8gTGFTb25vdGhlcXVlLm9yZwBURU5DAAAAHQAAA1N3aXRjaCBQbHVzIMKpIE5DSCBTb2Z0d2FyZQBUSVQyAAAABgAAAzIyMzUAVFNTRQAAAA8AAANMYXZmNTcuODMuMTAwAAAAAAAAAAAAAAD/80DEAAAAA0gAAAAATEFNRTMuMTAwVVVVVVVVVVVVVUxBTUUzLjEwMFVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVf/zQsRbAAADSAAAAABVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVf/zQMSkAAADSAAAAABVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
    document.getElementById("audioPlayback").load();
});

function displayError(err = "") {
    if (err != "") {
        $("#errorMsg").text(err);
    }
    $("#errorMsg").removeClass("d-none");
}

function cameraStarted() {
    webcam.flip();
    webcam.start();
    $("#errorMsg").addClass("d-none");
    $(".flash").hide();
    $("#webcam-caption").text("on");
    $("#webcam-control").removeClass("webcam-off");
    $("#webcam-control").addClass("webcam-on");
    $(".webcam-container").removeClass("d-none");
    if (webcam.webcamList.length == 1) {
        $("#cameraFlip").removeClass("d-none");
    }
    $("#wpfront-scroll-top-container").addClass("d-none");
    window.scrollTo(0, 0);
    $("body").css("overflow-y", "hidden");
}

//Loads selected image and unencodes image bytes for Rekognition/Textract API
function ProcessImage(picture) {
    if (window.location.href.indexOf("f-see-text") != -1) {
        var blob;

        // Load base64 encoded image for display
        var reader = new FileReader();

        reader.onload = (function (theFile) {
            return function (e) {
                //Call Rekognition
                AWS.region = region;
                var rekognition = new AWS.Rekognition();
                var params = {
                    Image: {
                        Bytes: e.target.result,
                    },
                };

                rekognition.detectText(params, function (err, data) {
                    if (err) console.log(err, err.stack);
                    // an error occurred
                    else {
                        var string = "";
                        var stringHTML = "";
                        for (var i = 0; i < data.TextDetections.length; i++) {
                            if (data.TextDetections[i].Type == "LINE") {
                                string += data.TextDetections[i].DetectedText + ", ";
                                stringHTML += data.TextDetections[i].DetectedText + "\r\n";
                            }
                        }

                        if (string != "") {
                            string = string.substring(0, string.length - 1);
                            stringHTML = stringHTML.substring(0, stringHTML.length - 1);
                            var urlParams = new URLSearchParams(queryString);
                            var languageFromURL = urlParams.get("language");
                            document.getElementById("show-text").style.display = "block";
                            document.getElementById("detectedText").style.display = "block";
                            document.getElementById("detectedTextAlert").style.display =
                                "none";
                            document.getElementById("detectedText").value = stringHTML;
                            detectedText = stringHTML;
                            document.getElementById("translate-dyslexic-div").style.display =
                                "block";
                            if (languageFromURL == "ar" || languageFromURL == "arb") {
                                $("#detectedText").css("direction", "rtl");
                            } else {
                                $("#detectedText").css("direction", "ltr");
                            }
                            speakDetectedTextFromWebcam(string, languageFromURL);
                        } else {
                            document.getElementById("show-text").style.display = "block";
                            document.getElementById("detectedTextAlert").style.display =
                                "block";
                            document.getElementById("translate-dyslexic-div").style.display =
                                "none";
                            document.getElementById("detectedText").style.display = "none";
                            document.getElementById("detectedTextAlert").textContent =
                                "No text detected. Please try again!";
                            speakDetectedTextFromWebcam(
                                "No text detected. Please try again!",
                                "en-US"
                            );
                        }
                    }
                });
            };
        })(blob);

        fetch(picture)
            .then((resp) => resp.blob())
            .then((blob) => reader.readAsArrayBuffer(blob));
    } else if (window.location.href.indexOf("f-see-document") != -1) {
        var blob;

        // Load base64 encoded image for display
        var reader = new FileReader();

        reader.onload = (function (theFile) {
            return function (e) {
                //Call Textract
                var textract = new AWS.Textract();
                var params = {
                    Document: {
                        /* required */ Bytes: e.target.result,
                    },
                };

                textract.detectDocumentText(params, function (err, data) {
                    if (err) console.log(err, err.stack);
                    // an error occurred
                    else {
                        var string = "";
                        var stringHTML = "";
                        for (var i = 0; i < data.Blocks.length; i++) {
                            if (data.Blocks[i].BlockType == "LINE") {
                                string += data.Blocks[i].Text + ", ";
                                stringHTML += data.Blocks[i].Text + "\r\n";
                            }
                        }

                        if (string != "") {
                            string = string.substring(0, string.length - 1);
                            stringHTML = stringHTML.substring(0, stringHTML.length - 1);
                            var urlParams = new URLSearchParams(queryString);
                            var languageFromURL = urlParams.get("language");
                            document.getElementById("show-text").style.display = "block";
                            document.getElementById("detectedText").style.display = "block";
                            document.getElementById("detectedTextAlert").style.display =
                                "none";
                            document.getElementById("detectedText").value = stringHTML;
                            detectedText = stringHTML;
                            document.getElementById("translate-dyslexic-div").style.display =
                                "block";
                            if (languageFromURL == "ar" || languageFromURL == "arb") {
                                $("#detectedText").css("direction", "rtl");
                            } else {
                                $("#detectedText").css("direction", "ltr");
                            }
                            speakDetectedTextFromWebcam(string, languageFromURL);
                        } else {
                            document.getElementById("show-text").style.display = "block";
                            document.getElementById("detectedTextAlert").style.display =
                                "block";
                            document.getElementById("translate-dyslexic-div").style.display =
                                "none";
                            document.getElementById("detectedText").style.display = "none";
                            document.getElementById("detectedTextAlert").textContent =
                                "No text detected. Please try again!";
                            speakDetectedTextFromWebcam(
                                "No text detected. Please try again!",
                                "en-US"
                            );
                        }
                    }
                });
            };
        })(blob);

        fetch(picture)
            .then((resp) => resp.blob())
            .then((blob) => reader.readAsArrayBuffer(blob));
    } else if (window.location.href.indexOf("f-see-objects") != -1) {
        var blob;
        var labels = [];
        var parents = [];

        // Load base64 encoded image for display
        var reader = new FileReader();

        reader.onload = (function (theFile) {
            return function (e) {
                //Call Rekognition
                AWS.region = region;
                var rekognition = new AWS.Rekognition();
                var params = {
                    Image: {
                        Bytes: e.target.result,
                    },
                    MaxLabels: 10,
                    MinConfidence: 80,
                };

                rekognition.detectLabels(params, function (err, response) {
                    if (err) console.log(err, err.stack);
                    // an error occurred
                    else {
                        response.Labels.forEach((label) => {
                            labels.push(label.Name);
                            if (
                                typeof label.Parents !== "undefined" &&
                                label.Parents.length > 0
                            )
                                label.Parents.forEach((parent) => {
                                    parents.push(parent.Name);
                                });
                        });
                        labels = labels.filter((el) => !parents.includes(el));
                        if (labels.length > 0) {
                            var string = "I see ";
                            labels.forEach((item) => {
                                string += item.toLowerCase() + ", ";
                            });
                            string = string.substring(0, string.length - 2);

                            var sanitizedString = document.createElement("div");

                            var urlParams = new URLSearchParams(queryString);
                            var languageFromURL = urlParams.get("language");
                            document.getElementById("show-text").style.display = "block";

                            if (languageFromURL != "en") {
                                var translate = new AWS.Translate();
                                var params = {
                                    SourceLanguageCode: "en",
                                    TargetLanguageCode: languageFromURL.substring(0, 2),
                                    Text: string,
                                };
                                translate.translateText(params, function (err, data) {
                                    if (err) console.log(err, err.stack);
                                    else {
                                        string = data["TranslatedText"];
                                        sanitizedString.textContent = string;
                                        document.getElementById("detectedText").textContent =
                                            sanitizedString;
                                        if (languageFromURL == "ar" || languageFromURL == "arb") {
                                            $("#detectedText").css("direction", "rtl");
                                        } else {
                                            $("#detectedText").css("direction", "ltr");
                                        }
                                        speakDetectedTextFromWebcam(string, languageFromURL);
                                    }
                                });
                            } else {
                                sanitizedString.textContent = string;
                                document.getElementById("detectedText").textContent =
                                    sanitizedString;
                                $("#detectedText").css("direction", "ltr");
                                speakDetectedTextFromWebcam(string, languageFromURL);
                            }
                        } else {
                            console.log("No objects detected. Please try again!");
                            document.getElementById("show-text").style.display = "block";
                            document.getElementById("detectedText").textContent =
                                "No objects detected. Please try again!";
                            speakDetectedTextFromWebcam(
                                "No objects detected. Please try again!",
                                "en-US"
                            );
                        }
                    }
                });
            };
        })(blob);

        fetch(picture)
            .then((resp) => resp.blob())
            .then((blob) => reader.readAsArrayBuffer(blob));
    }
}

function speakDetectedTextFromWebcam(detectedText, language) {
    setLanguage();
    var voiceId = pollyVoiceMap.get(language);

    // Create the JSON parameters for getSynthesizeSpeechUrl
    var speechParams = {
        OutputFormat: "mp3",
        SampleRate: "16000",
        Text: "",
        TextType: "text",
        VoiceId: voiceId,
        Engine: "standard",
    };

    if (pollyNeuralVoices.includes(voiceId)) speechParams.Engine = "neural";

    speechParams.Text = detectedText;

    // Create the Polly service object and presigner object
    var polly = new AWS.Polly({apiVersion: "2016-06-10"});
    var signer = new AWS.Polly.Presigner(speechParams, polly);

    // Create presigned URL of synthesized speech file
    signer.getSynthesizeSpeechUrl(speechParams, function (error, url) {
        if (error) {
        } else {
            document.getElementById("audioSource").src = url;
            document.getElementById("audioPlayback").load();
        }
    });
}
