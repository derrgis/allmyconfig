
#include "FastLED.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0

#define NUM_LEDS 144
CRGB leds[NUM_LEDS];
int off=0;
void setup() {

  FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0;i<NUM_LEDS;i++)
{
  leds[i]=CHSV( (int)(  (i+off)%NUM_LEDS*255/NUM_LEDS),192,64)  ;
  
}
off=off+1;
FastLED.show();
FastLED.show();
FastLED.delay(1);

}
