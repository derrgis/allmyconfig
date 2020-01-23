#define FASTLED_ESP32_I2S 1
#include "FastLED.h"

#define DATA_PIN    13
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    68 //62
#define TRAVELSPEED 75 // Lower = faster
#define TAIL_LENGTH 1 // Lower = longer
#define WHITE_HEAD 0 // 1 = White headed

int pos = NUM_LEDS-1;
int hue = 0;

int fadeAmount = 5; 
int brightness = 0;       

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS];
CRGB leds5[NUM_LEDS];
CRGB leds6[NUM_LEDS];
CRGB leds7[NUM_LEDS];
CRGB leds8[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE,26,COLOR_ORDER>(leds, NUM_LEDS); //Strip 4
  FastLED.addLeds<LED_TYPE,25,COLOR_ORDER>(leds2, NUM_LEDS); //Strip 3
  FastLED.addLeds<LED_TYPE,33,COLOR_ORDER>(leds3, NUM_LEDS); //Strip 2
  FastLED.addLeds<LED_TYPE,32,COLOR_ORDER>(leds4, NUM_LEDS); //Strip 1
  FastLED.addLeds<LED_TYPE,12,COLOR_ORDER>(leds5, NUM_LEDS); //Strip 5
  FastLED.addLeds<LED_TYPE,13,COLOR_ORDER>(leds6, NUM_LEDS); //Strip 6
  FastLED.addLeds<LED_TYPE,33,COLOR_ORDER>(leds7, NUM_LEDS); //Strip 8
  FastLED.addLeds<LED_TYPE,32,COLOR_ORDER>(leds8, NUM_LEDS); //Strip 7


  Serial.begin(115200);
}

void loop () {
  //int x;
  
  trailforward(WHITE_HEAD);
  trailbackward(WHITE_HEAD);
}


void trailforward(int head) {
  static int pos; // Must be local


  EVERY_N_MILLISECONDS(TRAVELSPEED) {  //travel speed [lower is faster]

    fadeToBlackBy(leds, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
    fadeToBlackBy(leds2, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]  
    fadeToBlackBy(leds3, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]  
    fadeToBlackBy(leds4, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]                        
    fadeToBlackBy(leds5, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
    fadeToBlackBy(leds6, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]  
    fadeToBlackBy(leds7, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]  
    fadeToBlackBy(leds8, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]                        
    if (head == 1) {
      leds[pos] = CRGB::White;
      leds2[pos] = CRGB::White;
      leds3[pos] = CRGB::White;
      leds4[pos] = CRGB::White;
      FastLED.show();
    }
    leds[pos] = CHSV(hue,200,128);
    leds2[pos] = CHSV(hue,200,128);
    leds3[pos] = CHSV(hue,200,128);
    leds4[pos] = CHSV(hue,200,128);
    leds5[pos] = CHSV(hue,200,128);
    leds6[pos] = CHSV(hue,200,128);
    leds7[pos] = CHSV(hue,200,128);
    leds8[pos] = CHSV(hue,200,128);
    pos++;
    if (pos == NUM_LEDS) { 
      pos = 0; 
      hue = hue + 10;
      if (hue > 254) { hue = 0; }  //go back to first pixel
    }  //go back to first pixel
    FastLED.show();
  }
}

void trailbackward(int head) {

    EVERY_N_MILLISECONDS(TRAVELSPEED) {  //travel speed [lower is faster]
      Serial.println(pos);
      fadeToBlackBy(leds, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds2, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds3, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds4, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds5, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds6, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds7, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      fadeToBlackBy(leds8, NUM_LEDS, TAIL_LENGTH);  //fade amount [lower is less fade]
      if (head == 1) {
        leds[pos] = CRGB::White;
        leds2[pos] = CRGB::White;
        leds3[pos] = CRGB::White;
        leds4[pos] = CRGB::White;
        FastLED.show();
      }
      leds[pos] = CHSV(hue,200,128);
      leds2[pos] = CHSV(hue,200,128);
      leds3[pos] = CHSV(hue,200,128);
      leds4[pos] = CHSV(hue,200,128);
      leds5[pos] = CHSV(hue,200,128);
      leds6[pos] = CHSV(hue,200,128);
      leds7[pos] = CHSV(hue,200,128);
      leds8[pos] = CHSV(hue,200,128);
      pos = pos - 1;
      if (pos == -1) { 
        pos = NUM_LEDS-1; 
        hue = hue + 10;
        if (hue > 254) { hue = 0; }  //go back to first pixel
      }  //go back to first pixel
      FastLED.show();
    }

}

