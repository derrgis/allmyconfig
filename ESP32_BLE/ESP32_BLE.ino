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
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use ESP32 BLE
  to connect your project to Blynk.

  Warning: Bluetooth support is in beta!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "7929792dbe3c4a3783c7b6f372f82a12";
int Screen=0;
void setup()
{
  // Debug console
  Serial.begin(115200);


 //Blynk.setDeviceName("test");

  Blynk.begin(auth);
}


BLYNK_WRITE(V0) 
{ Screen=param.asInt();
}


void loop()
{
 // Serial.println(Screen);
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

}

