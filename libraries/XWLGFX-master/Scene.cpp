#include "Scene.h"

void Scene::rainbow(CRGB *pix, uint32_t num, uint8_t hue, uint8_t size) {
    fill_rainbow(pix, num, hue, size);
}

void Scene::confetti(CRGB *pix, uint32_t num, uint8_t bhue) {
    //TODO: Replace with using dynamic memory
    static CRGB confettiPix[MAX_PIXEL];
    fadeToBlackBy(confettiPix, num, 10);
    int pos = random16(num);
    confettiPix[pos] += CHSV(bhue + random8(64), 200, 255);
    memcpy(pix, confettiPix, num * 3);
}

void Scene::bpm(CRGB *pix, uint32_t num, uint8_t bhue, uint8_t bpm, CRGBPalette16 palette) {
    //CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(bpm, 64, 255);
    //Serial.print("beat=");
    //Serial.println(beat);
    for ( int i = 0; i < num; i++) {
//        pix[i] = ColorFromPalette(palette, bhue + (i * 2), beat - bhue + (i * 10));
        pix[i] = ColorFromPalette(palette, bhue + (i * 2), 255);
    }
}

void Scene::juggle(CRGB *pix, uint32_t num, uint8_t bpm) {
    //TODO: Replace with using dynamic memory
//    static CRGB jugglePix[MAX_PIXEL];
//    fadeToBlackBy( jugglePix, num, 1);
//    byte dothue = 0;
//    
//    for (int i = -8; i < 0; i++) {
//        //TODO: fix for BPM
//        //jugglePix[beatsin16( i + 7, 0, num - 1 )] |= CHSV(dothue, 200, 255);
//        jugglePix[beatsin16( i + bpm, 0, num - 1 )] |= CHSV(dothue, 200, 255);
//        dothue += 32;
//    }
//    memcpy(pix, jugglePix, num * 3);
}

void Scene::sinelon(CRGB *pix, uint32_t num, uint8_t hue) {
    //TODO: Replace with using dynamic memory
//    static CRGB sinelonPix[MAX_PIXEL];
//    fadeToBlackBy( sinelonPix, num, 20);
//    int pos = beatsin16( 13, 0, num - 1 );
//    sinelonPix[pos] += CHSV(hue, 255, 192);
//    memcpy(pix, sinelonPix, num);
}

void Scene::fire(CRGB *pix, uint16_t width, uint16_t height, CRGBPalette16 gPal, bool hDirection, bool vDirection,
                         uint8_t cooling, uint8_t sparking) {
    int perOut = height;
    int output = width;
    static uint8_t *fireTemp;
    static uint16_t fireTempW;
    static uint16_t fireTempH;
    //Allocated memory for temp value
    if (fireTemp == NULL) {
        fireTemp = (uint8_t *) malloc(sizeof(uint8_t) * width * height);
        fireTempW = width;
        fireTempH = height;
    } else {
        if (fireTempW < width || fireTempH < height) {
            fireTemp = (uint8_t *) realloc(fireTemp, sizeof(uint8_t) * width * height);
            fireTempW = width;
            fireTempH = height;
        }
    }
    for (int x = 0; x < width; x++) {
        for (int i = 0; i < height; i++) {
            fireTemp[x * height + i] = qsub8(fireTemp[x * height + i], random8(0, ((cooling * 10) / height) + 2));
        }
        for (int k = height - 1; k >= 2; k--) {
            fireTemp[x * height + k] = (fireTemp[x * height + k - 1] +
                             fireTemp[x * height + k - 2] + fireTemp[x * height + k - 3]) / 3;
        }
        if (random8() < sparking) {
            int y = random8(7);
            fireTemp[x * height + y] = qadd8(fireTemp[x * height + y], random8(160, 255));
        }
        for (int j = 0; j < height; j++) {
            byte colorindex = scale8(fireTemp[x * height + j], 240);
            CRGB color = ColorFromPalette(gPal, colorindex);
            int pixelnumber;
            if (vDirection) {
                pixelnumber = (height - 1) - j;
            } else {
                pixelnumber = j;
            }
            if (hDirection) {
                pix[x * height + pixelnumber] = color;
            } else {
                pix[(width - x - 1) * height + pixelnumber] = color;
            }
        }
    }
}

void Scene::lines(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint8_t numLines, uint16_t sizeLine, uint16_t angle, uint8_t feather, CRGB color){
    if (numLines > 0) {
//        Serial.print("LINE-");
        uint16_t maxSide;
        uint16_t minSide;
        if (width > height) {
            maxSide = width;
            minSide = height;
        } else {
            maxSide = height;
            minSide = width;
        }
        uint16_t dist = maxSide / numLines; //TODO fix num line
        //angle = 45;
        float co = tan(angle*3.14/180.0);
        Serial.print("tg-");
        Serial.print(co);
        uint16_t dx = tan(angle*3.14/180) * minSide;
        //uint16_t dx = 4;
//        Serial.print("minSide-");
//        Serial.print(minSide);
//        Serial.print(" dx-");
//        Serial.print(dx);
        int xstart = map(beat88(speed), 0, 65535 , 0, dist);
        for (int x = xstart-dx;
             x < dist * numLines;
             x += dist) {
            //TODO: add size and feather functional
//            Serial.print("LINE: x0-");
//            Serial.print(x);
//            Serial.print(" y0-");
//            Serial.print(0);
//            Serial.print(" x1-");
//            Serial.print(x + dx);
//            Serial.print(" y1-");
//            Serial.println(height - 1);
            //Draw::drawLine(pix, width, height, 1, 0, 1, 1, color);
            Draw::drawLine(pix, width, height, x, 0, x + dx, height, color);
        }
    }
}

void Scene::circles(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint8_t numLines, uint16_t sizeLine, uint16_t angle, uint8_t feather, CRGB color){
    if (numLines > 0) {
        uint16_t maxSide;
        uint16_t minSide;
        if (width > height) {
            maxSide = width;
            minSide = height;
        } else {
            maxSide = height;
            minSide = width;
        }
        int r = map(beat88(speed), 0, 65535 , 0, maxSide * 0.7);
        Draw::drawCircle(pix, width, height, width / 2 - 1, height / 2 - 1, r - 1, color);
        Draw::drawCircle(pix, width, height, width / 2 - 1, height / 2 - 1, r, color);
        Draw::drawCircle(pix, width, height, width / 2 - 1, height / 2 - 1, r + 1, color);
    }
}

void Scene::sinusWave(CRGB *pix, uint16_t width, uint16_t height, accum88 speed, uint16_t widthWave, uint16_t heightWave, uint16_t sizeLine, uint8_t feather, CRGB color){
    heightWave = 40;
    // TODO NEW !!!
    uint16_t dx = 65535 / widthWave;
    uint16_t beat = beatsin88(speed);
    uint16_t yn2 = heightWave * 0.5;
    uint16_t yh2 = height * 0.5;
    for (int i = 0; i < width; i ++) {
        uint32_t px = beatsin88(speed) + dx * i;
        if (px > 65535) {
            px -= 65535;
        }
        uint16_t ystart = map(px, 0, 65535, 0, yn2);
//        Serial.print(" start-");
//        Serial.print(ystart);
        //uint16_t ystart = 10;
        for (int y = (yh2 - 1) - ystart; y < (yh2 - 1) + ystart; y++) {
            Draw::write_s(pix, width, height, i, y, color);
        }
    }
    //fillRGB(pix, 24 * 20, 1, 1, 1);
    //Draw::drawLine(pix, width, height, 0, 0, width, height, CRGB::White);
    
}

void Scene::fillRGB(CRGB *pix, uint32_t num, uint8_t r, uint8_t g, uint8_t b) {
    for (int i = 0; i < num; i++) {
        pix[i] = CRGB(r, g, b);
    }
}

void Scene::fillHSV(CRGB *pix, uint32_t num, uint8_t h, uint8_t s, uint8_t v) {
    for (int i = 0; i < num; i++) {
        pix[i] = CHSV(h, s, v);
    }
}

