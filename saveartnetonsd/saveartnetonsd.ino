#include <WiFi.h>
//#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <Artnet.h>
/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0


#define LED_WIDTH 123
#define LED_HEIGHT 48
#define NUM_LEDS LED_WIDTH*LED_HEIGHT
#define UNIVERSE_SIZE 170
#define PIXEL_PER_PIN NUM_LEDS/5 
/*we will  use 5 pins each pin will drive two 16x16 panel hence 512 pix  it will take 15ms to refresh your entire panel instead of 76ms
hence having a refresh rate of 65fps instead of 13fps*/
#define PIN1 2
#define PIN2 4
#define PIN3 27
#define PIN4 12
#define PIN5 13
//CRGB leds[NUM_LEDS];
static uint8_t frame[NUM_LEDS*3]; //transfer files

char filename[256];
char SAVE_NAME[]="savedata"; //the name of your save
Artnet artnet;

File root;
static File  myFile ;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;

static uint32_t deb=0;
static uint32_t fin=0;



void saveframes()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
       // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle2);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}



void FastLEDshowTask2(void *pvParameters)
{
  //Serial.printf("djdj\n");

  
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
            
           
               // memcpy(leds,Tpic,LED_WIDTH*LED_HEIGHT*sizeof(CRGB));
  fin=millis();
     memcpy(frame,artnet.getframe(),NUM_LEDS*sizeof(CRGB));
    
            myFile.write(frame,NUM_LEDS*sizeof(CRGB));
           
            deb=deb+millis()-fin;
               userTaskHandle=0; //so we can't have two display tasks at the same time
                 
           }
}

uint16_t compteur=0;
void setup() {

  
Serial.begin(115200);

 if(!SD.begin(5,SPI,80000000)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
  memset(filename, 0, 255);
    sprintf(filename,"/%s.dat",SAVE_NAME);
  myFile = SD.open(filename, FILE_WRITE);

  
   WiFi.mode(WIFI_STA);
    
    Serial.printf("Connecting ");
   WiFi.begin("WiFi-2.4-E19C", "yvesyves");

    while (WiFi.status() != WL_CONNECTED) {
      Serial.println(WiFi.status());
   
      
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    
  // myFile = SD.open(filename, FILE_WRITE);
   
    xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 3000, NULL,1, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
    
    
 /* 
  FastLED.addLeds<WS2812B, PIN1, GRB>(leds, 0*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN2, GRB>(leds, 1*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN3, GRB>(leds, 2*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN4, GRB>(leds, 3*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN5, GRB>(leds, 4*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  `
  */
artnet.begin(NUM_LEDS,UNIVERSE_SIZE,3); //the number of pixels and the maximum size of your iniverses
 Serial.println("Ready to record...");

}
bool stopped=false;
void loop() {
      while(artnet.nbframeread<5000) //lenth of video
  {
      artnet.read(); 
      if(artnet.nbframeread==0)
          Serial.println("Recording has started");
     
       
      //memcpy(frame,artnet.getframe(),NUM_LEDS*sizeof(CRGB));
     saveframes();
     
     // artnet.resetsync();
      //compteur++;
  }
  if(stopped==false)
  {
  myFile.close();
     Serial.println("Recording stopped ...");
     Serial.printf("frames saved:%d frames lues:%d\n",artnet.nbframeread,artnet.frameslues)   ; 
     Serial.printf("time per frame %f\n",(float)deb/5000);
stopped=true;
  }
  // put your main code here, to run repeatedly:

}
