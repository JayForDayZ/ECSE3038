#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 

#define led1 2
#define led2 4
#define led3 15

bool order [8][3]={  {false,false,false},
                        {false,false,true },
                        {false,true ,false},
                        {false,true ,true },
                        {true ,false,false},
                        {true ,false,true },
                        {true ,true ,false},
                        {true ,true ,true },};

void setup() {

  Serial.begin(9600);

	pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
	pinMode(led3,OUTPUT);

	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");
	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    
    for (int i = 0 ; i < 8;i++){
      
      // Specify content-type header
      http.addHeader("X-API-Key",API_KEY);
      http.addHeader("Content-Type", "application/json");

      StaticJsonDocument<1024> doc;
      String httpRequestData;

      // Serialise JSON object into a string to be sent to the API
    

      doc["light_switch_1"] = order[i][0];
      doc["light_switch_2"] = order[i][1];
      doc["light_switch_3"] = order[i][2];

      digitalWrite(led1,order[i][0]);
      digitalWrite(led2,order[i][1]);
      digitalWrite(led3,order[i][2]);


      // convert JSON document, doc, to string and copies it into httpRequestData
      serializeJson(doc, httpRequestData);

      // Send HTTP PUT request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check reuslt of PUT request. negative response code means server wasn't reached
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("HTTP Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      delay(2000);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}