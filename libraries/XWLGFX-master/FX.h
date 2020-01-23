#ifndef FX_H
#define FX_H

#include <Arduino.h>
#include "FastLED.h"
#include "utility.h"

class FX {
  public:
    void static addGlitter(CRGB *pix, uint32_t num, fract8 chanceOfGlitter);
};
#endif
