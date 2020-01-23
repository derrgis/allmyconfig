#ifndef DRAW_H
#define DRAW_H

#include <Arduino.h>
#include "FastLED.h"
#include "utility.h"
/*
#define short_get_high_byte(x)((HIGH_BYTE & x) >> 8)
#define short_get_low_byte(x)(LOW_BYTE & x)
#define bytes_to_short(h,l)( ((h << 8) & 0xff00) | (l & 0x00FF) );
#define swapInt(a, b) { int t = a; a = b; b = t; }

#ifndef MAX_RENDER_WIDTH
#define MAX_RENDER_WIDTH 160
#endif

#ifndef MAX_RENDER_HEIGHT
#define MAX_RENDER_HEIGHT 118
#endif

#define MAX_PIXEL MAX_RENDER_WIDTH * MAX_RENDER_HEIGHT
*/
class Draw {
  public:
    
    void static drawLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void static drawLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t size, CRGB color);
    //add draw with DrawWithThinkAndFeather
    void static drawRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void static drawRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t angle, CRGB color);
    void static fillRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    //Rect with Rotation!)
    
    void static drawCircle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t r, CRGB color);
    void static fillCircle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t r, CRGB color);
    void static drawTriangle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color);
    
    
    
    void static writeLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color);
    void static writeVertLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t y1, CRGB color);
    void static writeHorzLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t x1, int16_t y, CRGB color);
    
    void static write_s(CRGB *pix, uint16_t width, uint16_t height, int16_t x, int16_t y, CRGB color);
    //void swapInt(int *a, int *b);
    
};
#endif
