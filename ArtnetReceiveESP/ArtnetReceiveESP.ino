/*
This is a basic example that will print out the header and the content of an ArtDmx packet.
This example uses the read() function and the different getter functions to read the data.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
This works with ESP8266 and ESP32 based boards
*/
//char leds[32*32*3+1];
#include <Artnet.h>

const char* ssid     = "yourssid";
const char* password = "yourpassword";

Artnet artnet;

void setup()
{
  Serial.begin(115200);
 Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
  WiFi.begin("iPhone de Yves BAZIN","yvesyves");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 artnet.setframe((CRGB*)malloc(32*32*sizeof(CRGB))) ;
  artnet.begin(48*123,170);
}

void loop()
{
  long time3=ESP.getCycleCount();

                                 
                                
    
  artnet.read();
 // {
    // print out our data
   /*Serial.print("universe number = ");
    Serial.print(artnet.getUniverse());
    Serial.print("\tdata length = ");
    Serial.print(artnet.getLength());
    Serial.print("\tsequence n0. = ");
    Serial.println(artnet.getSequence());
    Serial.print("DMX data: ");
    for (int i = 0 ; i < artnet.getLength() ; i++)
    {
      Serial.print(artnet.getDmxFrame()[i]);
      Serial.print("  ");
    }
    Serial.println();
    Serial.println();*/
   // if(artnet.getUniverse()==2)
//artnet.setsync(2);
   // if(artnet.getsync()==255)
     //     {
              //displayPicNew(artnet.getframe(),0,0,32,32);
              //replaceled();
              //FastLEDshowESP32();
              artnet.resetsync();
              Serial.println("on affiche");
              
       //   }
           long time2=ESP.getCycleCount();
                             
long delta=time3-time2;
         Serial.printf("fps:%f\n",(float)240000000/(delta));
           
  
}
