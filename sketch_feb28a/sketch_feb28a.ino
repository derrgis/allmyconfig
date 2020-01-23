//_______________________________________________
//parallel data output on ESP32

//strip / GPIO
//1 12
//2 13
//3 14
//4 15
//5 16
//6 17
//7 2
//8 4

// -- The core to run FastLED.show() 
#define FASTLED_ALLOW_INTERRUPTS 0 
#include "FastLED.h" 
#define FASTLED_USING_NAMESPACE 
#define FASTLED_SHOW_CORE 0

#define NUM_STRIPS 8 // Ici j'utilise 8 strips 
#define NUM_LEDS_PER_STRIP 60

// 8 strips enabled on GPIO 2, 4 12, 13 ,14 ,15, 16, 17 - bit mask right to left
#define PORT_MASK 0b0000000000000011111100000001010

/*THIS IS THE NEW POINT THIS MASK REFER TO THE PIN O(lowest bit) TO 31 (significant bit) 
 if you want 5 STRIPS on pins 2 6 12 16 19 the MASK will look like THIS
 0b1001000100000100010 set NUM_STRIPS to 11

*/ 
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP 
CRGB leds[NUM_LEDS];
  static uint8_t hue = 0; 
// -- Task handles for use in the notifications 
static TaskHandle_t FastLEDshowTaskHandle = 0; 
static TaskHandle_t userTaskHandle = 0;

void FastLEDshowESP32() { 
  if (userTaskHandle == 0) { 
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); 

// -- Store the handle of the current task, so that the show task can notify it when it's done 
  userTaskHandle = xTaskGetCurrentTaskHandle();

  // -- Trigger the show task 
  xTaskNotifyGive(FastLEDshowTaskHandle);
  
  // -- Wait to be notified that it's done 
  ulTaskNotifyTake(pdTRUE,portMAX_DELAY); userTaskHandle = 0; 
  } 
}

void FastLEDshowTask(void *pvParameters) { 
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500); 
  // -- Run forever... for(;;) { 
  // -- Wait for the trigger 
  ulTaskNotifyTake(pdTRUE,portMAX_DELAY);

  // -- Do the show (synchronously) 
  FastLED.show();
  
  // -- Notify the calling task 
  xTaskNotifyGive(userTaskHandle); 
  }
 

//Then add code to initialize the task in your setup() function:
void setup(){ 
  // -- Create the FastLED show task 
  xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);

//-- Initiate the Leds.
/* two choices use the PORT_MASK or use the hardcoded FOR Harcoded just PORT_MASK=0 
it's a bit faster because i have hardcoded the way the bit are encoded
when you use the PORT_MASK it's a bit slower bit still good you can use two setups 
WS2811_PORTA (less artifacts with hardcoded pins) or WS2812B_PORTA a new one a bit slower 
i have better result for non hardcoded up to you to test the different combination. */

//FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,PORT_MASK>(leds, NUM_LEDS_PER_STRIP); //
//OR

FastLED.addLeds<WS2812B_PORTA,NUM_STRIPS,PORT_MASK>(leds, NUM_LEDS_PER_STRIP); //

}

//Finally, call the new show function in place of regular show:

void loop(){

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
  FastLEDshowESP32(); // FastLED.show(); 
} 

