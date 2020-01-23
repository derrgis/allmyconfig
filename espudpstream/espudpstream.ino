/*
This is a basic example that will print out the header and the content of an ArtDmx packet.
This example uses the read() function and the different getter functions to read the data.
This example may be copied under the terms of the MIT license, see the LICENSE file for details
This works with ESP8266 and ESP32 based boards
*/
//char leds[32*32*3+1];
//#include <Artnet.h>
#include <Arduino.h>
//#include "FastLED.h"
#if defined(ARDUINO_SAMD_ZERO)
#include <WiFi101.h>
#include <WiFiUdp.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiUdp.h>
#else
#include <Ethernet.h>
#include <EthernetUdp.h>
#endif

#define ART_NET_PORT 100
// Opcodes
#define ART_POLL 0x2000
#define ART_POLL_REPLY 0x2100
#define ART_DMX 0x5000
#define ART_SYNC 0x5200
// Buffers
#define MAX_BUFFER_ARTNET 530
// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18
const char* ssid     = "yourssid";
const char* password = "yourpassword";
    WiFiUDP Udp;
char *artnetPacket;
void setup()
{
  Serial.begin(115200);
 Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
  WiFi.begin("WiFi-2.4-E19C","yvesyves");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(ART_NET_PORT);
  artnetPacket=(char*)malloc((123*3*3+1)*sizeof(char));

}
int k=0;
 
void loop()
{
  uint16_t sync=0;
 
 while (sync<0xFFFF)
 { 
int packetSize = Udp.parsePacket();
      if(packetSize>0)
      {
       // Serial.printf("size:%d\n",packetSize);
        
      Udp.read(artnetPacket, packetSize);
      //Serial.printf("univers:%d\n",artnetPacket[0]);
     sync=sync  | (1<<artnetPacket[0]);
      //free(artnetPacket);
      }
 }
           
  Serial.printf("frame done %d\n",k);
  k++;
}
