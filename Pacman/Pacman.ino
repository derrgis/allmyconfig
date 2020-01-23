#include "Wire.h"
#define LED_WIDTH 123
#define LED_HEIGHT_PER_STRIP 3
#define LED_HEIGHT NUM_STRIPS*LED_HEIGHT_PER_STRIP
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP
#define NUM_LEDS_PER_STRIP LED_HEIGHT_PER_STRIP*LED_WIDTH
#define NUM_STRIPS 16

#define ESP32_VIRTUAL_DRIVER true
#define ESP_VIRTUAL_DRIVER_82_2 1
#define NBIS2SERIALPINS 2
//#define STATIC_COLOR_BGR0100 1
#define STATIC_COLOR_PER_PIN 1
#define COLOR_2_RGB 1
//#define ESP32_VIRTUAL_DIVER true
//#define NBIS2SERIALPINS 4
//#define FASTLED_ESP32_I2S 1
#include "FastLED.h"
//FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0

//TwoWire d(0);
#define LATCH_PIN 13
#define CLOCK_PIN 27

int Pins[16]={12,14};
#include "NesController.h"
NesController nes;


#include "PacmanClass.h"

int dlock=0;
CRGB leds[NUM_LEDS];
CRGB Tpic[NUM_LEDS];
_nespad nespad;
Pacman new_pac(123,48,leds);




static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;



void replaceled()
{
  //return;
   
 // return;
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
 
 offset=34*LED_WIDTH+54+4;
 for(int i=0;i<3;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s; 
 }
 //on met les boards en noir
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
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //delay(100);
        //interrupts();
        userTaskHandle = 0;
    }
}


void FastLEDshowESP322()
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
static long time3=ESP.getCycleCount();
void FastLEDshowTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
     // time3=ESP.getCycleCount();
    //replaceled();
         //controller.showPixels(); /
     FastLED.show();
       // Serial.printf("FPS:%f\n",(float)(240000000/(ESP.getCycleCount()-time3)));

    
        xTaskNotifyGive(userTaskHandle);
    }
}


void FastLEDshowTask2(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
            /*
            if (anim==0)
             //  int i=0;
                     // displayPicNew(artnet.getframe(),0,0,48,123); //for the artnet
                       memcpy(leds,artnet.getframe(),123*48*sizeof(CRGB));
            else
                memcpy(leds,Tpic,123*48*sizeof(CRGB));
            
            replaceled();
            //delay(1),
            //controller.showPixels(); */
            FastLED.show();
            
               userTaskHandle=0; //so we can't have two display tasks at the same time
                 
           }
}



void setup() {
 // d.begin();
  // put your setup code here, to run once:
  Serial.begin(115200);
   xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 1024, NULL,2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
  Serial.printf("Pacman %d %d\n",new_pac.width,new_pac.height);
 // delay(3000);
while(!nes.begin())
{
  Serial.println("connectng nes");
}
 new_pac.start();
 // xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 2000, NULL,3, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
FastLED.addLeds<VIRTUAL_DRIVER,Pins,CLOCK_PIN, LATCH_PIN>(leds,NUM_LEDS_PER_STRIP);
new_pac.displayPacman();
//Serial.printf("%d %d\n",pacmanPosition.px,pacmanPosition.py);
delay(100);
FastLEDshowESP32();
FastLEDshowESP32();
new_pac.tempdirectionpac=RIGHT;
}

void neshow()
{
  if(new_pac.notdisplay){
    dlock++;
    if(dlock>10)
    {
    new_pac.notdisplay=false;
    dlock=0;
    }
    return;
    }
   new_pac.displayPacman();
 replaceled();
        //FastLED.show();
        FastLEDshowESP32();
}

void loop() {
  
  //Serial.printf("ll %d\n",new_pac.pacmanPosition.py);
  // put your main code here, to run repeatedly:
//delay(100);
    
    nespad.used=false;
nespad=nes.getpad();
if(nespad.used)
{

//Serial.println("ee");    
/*
    if(nespad.up)
    {
      //(pacmanPosition.py+2)%4>0
      for(int i=0;i<4;i++)
      {
       new_pac.moveup();
     
       
        neshow();
        delay(5);
      }
    }
    if(nespad.down)
    {
    //(pacmanPosition.py+2)%4>0
     for(int i=0;i<4;i++)
      {
       new_pac.movedown();
      
       
        neshow();
        delay(5);
      }
    }
    if(nespad.left)
    {
      //(pacmanPosition.py+2)%4>0
       for(int i=0;i<4;i++)
      {
       new_pac.moveleft();
      
       
        neshow();
       delay(5);
      }
    }
    if(nespad.right)
    {
    //(pacmanPosition.py+2)%4>0
     for(int i=0;i<4;i++)
      {
       new_pac.moveright();
      
       
        neshow();
      delay(5);
      }
    }
    */
if(nespad.up)
    {
      new_pac.tempdirectionpac=UP;
    }
if(nespad.left)
    {
      new_pac.tempdirectionpac=LEFT;
    }
 if(nespad.down)
    {
      new_pac.tempdirectionpac=DOWN;
    }   
if(nespad.right)
    {
      new_pac.tempdirectionpac=RIGHT;
    }    
    nespad.used=false;
    //Serial.printf("direction %d\n",new_pac.tempdirectionpac);
    //Serial.printf("direction:%d px:%d x:%d y:%d mod:%d tile x-1:%d\n",new_pac.Blinky.direction,new_pac.Blinky.px,(new_pac.Blinky.px+2)/4,(new_pac.Blinky.py+2)/4,(new_pac.Blinky.px+2)%4,new_pac.getTile((new_pac.Blinky.px+2)/4-1,(new_pac.Blinky.px+2)%4));
}
else
{
  new_pac.pacmanDirection.vy=0;
  new_pac.pacmanDirection.vx=0;
}
//Serial.printf("direction:%d px:%d x:%d y:%d mod:%d tile x-1:%d\n",new_pac.Blinky.direction,new_pac.Blinky.px,(new_pac.Blinky.px+2)/4,(new_pac.Blinky.py+2)/4,(new_pac.Blinky.px+2)%4,new_pac.getTile((new_pac.Blinky.px+2)/4-1,(new_pac.Blinky.px+2)%4));
 neshow();




}
