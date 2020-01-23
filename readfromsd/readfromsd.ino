
#include <SPI.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS 
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
//#include "SD.h"
#include "SPI.h"
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0

#define PORT_MASK 0b110111011111111000000110100  //0b1001111111000000111101
//#define PORT_MASK   0b000000000000000000
#define NUM_STRIPS 16
#define PINS_OUTPUT  0,1,2,4,5,14,23,25,26,21,22,16


#define LED_WIDTH 123
#define LED_HEIGHT_PER_STRIP 3
#define LED_HEIGHT NUM_STRIPS*LED_HEIGHT_PER_STRIP
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP
#define NUM_LEDS_PER_STRIP LED_HEIGHT_PER_STRIP*LED_WIDTH
//#define LED_HEIGHT 32
#define UNIVERSE_SIZE 170

/*we will  use 5 pins each pin will drive two 16x16 panel hence 512 pix  it will take 15ms to refresh your entire panel instead of 76ms
hence having a refresh rate of 65fps instead of 13fps*/

CRGB leds[NUM_LEDS];
uint8_t readbuffer[NUM_LEDS*3];
char filename[256];
char READ_NAME[]="savedata"; //the name of your save
//Artnet artnet;

const uint8_t SOFT_MISO_PIN = 27;
const uint8_t SOFT_MOSI_PIN = 32;
const uint8_t SOFT_SCK_PIN  = 33;
//
// Chip select may be constant or RAM variable.
const uint8_t SD_CHIP_SELECT_PIN =0;

// SdFat software SPI template
SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

// Test file.
SdFile file;
SdFile file2;
//File root;
//File  myFile;
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;

void replaceled()
{
  int offset=0;
  for(int i=0;i<123;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s;
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 offset=24*LED_WIDTH;
  for(int i=0;i<24*LED_WIDTH;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s; 
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 //on met les boards en noir

 for (int i=0;i<LED_HEIGHT;i++)
 {
  leds[i*LED_WIDTH]=CRGB::Black;
  leds[(i+1)*LED_WIDTH-1]=CRGB::Black;
 }
}

void FastLEDshowESP32()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
       // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle);

        // -- Wait to be notified that it's done
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS( 500 ));//portMAX_DELAY);
        //delay(100);
        //interrupts();
        userTaskHandle = 0;
    }
}



void FastLEDshowTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
replaceled();
    
        FastLED.show();

    
        xTaskNotifyGive(userTaskHandle);
    }
}


void setup() {

  
Serial.begin(115200);
   xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 1000, NULL,3, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
  /* WiFi.mode(WIFI_STA);
    
    Serial.printf("Connecting ");
    WiFi.begin("", "");

    while (WiFi.status() != WL_CONNECTED) {
      Serial.println(WiFi.status());
   
      
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
*/


  FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,PORT_MASK>(leds, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(64 );
    if (!sd.begin(SD_CHIP_SELECT_PIN)) {
    sd.initErrorHalt();
  }

 

    memset(filename, 0, 255);
    sprintf(filename,"%s.dat",READ_NAME);
// myFile = SD.open(filename);
    
     if (!file.open(filename)) {
    sd.errorHalt(F("open failed"));
  } 
  Serial.println("goo");
  

 
}

void loop() {
 // artnet.read(); 

 
 
     
 

  if (file.available())
  {
    file.read(readbuffer,NUM_LEDS*sizeof(CRGB));
    memcpy(leds,readbuffer,NUM_LEDS*sizeof(CRGB));
    FastLEDshowESP32();

   // delay(20); //this is to be changed to adapt the replay
  }
  else
  {
    file.rewind();
  }



 // artnet.resetsync();
 
  // put your main code here, to run repeatedly:

}

#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif  //ENABLE_SOFTWARE_SPI_CLASS
