#include "Draw.h"



void Draw::drawLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color){
    if (x0 == x1){
        writeVertLine(pix, width, height, x0, y0, y1, color);
    } else if (y0 == y1){
        writeHorzLine(pix, width, height, x0, x1, y0, color);
    } else {
        writeLine(pix, width, height, x0, y0, x1, y1, color);
        
    }
}

/******
   |
 ||||
 ||||      TODO: NEW ALGHORITM!
   ||||
    |
 ******/
void Draw::drawLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t size, CRGB color){
    /*
    int orient = 0;
    if ((x1 - x0 > 0) && (y1 - y0 > 0)) {
        orient = 1;
    }*/
    if (x0 > x1) {
        swapInt(x0, x1);
        swapInt(y0, y1);
    }
    //uint8_t c[3];
    if (floor(size / 2) == 0) {
        //if (orient == 1) {
            for (int i = (size / 2); i > 0; i++) {
                if (x0 == x1){
                    writeVertLine(pix, width, height, x0 - i, y0, y1, color);
                    writeVertLine(pix, width, height, x0 + i - 1, y0, y1, color);
                } else if (y0 == y1){
                    writeHorzLine(pix, width, height, x0, x1, y0 - i, color);
                    writeHorzLine(pix, width, height, x0, x1, y0 + i - 1, color);
                } else {
                    writeLine(pix, width, height, x0 - i, y0 - i, x1 - i, y1, color);
                    writeLine(pix, width, height, x0 + i - 1, y0 + i - 1, x1 + i - 1, y1 + i - 1, color);
                }
            }
        //}
    } else {
        if (x0 == x1){
            writeVertLine(pix, width, height, x0, y0, y1, color);
        } else if (y0 == y1){
            writeHorzLine(pix, width, height, x0, x1, y0, color);
        } else {
            writeLine(pix, width, height, x0, y0, x1, y1, color);
        }
        for (int i = ((size - 1)/ 2); i > 0; i++) {
            if (x0 == x1){
                writeVertLine(pix, width, height, x0 - i, y0, y1, color);
                writeVertLine(pix, width, height, x0 + i, y0, y1, color);
            } else if (y0 == y1){
                writeHorzLine(pix, width, height, x0, x1, y0 - i, color);
                writeHorzLine(pix, width, height, x0, x1, y0 + i, color);
            } else {
                writeLine(pix, width, height, x0 - i, y0, x1 - i, y1, color);
                writeLine(pix, width, height, x0 + i, y0, x1 + i, y1, color);
                /* 45 degree
                uint16_t d1 = floor((size - 1)/ 4);
                uint16_t d2 = floor((size - 1)/ 2 - 1)/2 ;
                writeLine(pix, width, height, x0 - d1, y0 - d2, x1 - d2, y1 - d1, color);
                writeLine(pix, width, height, x0 + d2, y0 + d1, x1 + d1, y1 + d1, color);
                 */
            }
        }
    }
}


void Draw::writeLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color){
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swapInt(x0, y0);
        swapInt(x1, y1);
    }

    if (x0 > x1) {
        swapInt(x0, x1);
        swapInt(y0, y1);
    }
    int16_t dx = x1 - x0;
    int16_t dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            write_s(pix, width, height, y0, x0, color);
        } else {
            write_s(pix, width, height, x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void Draw::drawRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color){
    writeVertLine(pix, width, height, x0, y0, y1, color);
    writeHorzLine(pix, width, height, x0, x1, y0, color);
    writeVertLine(pix, width, height, x1, y0, y1, color);
    writeHorzLine(pix, width, height, x0, x1, y1, color);
}


void Draw::drawRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t angle, CRGB color){
    uint16_t sizeH = abs(x1-x0);
    uint16_t sizeV = abs(y1-y0);
    //CRGB buf = CRGB[sizeH * sizeV];
    writeVertLine(pix, width, height, x0, y0, y1, color);
    writeHorzLine(pix, width, height, x0, x1, y0, color);
    writeVertLine(pix, width, height, x1, y0, y1, color);
    writeHorzLine(pix, width, height, x0, x1, y1, color);
}

void Draw::fillRect(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, CRGB color){
    if (y0>y1){
        swapInt(y0,y1);
    }
    for (int y = y0; y<y1; y++) {
        writeHorzLine(pix, width, height, x0, x1, y, color);
    }
}

void Draw::drawCircle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t r, CRGB color){
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    write_s(pix, width, height, x0, y0 + r, color);
    write_s(pix, width, height, x0, y0 - r, color);
    write_s(pix, width, height, x0 + r, y0, color);
    write_s(pix, width, height, x0 - r, y0, color);
    //Draw 1/8 Circle and mirroring data
    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        write_s(pix, width, height, x0 + x, y0 + y, color);
        write_s(pix, width, height, x0 - x, y0 + y, color);
        write_s(pix, width, height, x0 + x, y0 - y, color);
        write_s(pix, width, height, x0 - x, y0 - y, color);
        
        write_s(pix, width, height, x0 + y, y0 + x, color);
        write_s(pix, width, height, x0 - y, y0 + x, color);
        write_s(pix, width, height, x0 + y, y0 - x, color);
        write_s(pix, width, height, x0 - y, y0 - x, color);
    }
}

void Draw::fillCircle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t r, CRGB color){
    uint16_t corners = 3;
    uint16_t delta = 0;
    writeVertLine(pix, width, height, x0, y0 - r, y0 + 2 * r + 1, color);
    //writeFastVLine(x0, y0-r, 2*r+1, color);
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;
    
    delta++;
    
    write_s(pix, width, height, x0, y0 + r, color);
    write_s(pix, width, height, x0, y0 - r, color);
    write_s(pix, width, height, x0 + r, y0, color);
    write_s(pix, width, height, x0 - r, y0, color);
    //Draw 1/8 Circle and mirroring data
    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if(x < (y + 1)) {
            if (corners & 1) writeVertLine(pix, width, height, x0 + x, y0 - y, y0 + 2 * y + delta, color);
            if (corners & 1) writeVertLine(pix, width, height, x0 - x, y0 - y, y0 + 2 * y + delta, color);
            //if(corners & 1) writeFastVLine(x0+x, y0-y, 2*y+delta, color);
            //if(corners & 2) writeFastVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if (corners & 1) writeVertLine(pix, width, height, x0 + py, y0 - px, y0 + 2 * px + delta, color);
            if (corners & 1) writeVertLine(pix, width, height, x0 - py, y0 - px, y0 + 2 * px + delta, color);
//            if(corners & 1) writeFastVLine(x0+py, y0-px, 2*px+delta, color);
//            if(corners & 2) writeFastVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}


void Draw::drawTriangle(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, CRGB color) {
    drawLine(pix, width, height, x0, y0, x1, y1, color);
    drawLine(pix, width, height, x1, y1, x2, y2, color);
    drawLine(pix, width, height, x2, y2, x0, y0, color);
}

void Draw::writeVertLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x, int16_t y0, int16_t y1, CRGB color){
    if (y0>y1){
        swapInt(y0,y1);
    }
    for (int y = y0; y <= y1; y++) {
        write_s(pix, width, height, x, y, color);
    }
}

void Draw::writeHorzLine(CRGB *pix, uint16_t width, uint16_t height, int16_t x0, int16_t x1, int16_t y, CRGB color){
    if (x0>x1){
        swapInt(x0,x1);
    }
    for (int x = x0; x <= x1; x++) {
        write_s(pix, width, height, x, y, color);
    }
}

void Draw::write_s(CRGB *pix, uint16_t width, uint16_t height, int16_t x, int16_t y, CRGB color){
    //TODO: check !!
        if ((x < width) && (x >= 0) &&
            (y < height) && (y >= 0)) {
            
            pix[x * height + y] = color;
        } else {
//            Serial.print("Error P: x-");
//            Serial.print(x);
//            Serial.print(" y-");
//            Serial.print(y);
        }
}

