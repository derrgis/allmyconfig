#include "FX.h"

void FX::addGlitter(CRGB *pix, uint32_t num, fract8 chanceOfGlitter)
{
    if ( random8() < chanceOfGlitter) {
        pix[ random16(num) ] += CRGB::White;
    }
}
