/*
   XWLGFX lib example 2019

   Enjoy!
*/
#define FASTLED_ALLOW_INTERRUPTS 0
#define ESP_I2S 1
#include "FastLED.h"
#include "XWLGFX.h"
#ifndef ESP_I2S
#include "I2S.h"
#endif

#define NUM_OUT 16
//#define NUM_LED_PER_OUT 118
#define NUM_LED_PER_OUT 369
#define MAX_LED_OUT NUM_OUT * NUM_LED_PER_OUT

boolean useRMT = true; //to switch between sams (true) an new Yves drivers

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define LED_PIN1    2
#define LED_PIN2    4
#define LED_PIN3    5
#define LED_PIN4    12

#define LED_PIN5    13
#define LED_PIN6    14
#define LED_PIN7    15
#define LED_PIN8    16
//--------//--------
#define LED_PIN9     17
#define LED_PIN10    18
#define LED_PIN11    19
#define LED_PIN12    21

#define LED_PIN13    22
#define LED_PIN14    23 //25
#define LED_PIN15    25 //33
#define LED_PIN16    26 //32

XWLGFX gfx;

CRGB out_leds [MAX_LED_OUT];
uint8_t params[20];
int Pins[16]={2,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26};
#ifndef ESP_I2S
I2S controller(0);
#endif

Fixture *fixture;
uint8_t numFixture;

void setup() {
    Serial.begin(115200);
    if(useRMT)
    {
        Serial.println("we do RMT");
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
    } else {
        Serial.println("we do I2S");
        #ifndef ESP_I2S
       controller.initled(out_leds,Pins,NUM_OUT,NUM_LED_PER_OUT);
       controller.setBrightness(255);
       #endif
    }
    fill_solid(out_leds,MAX_LED_OUT,CRGB::Black);
    showLEDS();
    delay(1000);

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
  //gfx.begin(NUM_OUT, NUM_LED_PER_OUT, 10, 1);
    //gfx.begin(9, 6, 4, 6); //For LED PAR
    //For Yves setup
    gfx.begin(48, 123, 1, 1);
    //gfx.begin(48, 123, 2, 2); //Double resolution
    addFixture();
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
  params[4] = 75;    //Scene Preset (Implemented for "Fire")
  params[5] = 0;  //Color red       Coming soon...
  params[6] = 0;  //Color green  
  params[7] = 255;  //Color blue
  params[8] = 60;   //16-bit scene speed by BPM
  params[9] = 0;    //Fractional part 1/256
  //Add effect
  params[10] = 15;  //fx - glitter
  params[11] = 200; //fx bri
  params[12] = 1;   //fx speed BPM. don't use

}

void showLEDS() {
    if(useRMT)
        FastLED.show();
    else
    {
      #ifndef ESP_I2S
       controller.showPixels();
       #endif
    }
  
}

void loop() {
    EVERY_N_MILLISECONDS(1000/30) {
       // Serial.println();
        long lastHandle = ESP.getCycleCount();
        showLEDS();
        Serial.printf("FPS led show: %f \n",(float)240000000/(ESP.getCycleCount() - lastHandle));
        long lastHandle2 = ESP.getCycleCount();
        gfx.handle(params);
       // Serial.printf("HandleTime: %f us\n",(float)(ESP.getCycleCount() - lastHandle2)/240);
        lastHandle = ESP.getCycleCount();
        gfx.getData(out_leds, fixture, numFixture);
        //Serial.printf("GetTime: %f us\n",(float)(ESP.getCycleCount() - lastHandle2)/240);
       // Serial.printf("Total FPS: %f\n",(float)240000000/(ESP.getCycleCount() - lastHandle));
        //gfx.print(*gfx.mainBuffer, NUM_OUT, MAX_LED_OUT);
    }
}

void addFixture(){
    //XWL STRIP
    numFixture = 1;
    //MappingLayer layer[1] = { MappingLayer(0, 0, 159, 117, 0, 0, 15, 117) };
    //MappingLayer layer[1] = { MappingLayer(0, 0, 5, 5, 0, 0, 2, 2) };
    //Yves
    MappingLayer *layer = new MappingLayer(0, 0, 47, 122, 0, 0, 47, 122);
    //MappingLayer *layer = new MappingLayer(0, 0, 95, 245, 0, 0, 47, 122); //Double resolution

    fixture = new Fixture(layer, MappingType::ZIGZAG_UD_R);
    //fixture = new Fixture(layer);
    //fixture = new Fixture(layer, numFixture);
}

