#include "XWLGFX.h"

void XWLGFX::begin(uint16_t w, uint16_t h){
    begin(w, h, xScaleW, xScaleH);
}

void XWLGFX::begin(uint16_t w, uint16_t h, uint8_t scaleW, uint8_t scaleH){
    outWidth = w;
    outHeight = h;
    xScaleW = scaleW;
    xScaleH = scaleH;
    renderWidth = outWidth * xScaleW;
    renderHeight = outHeight * xScaleH;
    //Create Main Buffer.
    mainBuffer = (CRGB *) malloc( sizeof(CRGB) * renderWidth * renderHeight);
    if (mainBuffer != NULL) {
        Scene::fillRGB(mainBuffer, renderWidth * renderHeight, 0, 0, 0);
        Serial.print("Size main buffer: ");
        Serial.print(sizeof(CRGB) * renderWidth * renderHeight);
        Serial.println(" byte");
    } else {
        Serial.print("Error create main buffer!");
    }
    buffer = (CRGB *) malloc( sizeof(CRGB) * renderWidth * renderHeight);
    if (buffer != NULL) {
        Scene::fillRGB(buffer, renderWidth * renderHeight, 0, 0, 0);
        Serial.print("Size buffer: ");
        Serial.print(sizeof(CRGB) * renderWidth * renderHeight);
        Serial.println(" byte");
    } else {
        Serial.print("Error create buffer!");
    }
}

void XWLGFX::handle(){
    
}

void XWLGFX::handle(uint8_t params[]){
    if ((buffer != NULL) && (mainBuffer != NULL)) {
    //Scene Block 1
    //ch 1 (0-255) set Bric
    FastLED.setBrightness(params[1]); //Set Bri
    uint8_t scenePreset = floor((params[4]) / 10);
    accum88 sceneBPMSpeed = bit16to88(params[8], params[9]);
    //ch 2 (0-255) set Scene module
    int currentScn = floor((params[2]) / 10);
        //Clear buffer
    Scene::fillRGB(mainBuffer, renderWidth * renderHeight, 0, 0, 0);
    mergeADD(mainBuffer, sceneRender(renderWidth, renderHeight, currentScn, sceneBPMSpeed, scenePreset),
             renderWidth * renderHeight, params[3]);
    uint8_t idFX = floor((params[10]) / 10);
    uint8_t briFX = params[11];
    accum88 fxBPMspeed = bit16to88(params[12], params[13]);
    FXRender(mainBuffer, renderWidth, renderHeight, idFX, fxBPMspeed, briFX);
    }
}

void XWLGFX::getData(CRGB *out){
    //Serial.print(MAX_PIXEL);
    //memcpy(out, mainBuffer, 118*16*3);
    //print(mainBuffer, renderWidth, renderHeight);
    mergeAllPixels(out, outWidth, outHeight, mainBuffer, xScaleW, xScaleH);
    //print(out, outWidth, outHeight);
    //print(out, 8, 10);
    //Serial.print("get");
}

void XWLGFX::getData(CRGB *outBuff, Fixture *fixture, uint8_t numFixture){
//    print(mainBuffer, renderWidth, renderHeight);
    for (int i = 0; i < numFixture; i++) {
        //fixture[i].layer[0].print();
        Merge::mergeWithFixture(outBuff, outWidth, outHeight, mainBuffer, renderWidth, renderHeight, fixture[i]);
    }
//    print(outBuff, outWidth, outHeight);
}

void XWLGFX::getData(CRGB *out, uint16_t startPix){//From LED PAR TODO: Add Mapping customs
    //1
    print(mainBuffer, renderWidth, renderHeight);
    mergePixels(out, outWidth, outHeight, 0, mainBuffer, 0, 6,
                1, 4, 4, 6);
    mergePixels(out, outWidth, outHeight, 4, mainBuffer, 4, 3,
                1, 5, 4, 6);
    mergePixels(out, outWidth, outHeight, 9, mainBuffer, 8, 0,
                1, 6, 4, 6);
    mergePixels(out, outWidth, outHeight, 15, mainBuffer, 12, 3,
                1, 6, 4, 6);
    mergePixels(out, outWidth, outHeight, 20, mainBuffer, 16, 0,
                1, 6, 4, 6);
    mergePixels(out, outWidth, outHeight, 26, mainBuffer, 20, 3,
                1, 5, 4, 6);
    mergePixels(out, outWidth, outHeight, 31, mainBuffer, 24, 0,
                1, 6, 4, 6);
    mergePixels(out, outWidth, outHeight, 37,  mainBuffer, 28, 3,
                1, 5, 4, 6);
    mergePixels(out, outWidth, outHeight, 42, mainBuffer, 32, 6,
                1, 4, 4, 6);
//    print(out, outWidth, outHeight);
}

CRGB* XWLGFX::sceneRender(uint16_t width, uint16_t height, uint8_t idScn, accum88 speedBPM, uint8_t preset) {
    uint16_t num = width * height;
    Scene::fillRGB(buffer, num,0, 0, 64);
    if (buffer != NULL) {
        static float speedScn;
        static float cScn;
        cScn = beat8(speedBPM);
        CRGBPalette16 gPal = getPalette(preset);
        switch (idScn) {
            case 1: {
                if (preset == 0) {
                    gPal = HeatColors_p;
                }
                Scene::fire(buffer, width, height, gPal);
            } break;
            case 2: {
                Scene::bpm(buffer, num, cScn, speedBPM, gPal);
            }
                break;
            case 3: {
                Scene::confetti(buffer, num, cScn);
            } break;
            case 4: {
                Scene::juggle(buffer, num, speedBPM);
            } break;
            case 5: { //(CRGB *pix, uint32_t num, uint8_t hue)
                //juggle(buf, num);
            } break;
            case 6: {
                Scene::lines(buffer, width, height, speedBPM, 2, 1, 45, 0, CRGB::White);
            } break;
            case 7: {
                Scene::circles(buffer, width, height, speedBPM, 2, 1, 45, 0, CRGB::Yellow);
            } break;
            case 8: {
                Scene::sinusWave(buffer, width, height, speedBPM, 50, 50, 1, 0, CRGB::Green);
            } break;
            default: {
            } break;
        }
    }
    //Tips! Need to clear memory after using data!
    return buffer;
}
void XWLGFX::FXRender(CRGB *buf, uint16_t width, uint16_t height, uint8_t idFX, accum88 speedBPM, uint8_t briFX) {
    static float speedFX;
    static float cFX;
    speedFX = BPMtoSpeed8(speedBPM);
    cFX += speedFX;
    Scene::fillRGB(buffer, renderWidth * renderHeight, 0, 0, 0);
    //---------
    switch (idFX) {
        case 1: {
            FX::addGlitter(buffer, width * height, 60);
            mergeADD(buf, buffer, width * height, briFX);
        } break;
        case 2: {
            Scene::fillHSV(buffer, width * height,
                    0, 0, squarewave8(cFX));
            mergeADD(buf, buffer, width * height, briFX);
        } break;
            
        case 5: {
            Scene::fillHSV(buffer, width * height,
                    0, 0, quadwave8(cFX));
            mergeSUB(buf, buffer, width * height, briFX);
        } break;
        case 6: {
            Scene::fillHSV(buffer, width * height,
                    0, 0, quadwave8(cFX));
            mergeSUB(buf, buffer, width * height, briFX);
        } break;
        case 7: {
            Scene::fillHSV(buffer, width * height,
                           0, 0, squarewave8(cFX));
            mergeSUB(buf, buffer, width * height, briFX);
        }
        default: break;
    }
}

void XWLGFX::mergeSUB(CRGB *a, CRGB *b, uint32_t num, uint8_t v) {
    for (int i = 0; i < num; i++) {
        a[i].r -= scale8_video(b[i].r, v);
        a[i].g -= scale8_video(b[i].g, v);
        a[i].b -= scale8_video(b[i].b, v);
    }
}

void XWLGFX::mergeADD(CRGB *a, CRGB *b, uint32_t num, uint8_t v) {
    for (int i = 0; i < num; i++) {
            a[i].r += scale8_video(b[i].r, v);
            a[i].g += scale8_video(b[i].g, v);
            a[i].b += scale8_video(b[i].b, v);
    }
}

void XWLGFX::mergeADD50(CRGB *a, CRGB *b, uint32_t num) {
    for (int i = 0; i < num; i++) {
        a[i].r = scale8_video(a[i].r, 127) + scale8_video(b[i].r, 127);
        a[i].g = scale8_video(a[i].r, 127) + scale8_video(b[i].g, 127);
        a[i].b = scale8_video(a[i].r, 127) + scale8_video(b[i].b, 127);
        
    }
}

void XWLGFX::mergeHTP(CRGB *a, CRGB *b, uint32_t num) {
    for (int i = 0; i < num; i++) {
        a[i] = a[i] & b[i];
    }
}

void XWLGFX::mergePTP(CRGB *a, CRGB *b, uint32_t num) {
    for (int i = 0; i < num; i++) {
        a[i].r = scale8_video(a[i].r, b[i].r);
        a[i].g = scale8_video(a[i].g, b[i].g);
        a[i].b = scale8_video(a[i].b, b[i].b);
    }
}

/***********************************************************************
 Merged pixels downgrade size for more quality and castomize led order.
 Don't checked size pixelBuffer! Always use b2 = b1 * N!
 
 CRGB[6,4]                              CRGB [2,2]
 -------------------------
 |  0   1   2 |  3   4   5 |              ---------
 |  6   7   8 |  9  10  11 |  scaleW = 3 | 0  |  1 |
 |-------------------------|     --->    | ------- |
 | 12  13  14 | 15  16  17 |  scaleH = 2 | 2  |  3 |
 | 18  19  20 | 21  22  23 |              ---------
 --------------------------
 */
void XWLGFX::mergeAllPixels(CRGB *b1, uint16_t width, uint16_t height, CRGB *b2, uint8_t scaleW, uint8_t scaleH) {
    uint16_t outPixel = 0;

    for (int x = 0; x < width * scaleW; x += scaleW) {
        for (int y = 0; y < height * scaleH; y += scaleH) {
            //Set block
            uint32_t color[3];
            for (int i = x; i < x + scaleW; i++) {
                for (int j = y; j < y + scaleH; j++) {
                    color[0] += b2[renderHeight * i + j].r;
                    color[1] += b2[renderHeight * i + j].g;
                    color[2] += b2[renderHeight * i + j].b;
                }
            }
            b1[outPixel] = CRGB((float)color[0] / (scaleW * scaleH),
                                (float)color[1] / (scaleW * scaleH),
                                (float)color[2] / (scaleW * scaleH));
            outPixel++;
            //Set Block
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
    }
}

/***********************************************************************
Merged pixels downgrade size for more quality and castomize led order.
Don't checked size pixelBuffer! Always use b2 = b1 * N!

CRGB[6,4]                              CRGB [2,2]
---------------------------   TODO FIX
|  0  | 1   2   3 |  4   5 |              ---------
|  6  | 7   8   9 | 10  11 |  scaleW = 3 | 0  |  1 |
|--------------------------|     --->    | ------- |
| 12  13  14 | 15   16  17 |  scaleH = 2 | 2  |  3 |
| 18  19  20 | 21   22  23 |              ---------
--------------------------
*/
void XWLGFX::mergePixels(CRGB *b1, uint16_t width, uint16_t height, uint16_t startPix, CRGB *b2, uint16_t mX, uint16_t mY, uint16_t mSizeX, uint16_t mSizeY, uint8_t scaleW, uint8_t scaleH) {
    //TODO: Check using width and height
    uint16_t outPixel = startPix;
    for (int x = mX; x < mX + mSizeX * scaleW; x += scaleW) {
        for (int y = mY; y < mY + mSizeY * scaleH; y += scaleH) {
            //Set block
            uint32_t color[3];
            for (int i = x; i < x + scaleW; i++) {
                for (int j = y; j < y + scaleH; j++) {
                    //TODO: replace to height
                    color[0] += b2[renderHeight * i + j].r;
                    color[1] += b2[renderHeight * i + j].g;
                    color[2] += b2[renderHeight * i + j].b;
                }
            }
            b1[outPixel] = CRGB((float)color[0] / (scaleW * scaleH),
                                (float)color[1] / (scaleW * scaleH),
                                (float)color[2] / (scaleW * scaleH));
            outPixel++;
            //Set Block
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
    }
}

float XWLGFX::BPMtoSpeed8(uint8_t bpm) {
    //def 50fps
    uint8_t fps = 60;
    if (bpm > 0) {
        return  (bpm * 255) / ((fps ^ 2) * 60);
    } else {
        return 0;
    }
}
float XWLGFX::BPMtoSpeed16(uint16_t bpm) {
    //def 50fps
    uint8_t fps = 60;
    if (bpm > 0) {
//        return  (bpm * 65535) / ((fps ^ 2) * 60) ;
        return  (bpm) / ((fps ^ 2) * 60) ;
    } else {
        return 0;
    }
}

float XWLGFX::BPMtoSpeed88(accum88 bpm) {
    //TODO: implementation for Q8.8
    //def 50fps
    uint8_t fps = 60;
    if (bpm > 0) {
        //        return  (bpm * 65535) / ((fps ^ 2) * 60) ;
        return  (bpm) / ((fps ^ 2) * 60); //Not valid!
    } else {
        return 0;
    }
}

accum88 XWLGFX::bit16to88(uint8_t b1, uint8_t b2) {
    //Calc Q8.8 fixed-point value
    accum88 s88 = (b1 + b2 * 0.00390625) * 256;
    return s88;
}

CRGBPalette16 XWLGFX::getPalette(uint8_t idPalette){
    CRGBPalette16 palette;
    switch (idPalette){
        case 1:{
            palette = CloudColors_p;
        } break;
        case 2:{
            palette = LavaColors_p;
        } break;
        case 3:{
            palette = OceanColors_p;
        } break;
        case 4:{
            palette = ForestColors_p;
        } break;
        case 5:{
            palette = PartyColors_p;
        } break;
        case 6:{
            palette = HeatColors_p;
        } break;
        case 7:{
            palette = RainbowColors_p;
        } break;
        case 8:{
            palette = RainbowStripeColors_p;
        } break;
        default:
            //TODO: Change pallette!
            palette = PartyColors_p;
            break;
    }
    return palette;
}

void XWLGFX::print(CRGB *pixels, uint16_t w, uint16_t h){
    Serial.print("h-");
    Serial.print(h);
    Serial.print(" w-");
    Serial.println(w);
    //for (int i = 0; i< h; i++) {
    for (int i = h - 1; i >= 0; i--) {
        for (int j = 0; j < w; j++) {
        //uint8_t c[] = pixels[i * w + j].raw;
            uint8_t c[3];
            c[0] = pixels[j * h + i].r;
            c[1] = pixels[j * h + i].g;
            c[2] = pixels[j * h + i].b;
        Serial.print("(");
        for (int p = 0; p < 3; p++) {
            if (c[p] < 10) {
                Serial.print("  ");
                Serial.print(c[p]);
            } else if (c[p] < 100) {
                Serial.print(" ");
                Serial.print(c[p]);
            } else {
                Serial.print(c[p]);
            }
            if (p != 2) {
                Serial.print(",");
            }
            
        }
        Serial.print(") ");
        }
    Serial.println();
    }
}

void XWLGFX::performanceTest(CRGB *out, uint16_t width, uint16_t height){
    int numReply = 100;
    Serial.print("Performance test! NumReply = ");
    Serial.println(numReply);
    static int beginTime = millis();
    for (int i = 0; i < numReply; i++) {
        Scene::fillRGB(mainBuffer, width * height, 0, 0, 0);
    }
    Serial.print("Fill Time: ");
    Serial.print(millis() - beginTime);
    Serial.print(" | ");
    Serial.println((millis() - beginTime)/(float)numReply);
    
    beginTime = millis();
    for (int i = 0; i < numReply; i++) {
        Scene::lines(buffer, width, height, 60*256, 2, 1, 45, 0, CRGB::White);
    }
    Serial.print("Line Time: ");
    Serial.print(millis() - beginTime);
    Serial.print(" | ");
    Serial.println((millis() - beginTime)/(float)numReply);
    /*
    CRGB *temp = (CRGB *) malloc( sizeof(CRGB) * width * height);
    beginTime = millis();
    for (int i = 0; i < numReply; i++) {
        memcpy(temp, out, sizeof(CRGB) * width * height);
    }
    free(temp);
    Serial.print("Line Time: ");
    Serial.print(millis() - beginTime);
    Serial.print(" | ");
    Serial.println((millis() - beginTime)/(float)numReply);
     */
}
