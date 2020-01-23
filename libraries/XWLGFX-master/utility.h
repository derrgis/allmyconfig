#ifndef UTILITY_H
#define UTILITY_H

#ifndef MAX_RENDER_WIDTH
#define MAX_RENDER_WIDTH 160
//#define MAX_RENDER_WIDTH 15
#endif

#ifndef MAX_RENDER_HEIGHT
#define MAX_RENDER_HEIGHT 118
//#define MAX_RENDER_HEIGHT 10
#endif

#define MAX_PIXEL MAX_RENDER_WIDTH * MAX_RENDER_HEIGHT
#include <Arduino.h>
#include "FastLED.h"

#define FIRE_RENDER_WIDTH 16
#define FIRE_RENDER_HEIGHT 118

#define short_get_high_byte(x)((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)(LOW_BYTE & x)
#define bytes_to_short(h,l)( ((h << 8) & 0xff00) | (l & 0x00FF) );
#define swapInt(a, b) { int t = a; a = b; b = t; }

#endif
