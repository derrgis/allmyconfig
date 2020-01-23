#ifndef SCENE_H
#define SCENE_H

#include <Arduino.h>
#include "FastLED.h"
#include "Draw.h"
#include "utility.h"

class Scene {
  public:
//    static uint8_t *fireTemp;
//    static uint16_t fireTempW;
//    static uint16_t fireTempH;
    
    void static rainbow(CRGB *pix, uint32_t num, uint8_t hue, uint8_t size);
    void static confetti(CRGB *pix, uint32_t num, uint8_t bhue);
    void static bpm(CRGB *pix, uint32_t num, uint8_t bhue,
             uint8_t bpm, CRGBPalette16 palette);
    void static juggle(CRGB *pix, uint32_t num, uint8_t bpm);
    void static sinelon(CRGB *pix, uint32_t num, uint8_t hue);
    void static fire(CRGB *pix, uint16_t width, uint16_t height, CRGBPalette16 gPal, bool hDirection = false, bool vDirection = false, uint8_t cooling = 65, uint8_t sparking = 40);
    
    void static lines(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint8_t numLines, uint16_t sizeLine, uint16_t angle, uint8_t feather, CRGB color);
    void static circles(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint8_t numLines, uint16_t sizeLine, uint16_t angle, uint8_t feather, CRGB color);
    
    void static sinusWave(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint16_t widthWave, uint16_t heightWave, uint16_t sizeLine, uint8_t feather, CRGB color);
    
    void static fillRGB(CRGB *pix, uint32_t num, uint8_t r, uint8_t g, uint8_t b);
    void static fillHSV(CRGB *pix, uint32_t num, uint8_t h, uint8_t s, uint8_t v);

};
#endif
