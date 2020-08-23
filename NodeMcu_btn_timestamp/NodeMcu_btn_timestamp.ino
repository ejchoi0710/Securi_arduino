
/*
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2020 mobizt
 *
*/

//This example shows how to set and push timestamp (server time) which is the server variable that suopported by Firebase

//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "https://securi-f4c0d.firebaseio.com/"
#define FIREBASE_AUTH "aL53gG5Y0esOemRmox1ED7MRY8kYRk0fmuC7uKbB"
#define WIFI_SSID "SK_WiFiGIGA2A98"
#define WIFI_PASSWORD "1810000573"

//Define Firebase Data object
FirebaseData firebaseData;
// set pin numbers
const int buttonPin = 4;     // the number of the pushbutton pin

// variable for storing the pushbutton status
int buttonState = 0;
void setup()
{
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
}

void loop()
{
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH
  if (buttonState == HIGH) {
    // turn LED on
    digitalWrite(LED_BUILTIN, LOW);
    String path = "/Test";

  Serial.println("------------------------------------");
  Serial.println("Set Timestamp test...");

  if (Firebase.setTimestamp(firebaseData, path + "/Set/Timestamp"))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());

    //Timestamp saved in millisecond, get its seconds from intData()
    Serial.print("TIMESTAMP (Seconds): ");
    Serial.println(firebaseData.intData());

    //Or print the total milliseconds from doubleData()
    //Due to bugs in Serial.print in Arduino library, use printf to print double instead.
    printf("TIMESTAMP (milliSeconds): %.0lf\n", firebaseData.doubleData());

    //Or print it from payload directly
    Serial.print("TIMESTAMP (milliSeconds): ");
    Serial.println(firebaseData.payload());

    //Due to some internal server error, ETag cannot get from setTimestamp
    //Try to get ETag manually
    Serial.println("ETag: " + Firebase.getETag(firebaseData, path + "/Set/Timestamp"));
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  Serial.println("------------------------------------");
  Serial.println("Get Timestamp (double of milliseconds) test...");

  if (Firebase.getDouble(firebaseData, path + "/Set/Timestamp"))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());

    printf("TIMESTAMP: %.0lf\n", firebaseData.doubleData());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  Serial.println("------------------------------------");
  Serial.println("Push Timestamp test...");

  if (Firebase.pushTimestamp(firebaseData, path + "/Push/Timestamp"))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());

    //Due to some internal server error, ETag cannot get from pushTimestamp
    //Try to get ETag manually
    Serial.println("ETag: " + Firebase.getETag(firebaseData, path + "/Push/Timestamp/" + firebaseData.pushName()));
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  } else {
    // turn LED off
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
