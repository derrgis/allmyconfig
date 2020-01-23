#include "MERGE.h"

void Merge::mergeWithFixture(CRGB *pix1, uint16_t width1, uint16_t height1, CRGB *pix2, uint16_t width2, uint16_t height2, Fixture fixture) {
    for (int i = 0; i < fixture.numLayer; i++) {
            if (fixture.layer[i].isFastSampling) {
                fastSamplingDown(pix1, width1, height1, pix2, width2, height2, fixture.layer[i]);
            }
    }
}

void Merge::fastSamplingDown(CRGB *outBuff, uint16_t outWidth, uint16_t outHeight, CRGB *inBuff, uint16_t inWidth, uint16_t inHeight, MappingLayer layer) {
    uint16_t inDX = layer.inputX2 - layer.inputX1 + 1;
    uint16_t outDX = layer.outX2 - layer.outX1 + 1;
    uint8_t scaleX = inDX / outDX;
    uint16_t inDY = layer.inputY2 - layer.inputY1 + 1;
    uint16_t outDY = layer.outY2 - layer.outY1 + 1;
    uint8_t scaleY = inDY / outDY;

//    Serial.print("sX");
//    Serial.print(scaleX);
//    Serial.print(" sY");
//    Serial.println(scaleY);
    
    uint32_t color[3];
    uint16_t outX;
    uint16_t outY;
    uint16_t offset;
    for (int i = 0; i < outDX; i++) {
        for (int j = 0; j < outDY; j++) {
            uint32_t color[3];
            for (int x = layer.inputX1 + i * scaleX; x < layer.inputX1 + (i + 1) * scaleX; x++) {
                for (int y = layer.inputY1 + j * scaleY; y < layer.inputY1 + (j + 1) * scaleY; y++) {
                   color[0] += inBuff[inHeight * x + y].r;
                    color[1] += inBuff[inHeight * x + y].g;
                    color[2] += inBuff[inHeight * x + y].b;
                   /* color[0] += inBuff[inWidth * y + x].r;
                     color[1] += inBuff[inWidth * y + x].g;
                     color[2] += inBuff[inWidth * y + x].b;*/
                }
            }
            if (layer.reverseX){
                outX = layer.outX2 - i;
            } else {
                outX = layer.outX1 + i;
            }
            if (layer.reverseY){
                outY = layer.outY2 - j;
            } else {
                outY = layer.outY1 + j;
            }
           // outBuff[outX * outHeight + outY]
           /* if(outX%2==0)
                offset=outY + outX*outHeight;
            else
                offset=(outHeight <<1)* ( (outX>>1) + 1) - (1 + outY);*/
            //Serial.printf("%ld %ld %ld %ld %ld\n",outX,outY,outWidth,outHeight,offset);
            //if(offset<outWidth*outHeight)
            //outBuff[offset]
            if(outX * outHeight + outY<outWidth*outHeight)
            outBuff[outX * outHeight + outY]= CRGB((float)color[0] / (scaleX * scaleY),
                                (float)color[1] / (scaleX * scaleY),
                                (float)color[2] / (scaleX * scaleY));
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }
    }
}
