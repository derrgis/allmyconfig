
#include "ledTable.h"
#include "images.h"

void setup() {
  // put your setup code here, to run once:
  /* LED_WIDTH=30;
    LED_HEIGHT=20;
    NUM_LEDS=600;
   // leds =(CRGB*)malloc(NUM_LEDS*sizeof(CRGB)) ;
    //Tpic =(CRGB*)malloc(NUM_LEDS*sizeof(CRGB)) ;
FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, 600   ) ;
   fill(CRGB::Red);


   FastLED.show();*/
initTable(30,20);
fill(CRGB::Red);
FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
