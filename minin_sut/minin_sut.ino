#include "Arduino.h"
#include "FastLED.h"
#define NUM_LED 65
#define PIN_NUM 27

 CRGB leds[NUM_LED];
 int k=0;
void setup() {
  FastLED.addLeds<WS2812B, PIN_NUM, GRB>(leds, 0, NUM_LED);
  // put your setup code here, to run once:

}

void loop() {
  for(int i=0;i<8;i++)
  for (int j=0;j<8;j++)
  {
    leds[i+8*j]=CRGB(i*16*2,j*16*2,(i+j)*16 );//s. CHSV((i+k)%8*16,j*16,60);
  }
  k++;
  FastLED.show();
  // put your main code here, to run repeatedly:

}
