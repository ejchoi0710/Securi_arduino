
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
#include<SoftwareSerial.h> // defining the software serial library
SoftwareSerial mySerial (D7,D6); // creating the object of the library as mySerial and defining the D7 pin as RX pin and D6 pin as Tx pin

#define FIREBASE_HOST "https://securi-f4c0d.firebaseio.com/"
#define FIREBASE_AUTH "aL53gG5Y0esOemRmox1ED7MRY8kYRk0fmuC7uKbB"
//#define WIFI_SSID "SK_WiFiGIGA2A98"
//#define WIFI_PASSWORD "1810000573"
#define WIFI_SSID "AndroidHotspot0871"
#define WIFI_PASSWORD "12345678"
const int buttonPin = 4;     // the number of the pushbutton pin
const int sensor = 5;
int state;
int buttonState = 0;
FirebaseData firebaseData;

String streampath = "/door";
void printResult(FirebaseData &data);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(sensor,INPUT_PULLUP);
  mySerial.begin(115200); // starting the software serial
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
  if (!Firebase.beginStream(firebaseData, streampath))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  
}

void loop()
{
  if (!Firebase.readStream(firebaseData))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (firebaseData.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }
  if (firebaseData.streamAvailable())
  {
    Serial.println("------------------------------------");
    Serial.println("Stream Data available...");
    Serial.println("STREAM PATH: " + firebaseData.streamPath());
    Serial.println("EVENT PATH: " + firebaseData.dataPath());
    Serial.println("DATA TYPE: " + firebaseData.dataType());
    Serial.println("EVENT TYPE: " + firebaseData.eventType());
    Serial.print("VALUE: ");
    printResult(firebaseData);
    Serial.println("------------------------------------");
    Serial.println();
  }
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
      Serial.println("button pressed");
      mySerial.println ('a'); //”DATA 1” “DATA2” will be printed on software serial monitor.
      delay(3000);
    }
//  state = digitalRead(sensor);
//  if(state == HIGH){
//    String path = "/open"; 
//    Serial.println("Door open");
//      Serial.println("------------------------------------");
//      Serial.println("Set Timestamp test...");
//    
//      if (Firebase.setTimestamp(firebaseData, path + "/Set/Timestamp"))
//      {
//        Serial.println("PASSED");
//        Serial.println("PATH: " + firebaseData.dataPath());
//        Serial.println("TYPE: " + firebaseData.dataType());
//    
//        //Timestamp saved in millisecond, get its seconds from intData()
//        Serial.print("TIMESTAMP (Seconds): ");
//        Serial.println(firebaseData.intData());
//    
//        //Or print the total milliseconds from doubleData()
//        //Due to bugs in Serial.print in Arduino library, use printf to print double instead.
//        printf("TIMESTAMP (milliSeconds): %.0lf\n", firebaseData.doubleData());
//    
//        //Or print it from payload directly
//        Serial.print("TIMESTAMP (milliSeconds): ");
//        Serial.println(firebaseData.payload());
//    
//        //Due to some internal server error, ETag cannot get from setTimestamp
//        //Try to get ETag manually
//        Serial.println("ETag: " + Firebase.getETag(firebaseData, path + "/Set/Timestamp"));
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//      else
//      {
//        Serial.println("FAILED");
//        Serial.println("REASON: " + firebaseData.errorReason());
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//    
//      Serial.println("------------------------------------");
//      Serial.println("Get Timestamp (double of milliseconds) test...");
//    
//      if (Firebase.getDouble(firebaseData, path + "/Set/Timestamp"))
//      {
//        Serial.println("PASSED");
//        Serial.println("PATH: " + firebaseData.dataPath());
//        Serial.println("TYPE: " + firebaseData.dataType());
//    
//        printf("TIMESTAMP: %.0lf\n", firebaseData.doubleData());
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//      else
//      {
//        Serial.println("FAILED");
//        Serial.println("REASON: " + firebaseData.errorReason());
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//    
//      Serial.println("------------------------------------");
//      Serial.println("Push Timestamp test...");
//    
//      if (Firebase.pushTimestamp(firebaseData, path + "/Push"))
//      {
//        Serial.println("PASSED");
//        Serial.println("PATH: " + firebaseData.dataPath());
//        Serial.print("PUSH NAME: ");
//        Serial.println(firebaseData.pushName());
//    
//        //Due to some internal server error, ETag cannot get from pushTimestamp
//        //Try to get ETag manually
//        Serial.println("ETag: " + Firebase.getETag(firebaseData, path + "/Push" + firebaseData.pushName()));
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//      else
//      {
//        Serial.println("FAILED");
//        Serial.println("REASON: " + firebaseData.errorReason());
//        Serial.println("------------------------------------");
//        Serial.println();
//      }
//      delay(3000);
//
//  } else {
//    //door 닫힘시 아무것도x
//  }
} //loop end

void printResult(FirebaseData &data)
{

  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData(), 5);
  else if (data.dataType() == "double")
    printf("%.9lf\n", data.doubleData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData() == 1 ? "true" : "false");
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
    if(data.stringData() == "a"){
      mySerial.println ('a'); //”DATA 1” “DATA2” will be printed on software serial monitor.
      delay(3000);
    }
    if(data.stringData() == "b"){
      mySerial.println ('b'); //”DATA 1” “DATA2” will be printed on software serial monitor.
      String path = "nfc";
      Serial.println("------------------------------------");
    Serial.println("push string...");
    //"AE-AF-4C-75"(원래) , "AC-AE-1B-42" (new)
    if (Firebase.pushString(firebaseData, path + "/Push", "AC-AE-1B-42"))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.print("VALUE: ");
      printResult(firebaseData);
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
      delay(3000);
    }
    if(data.stringData() == "c"){
      mySerial.println ('c'); //”DATA 1” “DATA2” will be printed on software serial monitor.
      delay(3000);
    }
  else if (data.dataType() == "json")
  {
    Serial.println();
    FirebaseJson &json = data.jsonObject();
    //Print all object data
    Serial.println("Pretty printed JSON data:");
    String jsonStr;
    json.toString(jsonStr, true);
    Serial.println(jsonStr);
    Serial.println();
    Serial.println("Iterate JSON data:");
    Serial.println();
    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
      if (type == FirebaseJson::JSON_OBJECT)
      {
        Serial.print(", Key: ");
        Serial.print(key);
      }
      Serial.print(", Value: ");
      Serial.println(value);
    }
    json.iteratorEnd();
  }
  else if (data.dataType() == "array")
  {
    Serial.println();
    //get array data from FirebaseData using FirebaseJsonArray object
    FirebaseJsonArray &arr = data.jsonArray();
    //Print all array values
    Serial.println("Pretty printed Array:");
    String arrStr;
    arr.toString(arrStr, true);
    Serial.println(arrStr);
    Serial.println();
    Serial.println("Iterate array values:");
    Serial.println();
    for (size_t i = 0; i < arr.size(); i++)
    {
      Serial.print(i);
      Serial.print(", Value: ");

      FirebaseJsonData &jsonData = data.jsonData();
      //Get the result data from FirebaseJsonArray object
      arr.get(jsonData, i);
      if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
        Serial.println(jsonData.boolValue ? "true" : "false");
      else if (jsonData.typeNum == FirebaseJson::JSON_INT)
        Serial.println(jsonData.intValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
        Serial.println(jsonData.floatValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
        printf("%.9lf\n", jsonData.doubleValue);
      else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
               jsonData.typeNum == FirebaseJson::JSON_NULL ||
               jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
               jsonData.typeNum == FirebaseJson::JSON_ARRAY)
        Serial.println(jsonData.stringValue);
    }
  }
  else
  {
    Serial.println(data.payload());
  }
}
