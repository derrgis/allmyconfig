/*
   XWLGFX lib example 2019

   Enjoy!
*/
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include "XWLGFX.h"

#define NUM_OUT 16
#define NUM_LED_PER_OUT 118
#define MAX_LED_OUT NUM_OUT * NUM_LED_PER_OUT

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define LED_PIN1    2
#define LED_PIN2    0
#define LED_PIN3    4
#define LED_PIN4    5

#define LED_PIN5    18
#define LED_PIN6    19
#define LED_PIN7    23
#define LED_PIN8    15
//--------//--------
#define LED_PIN9     13
#define LED_PIN10    12
#define LED_PIN11    14
#define LED_PIN12    27

#define LED_PIN13    26
#define LED_PIN14    25 //25
#define LED_PIN15    33 //33
#define LED_PIN16    32 //32

XWLGFX gfx;

CRGB out_leds [MAX_LED_OUT];
uint8_t params[20];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(out_leds, 0, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(out_leds, NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(out_leds, 2 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(out_leds, 3 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<LED_TYPE, LED_PIN5, COLOR_ORDER>(out_leds, 4 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN6, COLOR_ORDER>(out_leds, 5 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN7, COLOR_ORDER>(out_leds, 6 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN8, COLOR_ORDER>(out_leds, 7 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<LED_TYPE, LED_PIN9, COLOR_ORDER>(out_leds, 8 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN10, COLOR_ORDER>(out_leds, 9 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN11, COLOR_ORDER>(out_leds, 10 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN12, COLOR_ORDER>(out_leds, 11 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<LED_TYPE, LED_PIN13, COLOR_ORDER>(out_leds, 12 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN14, COLOR_ORDER>(out_leds, 13 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN15, COLOR_ORDER>(out_leds, 14 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, LED_PIN16, COLOR_ORDER>(out_leds, 15 * NUM_LED_PER_OUT, NUM_LED_PER_OUT).setCorrection(TypicalLEDStrip);
  /*
     Begin gfx:
     1. width
     2. height
     Super Resolution modificator
     3. scale width. default 2
     4. scale height. default 2
  */
  //gfx.begin(NUM_OUT, NUM_LED_PER_OUT);
  //Custom
  gfx.begin(NUM_OUT, NUM_LED_PER_OUT, 10, 1);
  /*
   * Options for GFX. Let the library know what you want :)
   */
  params[0] = 100; //Manual mode. At the moment, only he.
  params[1] = 255; //Global Brightness 
  /*
   * Scene (Not all scene functions are implemented.Not all scene functions are implemented):
   * The basis of scenes is the code from DemoReel100 by Mark Kriegsman:
   * 15 - fire
   * 25 - bpm 
   * 35 - confetti 
   * Demonstration of Super Resolution features
   * Change speed to feel the power! :) 
   * 65 - lines               
   * 75 - circles
   */
  params[2] = 75;  //scene 
  params[3] = 255; //Scene Brightness
  params[4] = 5;    //Scene Preset (Implemented for "Fire")
  params[5] = 255;  //Color red       Coming soon...
  params[6] = 255;  //Color green  
  params[7] = 255;  //Color blue
  params[8] = 60;   //16-bit scene speed by BPM
  params[9] = 0;    //Fractional part 1/256
  //Add effect
  params[10] = 15;  //fx - glitter
  params[11] = 200; //fx bri
  params[12] = 1;   //fx speed BPM. don't use
}

void loop() {
  EVERY_N_MILLISECONDS(1000/30) {
    FastLED.show();
    static uint8_t lastHandle = millis();
    gfx.handle(params);
    Serial.print("HandleTime: ");
    Serial.print(millis() - lastHandle);
    Serial.println("us");
    lastHandle = millis();
    gfx.getData(out_leds);
    Serial.print("GetTime: ");
    Serial.print(millis() - lastHandle);
    Serial.println("us");

    //gfx.print(*gfx.mainBuffer, NUM_OUT, MAX_LED_OUT);
  }
}
