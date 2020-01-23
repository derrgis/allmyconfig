#include <FS.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <WebSocketsServer.h>
#include "FastLED.h"
int LED_WIDTH=123;
//#include

WebServer server(80);

bool isTable=true;

#include "pacmanlib.h"
void setup() {
  Serial.begin(115200);


  
 WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
    Serial.printf("Connecting ");
    WiFi.begin("WiFi-2.4-E19C", "yvesyves");
//WiFi.begin("DomainePutterie", "Jeremyyves");
    while (WiFi.status() != WL_CONNECTED) {
     
      
        delay(500);
        Serial.print(".");
    } 
      Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  initPacman();
  newPacman(); 
}
bool f=true;
void loop() {
  if(f)
  {
   calculpath(&Blinky,targetPacmanPosition);
  // calculpath(&Clyde,targetPacmanPosition);
    //  calculpath(&Pinky,targetPacmanPosition);
  }
   f=false;
  // put your main code here, to run repeatedly:
delay(100);
executePacmanSocketControl();
}
