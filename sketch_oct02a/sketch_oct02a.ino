
#define NUM_STRIPS 8
#define FastMask(MASK) ((MASK & 1) << 2 ) | ((MASK & 6) << 3 ) | ((MASK & 8) << 12 ) | ((MASK & 48) << 14 ) | ((MASK & 64) << 15 ) | ((MASK & 128) << 16 )

#define FASTLED_ALLOW_INTERRUPTS 0
#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0
#define PORT_MASK 0b0101011001000000000110100
#define NUM_LEDS_PER_STRIP 300
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;

CRGB leds[NUM_STRIPS*NUM_LEDS_PER_STRIP];

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
        
      time3=ESP.getCycleCount();
    
        FastLED.show();
        Serial.printf("FPS:%f\n",(float)(240000000/(ESP.getCycleCount()-time3)));

    
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
        
            
            
           FastLED.show();
            
               userTaskHandle=0; //so we can't have two display tasks at the same time
                 
           }
}


void setup() {
Serial.begin(115200);
  xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2000, NULL,2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
  xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 2000, NULL,3, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
  // put your setup code here, to run once:
    FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,PORT_MASK>(leds, NUM_LEDS_PER_STRIP);

}

void loop() {
  // put your main code here, to run repeatedly:

  // you can use
  FastLEDshowESP32(); //instead of fastled.show() this one will wait till the fastledshow is done before giving you back the hand

  //you can use this one
  FastLEDshowESP322();  //instead of fastled.show()  this one will give you the hand right away (i use this one for arnet reading this way i can retrieve the packet while displaying using the second core

}
