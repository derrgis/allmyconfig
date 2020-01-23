#ifndef MERGE_H
#define MERGE_H

#include <Arduino.h>
#include "FastLED.h"
#include "utility.h"

#include "Fixture.h"

class Merge {
  public:
    void static mergeWithFixture(CRGB *pix1, uint16_t width1, uint16_t height1, CRGB *pix2, uint16_t width2, uint16_t height2, Fixture fixture);
    
    void static fastSamplingDown(CRGB *outBuff, uint16_t outWidth, uint16_t outHeight, CRGB *inBuff, uint16_t inWidth, uint16_t inHeight, MappingLayer layer);
};
#endif
