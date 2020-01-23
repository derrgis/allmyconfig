#ifndef XWLGFX_H
#define XWLGFX_H
#pragma message("GFX library by X-WL version 0.1")
#define VERSION_H 0
#define VERSION 1
#include "utility.h"
#include <Arduino.h>
#include "FastLED.h"

#ifndef MAX_RENDER_WIDTH
#define MAX_RENDER_WIDTH 160
//#define MAX_RENDER_WIDTH 15
#endif

#ifndef MAX_RENDER_HEIGHT
#define MAX_RENDER_HEIGHT 118
//#define MAX_RENDER_HEIGHT 10
#endif

#define MAX_PIXEL MAX_RENDER_WIDTH * MAX_RENDER_HEIGHT

#include "Scene.h"
#include "FX.h"
#include "Fixture.h"
#include "Merge.h"
//#include "Draw.h"

#define LEN_QUEUE 10
class XWLGFX {
  public:
    //system
    int width; //OutWidth
    int height;
    uint16_t outWidth;
    uint16_t outHeight;
    int xScaleW = 2;
    int xScaleH = 2;
    int renderWidth;
    int renderHeight;
    ///Using for calc speed
    int fps = 50;

    CRGB *mainBuffer;
    CRGB *buffer;
  public:
  
    void begin(uint16_t w, uint16_t h);
    void begin(uint16_t w, uint16_t h, uint8_t scaleW, uint8_t scaleH);
    void handle();
    void handle(uint8_t params[]);
    
    void getData(CRGB *out);//TODO
    void getData(CRGB *out, uint16_t startPix);//From LED PAR TODO: Add Mapping customs
    void getData(CRGB *outBuff, Fixture *fixture, uint8_t numFixture);

    CRGB* sceneRender(uint16_t width, uint16_t height, uint8_t idScn, accum88 speedBPM, uint8_t preset);
    void FXRender(CRGB *buf, uint16_t width, uint16_t height, uint8_t idFX, accum88 speedBPM, uint8_t briFX);
    
    void mergeSUB(CRGB *a, CRGB *b, uint32_t num, uint8_t v);
    void mergeADD(CRGB *a, CRGB *b, uint32_t num, uint8_t v);
 
    void mergeHTP(CRGB *a, CRGB *b, uint32_t num);
    void mergeADD50(CRGB *a, CRGB *b, uint32_t num);
    void mergePTP(CRGB *a, CRGB *b, uint32_t num);
    
    void mergeAllPixels(CRGB *b1, uint16_t width, uint16_t height, CRGB *b2, uint8_t scaleW, uint8_t scaleH);
    void mergePixels(CRGB *b1, uint16_t width, uint16_t height, uint16_t startPix, CRGB *b2, uint16_t mX, uint16_t mY, uint16_t mSizeX, uint16_t mSizeY, uint8_t scaleW, uint8_t scaleH);
    
    float BPMtoSpeed8(uint8_t bpm);
    float BPMtoSpeed16(uint16_t bpm);
    float BPMtoSpeed88(accum88 bpm);
    
    accum88 bit16to88(uint8_t b1, uint8_t b2);
    
    CRGBPalette16 getPalette(uint8_t idPalette);
    
    //uint8_t addFixture();
    
    void print(CRGB *pixels, uint16_t w, uint16_t h);
    void performanceTest(CRGB *out, uint16_t width, uint16_t height);
};
#endif
