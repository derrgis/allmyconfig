#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE
 #include <ArduinoJson.h>


extern "C" {
#include "user_interface.h"
}
// How many leds in your strip?


// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 3
#define CLOCK_PIN 13
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
//#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
//#include <MsTimer2.h>  // library to control of the timer 2 interupt http://playground.arduino.cc/Main/MsTimer2
//#include <TimerOne.h> 
#include <FS.h>
#include <EEPROM.h>
#include "string.h"
const char* ssid = "DomainePutterie";
const char* password = "Jeremyyves1";

MDNSResponder mdns;

//ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

#define DATA_PIN      D7     // for Huzzah: Pins w/o special function:  #4, #5, #12, #13, #14; // #16 does not work :(
#define LED_TYPE      WS2812
#define COLOR_ORDER   GRB
#define NUM_LEDS      420
#define LED_WIDTH 30
#define LED_HEIGHT 14
CRGB leds[NUM_LEDS];
CRGB solidColor = CRGB(10, 10, 10);
CRGB bgColor = CRGB(10, 10, 10);
CRGB Color = CRGB :: Blue;
CRGB Color2 = CRGB :: Green;
int i = 0;
int j;
int NUM_LEDS_ECL = 30;
int sperpentLength = 10;
int refresh = 50;

int k = 0;
int k2 = 0;

byte font[][7]=
{

//32->47
{B00000,  B00000, B00000, B00000, B00000, B00000, B00000  },{B01000,  B01000, B01000, B01000, B01000, B00000, B01000  },  {B01010,  B01010, B01010, B00000, B00000, B00000, B00000  },  {B01010,  B01010, B11111, B01010, B11111, B01010, B01010  },  {B00100,  B01111, B10100, B01110, B00101, B11110, B00100  },  {B11000,  B11001, B00010, B00100, B01000, B10011, B00011  },  {B01000,  B10100, B10100, B01000, B10101, B10010, B01101  },  {B01100,  B00100, B01000, B00000, B00000, B00000, B00000  },  {B00010,  B00100, B01000, B01000, B01000, B00100, B00010  },  {B01000,  B00100, B00010, B00010, B00010, B00100, B01000  },  {B10101,  B01110, B00100, B01110, B10101, B00000, B00000  },  {B00100,  B00100, B00100, B11111, B00100, B00100, B00100  },  {B00000,  B00000, B00000, B00000, B11000, B01000, B10000  },  {B00000,  B00000, B00000, B11100, B00000, B00000, B00000  },  {B00000,  B00000, B00000, B00000, B00000, B01100, B01100  },  {B00000,  B00001, B00010, B00100, B01000, B10000, B00000  },

//48->57
{B01110,  B10001, B10011, B10101, B11001, B10001, B01110  },  {B00100,  B01100, B00100, B00100, B00100, B00100, B01110  },  {B01110,  B10001, B00001, B00010, B00100, B01000, B11111  },  {B11111,  B00010, B00100, B00010, B00001, B10001, B01110  },  {B00010,  B00110, B01010, B10010, B11111, B00010, B00010  },  {B11111,  B10000, B11110, B00001, B00001, B10001, B01110  },  {B00110,  B01000, B10000, B11110, B10001, B10001, B01110  },  {B11111,  B00001, B00010, B00100, B01000, B10000, B10000  },  {B01110,  B10001, B10001, B01110, B10001, B10001, B01110  },  {B01110,  B10001, B10001, B01111, B00001, B00010, B01100  },

//58->64
{B00000,  B01100, B01100, B00000, B01100, B01100, B00000  },  {B00000,  B01100, B01100, B00000, B01100, B00100, B01000  },  {B00010,  B00100, B01000, B10000, B01000, B00100, B00010  },  {B00000,  B00000, B11111, B00000, B11111, B00000, B00000  },  {B10000,  B01000, B00100, B00010, B00100, B01000, B10000  },  {B01110,  B10001, B00001, B00010, B00100, B00000, B00100  },  {B01110,  B10001, B10011, B10100, B10101, B10001, B01110  },

//65->90
{B01110,  B10001, B10001, B11111, B10001, B10001, B10001  },  {B11110,  B10001, B10001, B11110, B10001, B10001, B11110  },  {B01110,  B10001, B10000, B10000, B10000, B10001, B01110  },  {B11110,  B10001, B10001, B10001, B10001, B10001, B11110  },  {B11111,  B10000, B10000, B11110, B10000, B10000, B11111  },  {B11111,  B10000, B10000, B11110, B10000, B10000, B10000  },  {B01110,  B10001, B10000, B10111, B10001, B10001, B01110  },  {B10001,  B10001, B10001, B11111, B10001, B10001, B10001  },  {B01110,  B00100, B00100, B00100, B00100, B00100, B01110  },  {B00111,  B00010, B00010, B00010, B00010, B10010, B01100  },  {B10001,  B10010, B10100, B11000, B10100, B10010, B10001  },  {B10000,  B10000, B10000, B10000, B10000, B10000, B11111  },  {B10001,  B11011, B10101, B10101, B10001, B10001, B10001  },  {B10001,  B10001, B11001, B10101, B10011, B10001, B10001  },  {B01110,  B10001, B10001, B10001, B10001, B10001, B01110  },  {B11110,  B10001, B10001, B11110, B10000, B10000, B10000  },  {B01110,  B10001, B10001, B10001, B10101, B10010, B01101  },  {B11110,  B10001, B10001, B11110, B10100, B10010, B10001  },  {B01111,  B10000, B10000, B01110, B00001, B00001, B11110  },  {B11111,  B00100, B00100, B00100, B00100, B00100, B00100  },  {B10001,  B10001, B10001, B10001, B10001, B10001, B01110  },  {B10001,  B10001, B10001, B10001, B10001, B01010, B00100  },  {B10001,  B10001, B10001, B10001, B10101, B10101, B01010  },  {B10001,  B10001, B01010, B00100, B01010, B10001, B10001  },  {B10001,  B10001, B10001, B01010, B00100, B00100, B00100  },  {B11111,  B00001, B00010, B00100, B01000, B10000, B11111  },

//91->96
{B01110,  B01000, B01000, B01000, B01000, B01000, B01110  },  {B10001,  B01010, B11111, B00100, B11111, B00100, B00100  },  {B01110,  B00010, B00010, B00010, B00010, B00010, B01110  },  {B00100,  B01010, B10001, B00000, B00000, B00000, B00000  },  {B00000,  B00000, B00000, B00000, B00000, B00000, B11111  },  {B01000,  B00100, B00010, B00000, B00000, B00000, B00000  },

//97->122
{B00000,  B00000, B01110, B00001, B01111, B10001, B01111  },  {B10000,  B10000, B10110, B11001, B10001, B10001, B11110  },  {B00000,  B00000, B01110, B10000, B10000, B10001, B01110  },  {B00001,  B00001, B01101, B10011, B10001, B10001, B01111  },  {B00000,  B00000, B01110, B10001, B11111, B10000, B01110  },  {B00110,  B01001, B01000, B11100, B01000, B01000, B01000  },  {B00000,  B01111, B10001, B10001, B01111, B00001, B01110  },  {B10000,  B10000, B10110, B11001, B10001, B10001, B10001  },  {B00100,  B00000, B01100, B00100, B00100, B00100, B01110  },  {B00010,  B00000, B00110, B00010, B00010, B10010, B01100  },  {B10000,  B10000, B10010, B10100, B11000, B10100, B10010  },  {B01100,  B00100, B00100, B00100, B00100, B00100, B01110  },  {B00000,  B00000, B11010, B10101, B10101, B10001, B10001  },  {B00000,  B00000, B10110, B11001, B10001, B10001, B10001  },  {B00000,  B00000, B01110, B10001, B10001, B10001, B01110  },  {B00000,  B00000, B11110, B10001, B11110, B10000, B10000  },  {B00000,  B00000, B01110, B10001, B01111, B00001, B00001  },  {B00000,  B00000, B10110, B11001, B10000, B10000, B10000  },  {B00000,  B00000, B01110, B10000, B01110, B00001, B11110  },  {B01000,  B01000, B11100, B01000, B01000, B01001, B00110  },  {B00000,  B00000, B10001, B10001, B10001, B10011, B01101  },  {B00000,  B00000, B10001, B10001, B10001, B01010, B00100  },  {B00000,  B00000, B10001, B10001, B10001, B10101, B01010  },  {B00000,  B00000, B10001, B01010, B00100, B01010, B10001  },  {B00000,  B00000, B10001, B10001, B01111, B00001, B01110  },  {B00000,  B00000, B11111, B00010, B00100, B01000, B11111  },


};
byte lettre[][5] = {
  {
    B0000,
    B0000,
    B0000,
    B0000,
    B0000
  },
 
  { //0
    B1100,
    B1010,
    B1010,
    B1010,
    B0110
  },
  { //1
    B110,
    B010,
    B010,
    B010,
    B010
  },
  {
    B0100, //a
    B1010,
    B1110,
    B1010,
    B1010
  },
  {
    B1100, //b
    B1010,
    B1100,
    B1010,
    B1100

  },
  {
    B0110, //c
    B1000,
    B1000,
    B1000,
    B0110
  },
  {
    B1100, //D
    B1010,
    B1010,
    B1010,
    B1100
  },
  {
    B1110, //e
    B1000,
    B1100,
    B1000,
    B1110
  },
  {
    B1110, //f
    B1000,
    B1100,
    B1000,
    B1000
  },
  {
    B01100, //g
    B10000,
    B10110,
    B10010,
    B01100,
  },
  {
    B1010, //H
    B1010,
    B1110,
    B1010,
    B1010,
  },
  {
    B10, //I
    B10,
    B10,
    B10,
    B10,
  },
  {
    B0110, //J
    B0010,
    B0010,
    B0010,
    B1100,
  },
  {
    B1010, //K
    B1010,
    B1100,
    B1010,
    B1010,
  },
  {
    B1000, //L
    B1000,
    B1000,
    B1000,
    B0110,
  },
  {
    B1010, //M
    B1110,
    B1010,
    B1010,
    B1010,
  },
  {
    B1100, //N
    B1010,
    B1010,
    B1010,
    B1010,
  },
  { //o
    B0100,
    B1010,
    B1010,
    B1010,
    B0100,
  },
  { //p
    B1100,
    B1010,
    B1100,
    B1000,
    B1000,
  },
  {
    B0110, //Q
    B1010,
    B0110,
    B0010,
    B0010
  },
  { //R
    B1100,
    B1010,
    B1100,
    B1010,
    B1010
  },
  {
    B0110,
    B1000,
    B0110,
    B0010,
    B1100
  },
  { //T
    B1110,
    B0100,
    B0100,
    B0100,
    B0100
  },
  { //U
    B1010,
    B1010,
    B1010,
    B1010,
    B0110
  },
  { //v
    B1010,
    B1010,
    B1010,
    B1100,
    B1000,
  },
  { //w
    B1010,
    B1010,
    B1010,
    B1110,
    B1010
  },
  { //x
    B1010,
    B1010,
    B0100,
    B1010,
    B1010,
  },
  { //y
    B1010,
    B1010,
    B0100,
    B0100,
    B0100
  },
  { //z
    B1110,
    B0010,
    B0100,
    B1000,
    B1110,
  }
};


CRGB  ghost[14][14] = {
  {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1},
  {1, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1},
  {0, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0}
};
CRGB ghostred[296];
CRGB ghostyellow[296];
CRGB ghostgreen[296];
CRGB ghostPurple[296];
CRGB ghostCyan[296];


CRGB fraise[11][10] = {
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 2, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
  {0, 1, 2, 1, 2, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 2, 1, 0},
  {1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 2, 1, 2, 1},
  {1, 2, 1, 2, 1, 3, 1, 1, 1, 1},
  {0, 1, 1, 1, 3, 3, 3, 1, 2, 0},
  {0, 0, 3, 3, 3, 2, 3, 3, 3, 0},
  {0, 0, 0, 0, 0, 2, 0, 0, 0, 0}
};

CRGB fraiseCalc[110];
CRGB transparent = CRGB(1, 1, 1);

int YO = 0;

int cerise[196] = {
  100, 100, 100, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 0, 1, 1, 1, 1, 0, 100, 100, 100, 100, 100, 100,
  100, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 100, 100,
  100, 0, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 100,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 1, 0,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0,
  100, 100, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 0, 3, 0, 1, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 3, 0, 100, 0, 1, 1, 1, 1, 0, 100,
  100, 100, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 100, 100,
  100, 0, 0, 3, 0, 3, 3, 0, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

int mario[238]={
   100,3,3,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
   100,3,3,3,2,2,2,2,2,2,100,100,100,100,100,100,100, 
   100,100,3,3,2,2,2,2,2,2,2,2,2,2,3,3,100,
   100,100,100,100,2,2,2,2,4,2,2,2,2,2,3,3,100,
    100,5,5,1,1,1,1,2,2,2,2,4,2,2,3,3,100, 
  5,5,1,1,1,1,1,2,1,1,1,2,100,100,3,3,100, 
    5,5,1,1,1,1,2,1,1,1,2,1,1,100,100,3,100,
  100,100,100,100,100,5,5,5,5,5,5,5,1,1,100,100,100,
  100,100,100,3,3,5,5,5,5,6,6,6,6,1,100,100,100,
  100,100,100,3,5,3,3,5,5,5,6,5,5,5,1,100,100,
  100,100,100,3,5,3,5,5,5,6,5,5,5,1,1,100,100,
  100,100,100,100,3,3,3,5,5,6,5,100,1,1,1,100,100, 
  100,100,100,100,1,1,1,1,1,1,1,1,1,5,5,100,100,
  100,100,100,100,100,1,1,1,1,1,100,100,5,5,5,100,100            
};


CRGB palec[4] = {CRGB::Black, CRGB::Red, CRGB::White, CRGB::Green};

CRGB palm[7]={CRGB:: Red,CRGB::Red, CRGB::Blue,CRGB(98,65,7),CRGB::Yellow,CRGB(238,151,64),CRGB::Black};

CRGB backgroundBlocks [160];     //8 x 16 display + 4 x 8 blocks above for blocks to spawn in
CRGB displayScreen [160]; 
CRGB TdisplayScreen [160];  //8 x 16 display + 4 x 8 blocks above for blocks to spawn in
int blockType ;                   //7 different block types
int state;                        //4 possible rotation states
const int activeBlocks [28][4][4] = {  
                           {{0,0,1,0},    //     blockType + 7(state)
                            {0,1,1,0},    // 0 =    0      + 7(0)
                            {0,1,0,0},
                            {0,0,0,0}}  ,
                         
                           {{0,2,0,0},    // 1 =    1      + 7(0)
                            {0,2,2,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,3,0,0},    // 2 =    2      + 7(0)
                            {0,3,3,0},
                            {0,0,3,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 3 =    3      + 7(0)
                            {0,4,4,0},
                            {0,4,4,0},
                            {0,0,0,0}} ,
                            
                           {{0,5,0,0},    // 4 =    4      + 7(0)
                            {0,5,0,0},
                            {0,5,5,0},
                            {0,0,0,0}} ,
                            
                           {{0,6,6,0},    // 5 =    5      + 7(0)
                            {0,6,0,0},
                            {0,6,0,0},
                            {0,0,0,0}} ,
                           
                           {{0,0,7,0},    // 6 =    6      + 7(0)
                            {0,0,7,0},
                            {0,0,7,0},
                            {0,0,7,0}} ,
                            
                            {{0,0,0,0},  // 7 =    0      + 7(1)
                            {1,1,0,0},    
                            {0,1,1,0},
                            {0,0,0,0}}  ,
                        
                           {{0,0,0,0},    // 8 =    1      + 7(1)
                            {2,2,2,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 9 =    2      + 7(1)
                            {0,3,3,0},
                            {3,3,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 10 =    3      + 7(1)
                            {0,4,4,0},
                            {0,4,4,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 11 =    4      + 7(1)
                            {5,5,5,0},
                            {5,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 12 =    5      + 7(1)
                            {6,6,6,0},
                            {0,0,6,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 13 =    6      + 7(1)
                            {0,0,0,0},
                            {7,7,7,7},
                            {0,0,0,0}} ,

                           {{0,1,0,0},    // 14 =    0      + 7(2)
                            {1,1,0,0},    
                            {1,0,0,0},
                            {0,0,0,0}}  ,
                            
                           {{0,2,0,0},    // 15 =    1      + 7(2)
                            {2,2,0,0},
                            {0,2,0,0},
                            {0,0,0,0}} ,
                            
                           {{3,0,0,0},    // 16 =    2      + 7(2)
                            {3,3,0,0},
                            {0,3,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 17 =    3      + 7(2)
                            {0,4,4,0},
                            {0,4,4,0},
                            {0,0,0,0}} ,
                            
                           {{5,5,0,0},    // 18 =    4      + 7(2)
                            {0,5,0,0},
                            {0,5,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,6,0,0},    // 19 =    5      + 7(2)
                            {0,6,0,0},
                            {6,6,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,7,0,0},    // 20 =    6      + 7(2)
                            {0,7,0,0},
                            {0,7,0,0},
                            {0,7,0,0}} ,

                           {{1,1,0,0},    // 21 =    0      + 7(3)
                            {0,1,1,0},    
                            {0,0,0,0},
                            {0,0,0,0}}  ,
                            
                           {{0,2,0,0},    // 22 =    1      + 7(3)
                            {2,2,2,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,3,3,0},    // 23 =    2      + 7(3)
                            {3,3,0,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 24 =    3      + 7(3)
                            {0,4,4,0},
                            {0,4,4,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,5,0},    // 25 =    4      + 7(3)
                            {5,5,5,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{6,0,0,0},    // 26 =    5      + 7(3)
                            {6,6,6,0},
                            {0,0,0,0},
                            {0,0,0,0}} ,
                            
                           {{0,0,0,0},    // 27 =    6      + 7(3)
                            {7,7,7,7},
                            {0,0,0,0},
                            {0,0,0,0}}
                          };
                          
CRGB paletblock[10]={CRGB::Black,CRGB::Red,CRGB::Green,CRGB::Blue,CRGB::Yellow,CRGB::Purple,CRGB::Cyan,CRGB::White} ;                                                                                                   
int blockColumn;             //the x value of the left most row in the 4x4 active block placeholder
int blockRow;                //the y value of the bottom column in the 4x4 active block placeholder

int rowToBeDeleted;
bool inGame = false;
bool justLost = false;
int brightness = 8;
long fallRate;
long dixs=4000000;




void displaypic( CRGB *pic, int x0, int y0, int h, int w)
{
  if (x0 >= LED_WIDTH or y0 >= LED_HEIGHT)
    return;
  int maxx = LED_WIDTH - x0;
  int maxxy = LED_HEIGHT - y0 ;
  int hmin = (h > maxxy) ? maxxy : h;
  int wmin = (w > maxx) ? maxx : w;
  // Serial.println(hmin);
  //Serial.println(wmin);
  for (int y = y0; y < hmin + y0; y++)
  {
    if (y >= 0) {
      if (y % 2 == 0)
      {
        if (x0 < 0)
        {
          int nh = wmin + x0;
          if (nh > 0)
            memcpy(&leds[LED_WIDTH * y], &pic[(y - y0) *w - x0], nh * sizeof(CRGB));
        }
        else
          memcpy(&leds[LED_WIDTH * y + x0], &pic[(y - y0)*w], wmin * sizeof(CRGB));
      }
      else
      {
        for (int x = x0; x < wmin + x0; x++)
        {
          if (x >= 0)
            leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x] = pic[w * (y - y0) + x - x0 ];
        }
      }
    }
  }
}


void newGame()
{
    fallRate=10;
    Serial.println("voici tetris");                    // clears the red display matrix
  resetArrays();
  newBlock();
  updateDisplay();
  //MsTimer2::set(10, updateDisplay);     // initialises timer2, timer2 calls updateDisplay() every 10ms
  //MsTimer2::start();                    // starts timer2
  fallRate = 10;       // set initial fall rate to 1000000 nanoseconds
  //Timer1.initialize(fallRate);          // initialises timer1
  //Timer1.attachInterrupt(fall);         // timer1 calls fall() 
  noInterrupts();
  randomSeed(analogRead(0));
  timer0_isr_init();
  timer0_attachInterrupt(fall);
  int next=ESP.getCycleCount() + fallRate*dixs;
  timer0_write(next);
    
 // timer1_isr_init();
  //timer1_attachInterrupt(updateDisplay);
  //next=1000;
  //timer1_write(next);
  interrupts();
}
  
void updateDisplay() // this function is called by a timer interupt every 10 milliseconds
{
  //Serial.println("on affiche");
  combineArrays();   
   


 
    // adds the latest position of the active block to the displayScreen Array
 //Serial.println("on display");
  displayArray();       

  FastLED.show();// outputs the displayScreen Array
}

void  combineArrays()  //updates the displayScreen array by combining the current positions of the backgroundBlocks and the activeBlocks
{
  //first make an exact copy of the backgroundBlocks
 // Serial.println("on combine");
  for(byte y = 0; y < 20; y++)
    for(byte x = 0; x < 8; x++)
      displayScreen[x+8*y] = backgroundBlocks[x+8*y];
      
  //then add the non zero elements of the activeBlocks
  for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++)
     if(activeBlocks[blockType + (7 * state)][x][y] > 0)
      {
     //   Serial.print("e:");
      //Serial.println(activeBlocks[blockType + (7 * state)][x][y]);
        displayScreen[x + blockColumn+8*(y + blockRow)] = paletblock[ activeBlocks[blockType + (7 * state)][x][y] ];
      }
}

void displayArray()  // cycles through each pixel in displayScreen and outputs corresponding colour
{

   for(int y=0;y<20;y++)
    {
      for (int x=0;x<8;x++)
      {
      // Serial.print(displayScreen[8*y+x]);
       TdisplayScreen[20*x+y]=displayScreen[8*y+x];
      }
     // Serial.println();
    }
  displaypic(TdisplayScreen,0,0,8,20);
 
}

void resetArrays()  // resets arrays to all zeros
{
  for(byte y = 0; y < 20; y++)
     for(byte x = 0; x < 8; x++)
     {
        displayScreen[x+8*y] = CRGB(0,0,0);
        backgroundBlocks[x+8*y] = CRGB(0,0,0);
}   }

void newBlock() // 
{
  blockColumn = 2;       
  blockRow = 16;          // new block spawns at 2,14
  blockType = random(7);  // blocktype is a random between 0 and 6 inclusive
  Serial.println(blockType);
  state = 0;     // reset state back to spawn block state
}

void fall()               // this function is called by a timer interupt every fallRate nanoseconds
{
 // Serial.println("on fal");
 noInterrupts();
  if(canMoveDown())
    blockRow--;
  else   //block cant move
  {
    moveActiveBlocksToBackground();      // adds the activeBlocks to the background display
    deleteFullRows();                    // deletes any full rows caused by now stationary block
    if(spaceForNewBlock())  
    {// checks can a new block spawn
      newBlock();                
    }
    else  // new block cant spawn so game over
    {                           
   //   MsTimer2::stop();                  // stops timer2 interupt
     // Timer1.detachInterrupt();
     timer0_detachInterrupt();
    // timer1_detachInterrupt();// stops timer1 interupt
      inGame = false;           
      justLost = true;
      fillScreenT();  
      interrupts();
      return;
    }
  }
  updateDisplay();
int    next=ESP.getCycleCount() +  fallRate*dixs;
   timer0_write(next);  
   interrupts();
}


void deleteFullRows()
{
   for(int i = 0; i < 4; i++) // checking 4 times if there are full rows to be deleted
   {
      if(rowIsFull())
      {
        deleteRow();
        speedUpFalling();
      }
   }
} 

void speedUpFalling()
{
  if(fallRate > 1)   // to stop fallRate decreasing to zero
    fallRate -= 1;   // blocks falling speeds up by 50000 nanoseconds every full row made

  //Timer1.setPeriod(fallRate);  // updates the timer inetupt with the new fallRate

  //int next=ESP.getCycleCount()+fallRate;
  //timer0_write(next);
  
}

bool spaceForNewBlock()  // returns true if there is space for the new block to be spawned at the top of the matrix
{
  if(canBeMoved(2, 14))
    return true;
  else
    return false;
}

void fillScreenT()       // fills pixels one at a time with green at the end of the game one at a time starting from the top
{
  for(int y = 0; y <20; y++)
  {
    for(int x = 0; x < 8; x++)
    {
        displayScreen[x+8*y] = CRGB::Green;    // turns the pixel green
        displayArray();   
        FastLED.show();// outputs the pixel change to the screen
        delay(30);
    }
  } 
}

bool canBeMoved(int newCol, int newRow)  // return true if the there is empty pixels for the block to move in to
{ 
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 4; x++)
    {
      if(activeBlocks[blockType + (7 * state)][x][y] > 0)   // finds the positions of the non-blank blocks in the 4x4 matrix
      {
        int boardX = x + newCol;                            // calculates the corresponding x position on the display board
        int boardY = y + newRow;                            // calculates the corresponding y position on the display board
        if(boardX < 0 ||  boardX > 7 || boardY < 0)         // if the pixel is outside the boundaries of the board
          return false;
        if(backgroundBlocks[boardX+8*boardY] > CRGB(0,0,0))            // if the pixel is already full
          return false;
      }  
    }
  }
  return true;
}

bool canMoveDown()   // row - 1
{
  if(canBeMoved(blockColumn, blockRow -1))
    return true;
  else
    return false;
}

bool canMoveLeft()
{
  if(canBeMoved(blockColumn - 1, blockRow)) // column -1
    return true;
  else
    return false;
}

bool canMoveRight()
{
  if(canBeMoved(blockColumn + 1, blockRow)) // column + 1
    return true;
  else
    return false;
}

bool canBeRotated(int newState)
{
  for(int y = 0; y < 4; y++)
  {
    for(int x = 0; x < 4; x++)
    {
      if(activeBlocks[blockType + (7 * newState)][x][y] > 0) // finds the positions of the non-blank blocks in the 4x4 matrix
      {
        int boardX = x + blockColumn;                        // calculates the corresponding x position on the display board
        int boardY = y + blockRow;                           // calculates the corresponding y position on the display board
        if(boardX < 0 ||  boardX > 7 || boardY < 0)          // if the pixel is outside the boundaries of the board
          return false;
        
        if(backgroundBlocks[boardX+8*boardY] > CRGB(0,0,0))             // if the pixel is already full
          return false; 
      }  
    }
  }
  return true;
}

bool canRotateLeft()
{
  int nextState;
  if(state == 0)      // state to the left of 0 is 3
    nextState = 3;
  else
    nextState = state - 1;  
  
  if(canBeRotated(nextState))
    return true;
  else
    return false;
}

bool canRotateRight()
{
  int nextState;
  if(state == 3)      // state to the right of 3 is 0
    nextState = 0;
  else
    nextState = state + 1;

  if(canBeRotated(nextState))
    return true;
  else
    return false;
}

void moveActiveBlocksToBackground()
{
  combineArrays(); //updated the display array
  
  for(byte y = 0; y < 16; y++)
    for(byte x = 0; x < 8; x++)
      backgroundBlocks[x+8*y] = displayScreen[x+8*y];    // copy the displayScreen to the backgroundBlocks
}

bool rowIsFull()                // goes through all 16 rows and checks if they are full
{
  for(byte y = 0; y < 16; y++)
  {
    if((backgroundBlocks[8*y] > CRGB(0,0,0)) && (backgroundBlocks[1+8*y] > CRGB(0,0,0) ) && (backgroundBlocks[2+8*y] > CRGB(0,0,0) ) && (backgroundBlocks[3+8*y] > CRGB(0,0,0) ) &&
        (backgroundBlocks[4+8*y] > CRGB(0,0,0) ) && (backgroundBlocks[5+8*y] > CRGB(0,0,0) ) && (backgroundBlocks[6+8*y] > CRGB(0,0,0) ) && (backgroundBlocks[7+8*y] > CRGB(0,0,0) ))
    {
       rowToBeDeleted = y;      // the row is returned as a global variable
       return true;
    }
  }
  return false; 
}

void deleteRow()   
{
  
  for(byte y = rowToBeDeleted; y < 16; y++)  
    for(byte x = 0; x < 8; x++)
      backgroundBlocks[x+8*y] = backgroundBlocks[x+8*(y + 1)];  // replaces row with row above 
}




void calculpic(int *pic, CRGB *palette, CRGB color, int h, int w,CRGB *result)
{
  Serial.println("on est là");
  int to = h * w;
 // static CRGB result[296];
  for (int m = 0; m < h; m++)
  {
    for (int y = 0; y < w; y++)
    {

      result[m * w + y] = palette[pic[m * w + y]];
      //Serial.print(palette[pic[m * w + y]]);
      //Serial.printf(":%d\n", pic[m * w + y]);


      if (pic[m * w + y] == 100)
      {
        result[m * w + y] = color;

      }
      if (pic[m * w + y] == 9)
      {
        result[m * w + y] = color;

      }

    }
  }
 // return result;

}

CRGB cerisecalc[196];

CRGB mariocalc[238];
String function = "tetris";


int taille[34] = {2,4,3, 4, 4, 4, 4, 4, 4, 5, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
String mess = String("HELLO FIRST LIGHT AND SCROLL ON THE TABLE   ONLY 6 ROWS BUT GETTING THERE   ");
String mess2 = "           DOUBLE LIGNE        ";
byte coul[] = {1, 2, 2, 3, 3, 2, 3, 1, 2, 3, 1, 1, 2, 3, 3, 3, 1, 2, 2, 3, 3, 2, 3, 1, 2, 3, 1, 1, 2, 3, 3, 3, 1, 2, 2, 3, 3, 2, 3, 1, 2, 3, 1, 1, 2, 3, 3, 3, 1, 2, 2, 3, 3, 2, 3, 1, 2, 3, 1, 1, 2, 3, 3, 3, 2, 3, 1, 2, 3, 1, 1, 2, 3, 3, 3};
int lens = mess.length();



void strcopy(String g)
{
  for (int i = 0; i < g.length(); i++)
  {
    mess[i] = g[i];
  }
  mess[g.length()] = 0;
}

void calculfraise(CRGB color)
{
  for (int m = 0; m < 11; m++)
  {
    for (int y = 0; y < 10; y++)
    {
      if (fraise[m][y] == (CRGB)1)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Red;

      }
      if (fraise[m][y] == (CRGB)2)
      {
        fraiseCalc[m * 10 + y] = CRGB :: White;

      }

      if (fraise[m][y] == (CRGB)3)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Green;

      }
      if (fraise[m][y] == (CRGB)0)
      {
        fraiseCalc[m * 10 + y] = color;

      }

    }
  }
}

void calculghosts(CRGB color)
{
  for (int m = 0; m < 14; m++)
  {
    for (int y = 0; y < 14; y++)
    {
      if (ghost[m][y] == (CRGB)1)
      {
        ghostred[m * 14 + y] = CRGB :: Red;
        ghostyellow[m * 14 + y] = CRGB :: Yellow;
        ghostgreen[m * 14 + y] = CRGB :: Green;
        ghostPurple[m * 14 + y] = CRGB :: Purple;
        ghostCyan[m * 14 + y] = CRGB(64, 222, 255);
      }
      if (ghost[m][y] == (CRGB)2)
      {
        ghostred[m * 14 + y] = CRGB :: Blue;
        ghostyellow[m * 14 + y] = CRGB :: Blue;
        ghostgreen[m * 14 + y] = CRGB :: Blue;
        ghostPurple[m * 14 + y] = CRGB :: Blue;
        ghostCyan[m * 14 + y] = CRGB :: Blue;
      }

      if (ghost[m][y] == (CRGB)3)
      {
        ghostred[m * 14 + y] = CRGB :: White;
        ghostyellow[m * 14 + y] = CRGB :: White;
        ghostgreen[m * 14 + y] = CRGB :: White;
        ghostPurple[m * 14 + y] = CRGB :: White;
        ghostCyan[m * 14 + y] = CRGB :: White;
      }
      if (ghost[m][y] == (CRGB)0)
      {
        ghostred[m * 14 + y] = color;
        ghostyellow[m * 14 + y] = color;
        ghostgreen[m * 14 + y] = color;
        ghostPurple[m * 14 + y] = color;
        ghostCyan[m * 14 + y] = color;
      }

    }
  }
}








void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  noInterrupts();
  Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\r\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // Send the current LED status
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\r\n", num, payload);
  if (strcmp("up", (const char *)payload) == 0)  {
        //writeLED(true);
        Serial.println("on go up");
         YO = (YO + 1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
      }
      else  if (strcmp("down", (const char *)payload) == 0)  {
        Serial.println("on go down");
              YO = (YO -1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
      }

      if (strcmp("right", (const char *)payload) == 0)  {

      Serial.println("on va a droite");
      if(canMoveRight())
        {
            noInterrupts();
          blockColumn++;
          updateDisplay();
           int next=ESP.getCycleCount()+fallRate*dixs;
            timer0_write(next);
          interrupts();
        }     
      }

 if (strcmp("left", (const char *)payload) == 0)  {

      Serial.println("on va a droite");
      if(canMoveLeft())
        {
            noInterrupts();
          blockColumn--;
          updateDisplay();
           int next=ESP.getCycleCount()+fallRate*dixs;
            timer0_write(next);
          interrupts();
        }     
      }
 if (strcmp("new", (const char *)payload) == 0)  {
  newGame();
 }

 if (strcmp("turnr", (const char *)payload) == 0)  {
 
      if(canRotateRight())
      {
         noInterrupts();
        if(state == 3)
          state = 0;  // state to the right of 3 is 0
        else
          state++;
                     
          updateDisplay();
           int next=ESP.getCycleCount()+fallRate*dixs;
            timer0_write(next);
          interrupts();
      }
    
  }




 if (strcmp("turnl", (const char *)payload) == 0)  {
 
      if(canRotateLeft())
      {
         noInterrupts();
        if(state == 0)
          state = 3;  // state to the right of 3 is 0
        else
          state--;
                     
          updateDisplay();
           int next=ESP.getCycleCount()+fallRate*dixs;
            timer0_write(next);
          interrupts();
      }
    
  }
  
      
      else {
        Serial.println("Unknown command");
      }
      // send data to all connected clients
      //webSocket.broadcastTXT(payload, length);
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\r\n", num, length);
      hexdump(payload, length);

      // echo data back to browser
     // webSocket.sendBIN(num, payload, length);
      break;
    default:
      Serial.printf("Invalid WStype [%d]\r\n", type);
      break;
  }

 interrupts();
}



void getmeteo()
{
  int nbretry=3;
  String json = "";
  char  *host="api.openweathermap.org";
Serial.println("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  // while(nbretry >1 and !((const char *)json.charAt(1)=="}") )
 //{
 // String json = "";
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/data/2.5/weather?zip=1000,be&units=metric&appid=cee2dece9fdd919d420d4f5972ec3f1e";

  Serial.print("Requesting URL: ");
  Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" + 
                     "Connection: close\r\n\r\n");

        // This will send the request to the server
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }
        
       
       // delay(20);
      
        // Read all the lines of the reply from server and print them to Serial
        Serial.println("Respond:");
      
      String str="";
      boolean httpBody = false;
      while (client.available()) {
        String line = client.readStringUntil('\r');
        str="line:charat0";  //+":"+line.charAt(1);
        //Serial.println(str);
        //Serial.println(line.charAt(1));
       // Serial.println(line.charAt(0));
        //Serial.println(line);
       // Serial.println("end line");
        if (!httpBody and (line.charAt(1) == '{')) {
          httpBody = true;
        }
        if (httpBody) {
          // Serial.print("line:");
         // Serial.println(line);
          json += line;
         //Serial.print("json:");
        // Serial.println(json);
        }
      }
    //  nbretry--;
 //}
 
StaticJsonBuffer<1000> jsonBuffer;
//Serial.println("Got data json:");
//Serial.println(json);
JsonObject &root = jsonBuffer.parseObject(json);
String data = root["name"];
String tp= root["main"]["temp"];
int temp=tp.toInt();
String ville=(String)data[0]+(String)data[1]+(String)data[2]+(String)data[3 ]+(String)data[4 ];
//data.toUpperCase();
if (data.length()>0)
{
  affichetext(ville,0,7);
  affichetext(String(temp), 12,0);
  affichetext("C",24,0);
}
//k2=0;
  //Serial.println(data);
   Serial.println(ville); Serial.println(String(temp));
  //Serial.println("closing connection");
  
}

void setup() {
  // put your setup code here, to run once:



  calculghosts(solidColor);
  calculfraise(solidColor);


  Serial.begin(115200);
  delay(100);
  Serial.setDebugOutput(true);

  calculpic(cerise, palec, solidColor, 14, 14,cerisecalc);
   calculpic(mario, palm, solidColor, 14, 17,mariocalc);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);         // for WS2812 (Neopixel)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  //FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(64);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
  fill_solid(leds, NUM_LEDS, solidColor);
  FastLED.show();


  SPIFFS.begin();

  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
  }
  Serial.printf("\n");



  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s\n", ssid);
  //if (String(WiFi.SSID()) != String(ssid)) {
  Serial.printf("Connecting ");
  WiFi.begin(ssid, password);
  //}

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected! Open http://");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser");
 //if(mdns.begin("ledtable",WiFi.localIP()))
  //Serial.println("MDNS has started");


  server.on("/s", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("s"  );
   // char* r2=server.arg("s");
    refresh = r.toInt();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/meteo", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    if (function="tetris")
         timer0_detachInterrupt();
    function = "meteo";
    fill_solid(leds, NUM_LEDS, bgColor);
       getmeteo();
            FastLED.show();
      FastLED.delay((int)(1000 / refresh) + 1);
       server.send(200, "text/html", "done");
  });


  server.on("/c", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    Color = CRGB(r.toInt(), g.toInt(), b.toInt());
    server.send(200, "text/html", "done");
    // i=0;
  });


  server.on("/c2", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    Color2 = CRGB(r.toInt(), g.toInt(), b.toInt());
    server.send(200, "text/html", "done");
    // i=0;
  });




  server.on("/changetext", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();

    String g = server.arg("v");
    Serial.println(g);
    //strcopy(g);
    mess = g;
    k2 = 0;
    Serial.println(mess);
    lens = mess.length();
    Serial.println(lens);
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/change", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String g = server.arg("v");
     Serial.println(g);
    if (g=="pacman")
    {
      if (function="tetris")
         timer0_detachInterrupt();
       YO=0;
       calculghosts(bgColor) ;
    calculfraise(bgColor);
  
     calculpic(cerise, palec, bgColor, 14, 14,cerisecalc);
   calculpic(mario, palm, bgColor, 14, 17,mariocalc);
   function = g;
    }
    if(g=="scroll")
    {
       if (function="tetris")
         timer0_detachInterrupt();
      YO=5;
      function = g;
    }
    if(g=="tetris")
    {
      function = g;
      Serial.println(function);
          fill_solid(leds, NUM_LEDS, bgColor);
         FastLED.show();
          
           interrupts();
          newGame();
           server.send(200, "text/html", "done");
          return;
    }
      
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });



  server.on("/up", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    YO = (YO + 1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
    server.send(200, "text/html", "done");
    // i=0;
  });

  server.on("/down", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    YO = (YO - 1) % LED_HEIGHT;
     fill_solid(leds, NUM_LEDS, bgColor);
    server.send(200, "text/html", "done");
    // i=0;
  });


  server.on("/bg", HTTP_GET, []() {
    //fill_solid(leds, NUM_LEDS, solidColor);
    //FastLED.show();
    String r = server.arg("r");

    String g = server.arg("g");
    String b = server.arg("b");
    bgColor = CRGB(r.toInt(), g.toInt(), b.toInt());
    calculghosts(bgColor) ;
    calculfraise(bgColor);
 calculpic(cerise, palec, bgColor, 14, 14,cerisecalc);
   calculpic(mario, palm, bgColor, 14, 17,mariocalc);
    fill_solid(leds, NUM_LEDS, bgColor);
    FastLED.show();
    server.send(200, "text/html", "done");
    // i=0;
  });

  server.on("/n", HTTP_GET, []() {
    fill_solid(leds, NUM_LEDS, solidColor);
    FastLED.show();
    String r = server.arg("led");
    NUM_LEDS_ECL = r.toInt();
    i = 0;
  });

  server.on("/l", HTTP_GET, []() {
    fill_solid(leds, NUM_LEDS, solidColor);
    FastLED.show();
    String r = server.arg("l");
    sperpentLength = r.toInt();
    i = 0;
  });


  server.on("/b", HTTP_GET, []() {
    String b = server.arg("r");

    FastLED.setBrightness(b.toInt());

    Serial.println(" in your bright");
    FastLED.show();

    server.send(200, "text/html", "done");
  });


  server.on("/test", HTTP_GET, []() {
    String r = server.arg("r");
    Serial.println("r=" + r);
    String g = server.arg("g");
    String b = server.arg("b");
    solidColor = CRGB(r.toInt(), g.toInt(), b.toInt());


    Serial.println(" in your ");
    fill_solid(leds, sperpentLength, solidColor);

    FastLED.show();

    server.send(200, "text/html", "done");
  });
  server.serveStatic("/", SPIFFS, "/index.html");

  server.serveStatic("/index.html", SPIFFS, "/index.html");
  
  server.serveStatic("/jquery.js", SPIFFS, "/jquery.js");
  
  server.serveStatic("/farbtastic.js", SPIFFS, "/farbtastic.js");
    
  server.serveStatic("/farbtastic.css", SPIFFS, "/farbtastic.css");
    
  server.serveStatic("/marker.png", SPIFFS, "/marker.png");
  
  server.serveStatic("/mask.png", SPIFFS, "/mask.png");
  
  server.serveStatic("/wheel.png", SPIFFS, "/wheel.png");
    server.serveStatic("/test.html", SPIFFS, "/test.html");

 server.serveStatic("/left.jpg", SPIFFS, "/left.jpg");
 server.serveStatic("/right.jpg", SPIFFS, "/right.jpg");
 server.serveStatic("/turn_left.png", SPIFFS, "/turn_left.png");
 server.serveStatic("/turn_right.png", SPIFFS, "/turn_right.png");
  //server.serveStatic("/turn_right.png", SPIFFS, "/turn_right.png");
 server.serveStatic("/images.css", SPIFFS, "/images.css");
    
//MDNS.addService("http","tcp",80);
//MDNS.addService("ws", "tcp", 81);
  server.begin();

   webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
    fill_solid(leds, NUM_LEDS, bgColor);
 // affichetext("TEXT", 0,0);
  //affichetext("12:13", 0,7);
delay(200);
    if(function=="tetris")
     newGame();
     // displaypic(mariocalc, 0 , 0, 14, 17);
   // FastLED.show();
     
}

void scrolllettre(int let, int ligne, int let2, int ligne2)
{

  for (int g = 0; g < taille[let]; g++)
  {
    //Serial.print("column:");
    //Serial.println(g);
    for (int x = 0 + ligne; x <= 4 + ligne; x++)
    {
      //Serial.print("line:");
      //Serial.println(x);
      int t;
      for (int m = 0; m < 30; m++)
      {
        t = 29 - m;
        if (x % 2 == 0) //ligne paire
          leds[t + 30 * x] = leds[t - 1 + 30 * x];
        else
          leds[60 * ((int)floor(x / 2) + 1) - 1 - t ] = leds[60 * ((int)floor(x / 2) + 1) - t ];
      }
      // Serial.print("valeur:");
      // Serial.println((lettre[let][x-ligne]));
      //Serial.print("mask:");
      //Serial.println(1<<(3-g));
      //Serial.println(!!(lettre[let][x-ligne] & (1<<(taille[let]-x-g))) );


      if ( ( (1 << (taille[let] - 1 - g)) &   lettre[let][4 - x + ligne] ) > 0 )
      {
        if (x % 2 == 0)
        {
          leds[0 + 30 * x] = Color;
          //  Serial.print("on ecrit:");
          // Serial.println(30*x);
        }
        else
        {
          leds[60 * ((int)floor(x / 2) + 1) - 1] = Color;
          //Serial.print("on ecrit:");
          //Serial.println(60*((int)floor(x/2)+1)-1);
        }
      }
      else
      {
        if (x % 2 == 0)
          leds[0 + 30 * x] = bgColor;
        else
          leds[60 * ((int)floor(x / 2) + 1) - 1] = bgColor;
      }
    }
    //tab[i+1]=(tab[i+1]<<1) | (!!(lettre[let][i-ligne] & (1<<(taille[let]-1-g))) & coul );
    //tab2[i+1]=(tab2[i+1]<<1)| (!!(lettre[let][i-ligne] & (1<<(taille[let]-1-g)))& !!(coul & 2) );
  }
  //delay(delai);

}





void affichelettre(int let,int x0,int y0)
{

int taille=5;
int hauteur=7;
if (x0>=LED_WIDTH or y0 >=LED_HEIGHT )
  return;
    
       // Serial.print(x0);
         // Serial.print(":");
        //Serial.println(y0);
  for(int x=x0;x<taille+x0;x++)
  {
        //Serial.print("column:");
        //Serial.println(x-y0);
      for (int y=y0;y<hauteur+y0;y++)
      {
        //Serial.print("ligne:");
        //Serial.println(y-y0);
         //Serial.println((1 << (x0-x+taille-1)));
           if ( ( (1 << (x0-x+taille-1)) &   font[let][hauteur-1 - y + y0] ) > 0 )
          {
            if (y % 2 == 0)
            {
              leds[(29-x) + 30 * y] = Color;
           //    Serial.print("on ecrit:");
            //  Serial.println((-x+29) + 30 * y);
            }
            else
            {
              leds[60 * ((int)floor(y / 2) + 1) - 1-(-x+29)] = Color;
              //Serial.print("on ecrit:");
              //Serial.println(60 * ((int)floor(y / 2) + 1) - 1-(-x+29));
            }
          }
          else
          {
            if (y % 2 == 0)
            {
              leds[(-x+29) + 30 * y]  = bgColor;
              //Serial.print("on ecrit pas:");
              //Serial.println((-x+29) + 30 * y);
            }
            else
            {
              //Serial.print("on ecrit pas:");
              //Serial.println(60 * ((int)floor(y / 2) + 1) - 1-x+29);
              leds[60 * ((int)floor(y / 2) + 1) - 1-(-x+29)] = bgColor;
            }
          }
      }


    
  }

}


void affichetext(String text, int x,int y)
{
  int taille=6;
 int off=0;

  for (int i=0;i<text.length();i++)
  {
    int afx=x+off;
    //on inverse les X 0->29 et 29->0
   
   //Serial.println(text[i]);
   //Serial.println((int)text[i]);
   affichelettre((int)text[i]-32,afx, y);
    
    off+=taille;
  }

}



void pix(int x, int y, CRGB color)
{

  int ledTolight = 0;
  if (y % 2 == 0)
  {

    ledTolight = LED_WIDTH * y + x;
  }
  else
  {
    ledTolight = 2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x;

  }

  leds[ledTolight] = color;
}


void scrollScreenX(int x)
{
  for (int y = 0; y < LED_HEIGHT; y++)
  {
    for (int m = 0; m <= LED_WIDTH - x; m++)
    {
      int t = LED_WIDTH - 1 - m;
      if (y % 2 == 0) //ligne paire
        leds[t + LED_WIDTH * y] = leds[t - x + LED_WIDTH * y];
      else
        leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - t ] = leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - t + x ];
    }
  }
}




void loop() {








webSocket.loop();


  if (function == "pacman")
  {

    k = k % 160;
    int offset = -20;
    fill_solid(leds, NUM_LEDS, bgColor);
    displaypic(cerisecalc, k + offset , YO, 14, 14);
    displaypic(fraiseCalc, k + offset - 14 , YO + 2, 11, 10);
    displaypic(ghostred, k + offset - 30, YO, 14, 14);
    displaypic(ghostyellow, k + offset - 46, YO, 14, 14);
    displaypic(ghostgreen, k + offset - 62, YO, 14, 14);
    displaypic(ghostPurple, k + offset - 78, YO, 14, 14);
    displaypic(ghostCyan, k + offset - 94, YO, 14, 14);
    displaypic(mariocalc, k + offset-110 , YO, 14, 17);
     //mariocalc = calculpic(mario, palm, solidColor, 17, 14);
    k++;

    //Serial.println("on rafraichit");
    FastLED.show();
    FastLED.delay((int)(1000 / refresh) + 1);
     
  }


  if (function ==  "scroll")
  {

    int offset3 = 0;
    int offset2 = 0;
    k2 = k2 % lens;
    int ascd = (int)mess[k2 % lens];
    //Serial.println(ascd);
    if (ascd >= 65)
    {
      offset3 = 65 - 3;
    }
    else
    {
      if (ascd>=48)
     {
        offset3=48-1;
     }
     else
        offset3 = 32;
    }

    int ascd2 = (int)mess2[k2 % lens];
    if (ascd2 >= 65)
    {
      offset2 = 65 - 8;
    }
    else
    {
      offset2 = 32;
    }

    scrolllettre(ascd - offset3, YO, ascd2 - offset2, 7);
    k2++;
    FastLED.show();
    FastLED.delay((int)(1000 / refresh) + 1);
    server.handleClient();
  }


  if(function=="meteo")
  {
       //YO=5;
       
    server.handleClient(); 
           
    //function="scroll";
  }

 if(function=="affiche")
 {
    fill_solid(leds, NUM_LEDS, bgColor);
  affichetext("TEXT", 0,0);
     FastLED.show();
      FastLED.delay((int)(1000 / refresh) + 1);
    server.handleClient();  
 }


if(function=="tetris")
{
 server.handleClient();  

  

}
 
  // put your m ain code here, to run repeatedly:

  //FastLED.show();
  //fill_solid(leds, 20, solidColor);
  // insert a delay to keep the framerate modest
  //FastLED.delay(int(1000 / 120));


  
}
