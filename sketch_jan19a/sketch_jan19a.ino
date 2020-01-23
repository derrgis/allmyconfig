
// -- The core to run FastLED.show()
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0



#define NUM_STRIPS 11  //from 1 to 16
#define NUM_LEDS_PER_STRIP  200

#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP
CRGB leds[NUM_LEDS];


// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle = 0;


void FastLEDshowESP32()
{
if (userTaskHandle == 0) {
const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
// -- Store the handle of the current task, so that the show task can
//    notify it when it's done
userTaskHandle = xTaskGetCurrentTaskHandle();

// -- Trigger the show task
xTaskNotifyGive(FastLEDshowTaskHandle);

// -- Wait to be notified that it's done
ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
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

// -- Do the show (synchronously)
FastLED.show();

// -- Notify the calling task
xTaskNotifyGive(userTaskHandle);
}

}
//Then add code to initialize the task in your setup() function:

void setup(){
// -- Create the FastLED show task
xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);

//-- Initiate the Leds.
FastLED.addLeds<WS2811_PORTA,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);



}


//Finally, call the new show function in place of regular show:


void loop(){

static uint8_t hue = 0;
for(int i = 0; i < NUM_STRIPS; i++) {
for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
}
}

// Set the first n leds on each strip to show which strip it is
for(int i = 0; i < NUM_STRIPS; i++) {
for(int j = 0; j <= i; j++) {
leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
}
}

hue++;

// send the 'leds' array out to the actual LED strip
FastLEDshowESP32();
// FastLED.show();
}

