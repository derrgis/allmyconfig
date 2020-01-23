/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "9bfb358632734840a5fb786f84f3418d";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi-2.4-E19C";
char pass[] = "yvesyves";
int Screen=0;

BLYNK_WRITE(V0) 
{ Screen=param.asInt();
}

void setup()
{
  
  // Debug console
 Serial.begin(115200);
/*WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
    Serial.printf("Connecting ");
    WiFi.begin("WiFi-2.4-E19C", "yvesyves");
//WiFi.begin("DomainePutterie", "Jeremyyves");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println(WiFi.status());
    
      
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  Serial.println("Waiting for connections...");*/
  Blynk.begin(auth,ssid,pass);
}

void loop()
{

  Blynk.run();
  if(Screen !=0)
  {
    Serial.println("******************Screeen*************");
    Serial.println("******************Screeen*************");
    Serial.println("******************Screeen*************");
    Serial.println("******************Screeen*************");
    Serial.println(Screen);
    Screen=0;
  }

   if( WiFi.status() == WL_CONNECTED) {
  Serial.println("Connected");
    //  Serial.println(WiFi.status());
  }
}

