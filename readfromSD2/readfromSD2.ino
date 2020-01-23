
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
#define FA
#define NUM_STRIPS 16
 
#include "SD.h"
#include "SPI.h"
#define FASTLED_ALLOW_INTERRUPTS 0
//#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
#include "Animation.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0

#define PORT_MASK 0b110111011111111000000110100

 //0b1001111111000000111101
//#define PORT_MASK   0b000000000000000000
//#define NUM_STRIPS 16
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
#define PIN1 2
#define PIN2 4
#define PIN3 5
#define PIN4 12
#define PIN5 13
CRGB leds[NUM_LEDS];
uint8_t readbuffer[NUM_LEDS*3];
char filename[256];
char READ_NAME[]="savedata2"; //the name of your save
//Artnet artnet;

File root;
File  myFile;
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;



class myAnimation2 : public Animation
 {
   private:
    int k;
   public:
    using Animation::Animation;
    //myAnimation ( CRGB *ext_leds,int start_led,int end_led):Animation(  ext_leds ,start_led,end_led,&this->animation);
    void setup()
    {
      k=300;
    }
    void animation() override
    {
      //Serial.println("ee");
      for(int i=0;i<=num_leds;i++)
      {
        ledsa[i]=CHSV((i+k)*255/num_leds,255,255);
      }  
      k=(k+10)%num_leds;
    } 
    
 };
 
class myAnimation : public Animation
 {
   private:
    int k;
   public:
    using Animation::Animation;
    //myAnimation ( CRGB *ext_leds,int start_led,int end_led):Animation(  ext_leds ,start_led,end_led,&this->animation);
    void setup()
    {
      k=0;
    }
    void animation() override
    {
      fill_solid(ledsa,num_leds,CRGB::Green);
      //Serial.println("ee");
      for(int i=0;i<=k;i++)
      {
        ledsa[i]=CRGB::Red;
      }  
      
      k=(k+1)%num_leds;
    } 
    
 };

 myAnimation yves=myAnimation(leds,20,500);
myAnimation2 yves2=myAnimation2(leds,1000,2400);
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
SPI.begin(33,35,32,27);
    
    if(!SD.begin(27,SPI,80000000)){

        Serial.println("Card Mount Failed");
        return;
    }
    Serial.println("SD found");
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    memset(filename, 0, 255);
    sprintf(filename,"/%s.dat",READ_NAME);
 myFile = SD.open(filename);
 if(!myFile)
    Serial.println("no file found");

  FastLED.setBrightness(64 );
  Serial.println(myFile.size());

    
  /*
  FastLED.addLeds<WS2812B, PIN1, GRB>(leds, 0*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN2, GRB>(leds, 1*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN3, GRB>(leds, 2*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN4, GRB>(leds, 3*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  FastLED.addLeds<WS2812B, PIN5, GRB>(leds, 4*PIXEL_PER_PIN, PIXEL_PER_PIN); 
  `
  
*/
 yves.setup();
 yves2.setup();
}

void loop() {
  fill_solid(leds,NUM_LEDS,CRGB::Blue);
  yves.loop();
 yves2.loop();
  FastLEDshowESP32();
 // artnet.read(); 

 
 
    // Serial.println("srart");
 
/*
  if (myFile.available())
  {
   // Serial.println("ee");
    myFile.read(readbuffer,NUM_LEDS*sizeof(CRGB));
    memcpy(leds,readbuffer,NUM_LEDS*sizeof(CRGB));
    Serial.println("Reading Frame");
    FastLEDshowESP32();
   //     FastLED.show();
//replaceled();
    
       // FastLED.show();
    delay(10); //this is to be changed to adapt the replay
  }
  else
  {
    myFile.seek(0);
  }


*/
 // artnet.resetsync();
 
  // put your main code here, to run repeatedly:

}
