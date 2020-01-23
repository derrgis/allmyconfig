#ifndef FIXTURE_H
#define FIXTURE_H

#include <Arduino.h>
#include "FastLED.h"
#include "utility.h"

/*
    Default order: start left-down!
    -----------------
    | 2   5   8  11 |
    | 1   4   7  10 |
    | 0   3   6   9 |
    -----------------
 */
typedef enum {
    NORMAL = 0,
    REVERS_VERTICAL,
    REVERS_HORIZONTAL,
    REVERS_FULL,
    ZIGZAG_LR_U,
    ZIGZAG_LR_D,
    ZIGZAG_RL_U,
    ZIGZAG_RL_D,
    ZIGZAG_UD_L,
    ZIGZAG_UD_R,
    ZIGZAG_DU_L,
    ZIGZAG_DU_R,
} MappingType;

struct MappingLayer {
  public:
    uint16_t inputX1;
    uint16_t inputY1;
    uint16_t inputX2;
    uint16_t inputY2;
    
    uint16_t   outX1;
    uint16_t   outY1;
    uint16_t   outX2;
    uint16_t   outY2;
    
    bool reverseX = false;
    bool reverseY = false;
    bool isFastSampling;
    
    MappingLayer(){
        this->inputX1 = 0;
        this->inputY1 = 0;
        this->inputX2 = 0;
        this->inputY2 = 0;
        
        this->outX1 = 0;
        this->outY1 = 0;
        this->outX2 = 0;
        this->outY2 = 0;
    }
    
    MappingLayer(uint16_t inputX1, uint16_t inputY1, uint16_t inputX2, uint16_t inputY2,
                 uint16_t   outX1, uint16_t   outY1, uint16_t   outX2, uint16_t   outY2){
        if (inputX1 > inputX2) {
            reverseDirectionX();
            this->inputX1 = inputX2;
            this->inputX2 = inputX1;
        } else {
            this->inputX1 = inputX1;
            this->inputX2 = inputX2;
        }
        if (inputY1 > inputY2) {
            reverseDirectionY();
            this->inputY1 = inputY2;
            this->inputY2 = inputY1;
        } else {
            this->inputY1 = inputY1;
            this->inputY2 = inputY2;
        }
        
        if (outX1 > outX2) {
            reverseDirectionX();
            this->outX1 = outX2;
            this->outX2 = outX1;
        } else {
            this->outX1 = outX1;
            this->outX2 = outX2;
        }
        if (outY1 > outY2) {
            reverseDirectionY();
            this->outY1 = outY2;
            this->outY2 = outY1;
        } else {
            this->outY1 = outY1;
            this->outY2 = outY2;
        }
        this->isFastSampling = checkFastSamplingAvailable(*this);
    }
    
    MappingLayer getNewLayer(uint16_t inputX1, uint16_t inputY1, uint16_t inputX2, uint16_t inputY2,
                            uint16_t   outX1, uint16_t   outY1, uint16_t   outX2, uint16_t   outY2){
        MappingLayer tlayer = *this;
        tlayer.inputX1 = inputX1; tlayer.inputY1 = inputY1;
        tlayer.inputX2 = inputX2; tlayer.inputY2 = inputY2;
        
        tlayer.outX1 = outX1; tlayer.outY1 = outY1;
        tlayer.outX2 = outX2; tlayer.outY2 = outY2;
        return tlayer;
    }
    
    void reverseDirectionX(){
        if (this->reverseX) {
            this->reverseX = false;
        } else {
            this->reverseX = true;
        }
    }
    
    void reverseDirectionY(){
        if (this->reverseY) {
            this->reverseY = false;
        } else {
            this->reverseY = true;
        }
    }
    
    bool checkFastSamplingAvailable(MappingLayer layer){
        uint16_t inDX = abs(layer.inputX2 - layer.inputX1) + 1;
        uint16_t outDX = abs(layer.outX2 - layer.outX1) + 1;
        if ((inDX % outDX) != 0) {
            return false;
        }
        uint16_t inDY = abs(layer.inputY2 - layer.inputY1) + 1;
        uint16_t outDY = abs(layer.outY2 - layer.outY1) + 1;
        if ((inDY % outDY) != 0) {
            return false;
        }
        return true;
    }
    
    void print(){
        Serial.print("Layer\t");
        Serial.print("In:\t(");
        Serial.print(inputX1);
        Serial.print(", ");
        Serial.print(inputY1);
        Serial.print(") (");
        Serial.print(inputX2);
        Serial.print(", ");
        Serial.print(inputY2);
        Serial.println(")");
        
        Serial.print("Out:\t\t(");
        Serial.print(outX1);
        Serial.print(", ");
        Serial.print(outY1);
        Serial.print(") (");
        Serial.print(outX2);
        Serial.print(", ");
        Serial.print(outY2);
        Serial.println(")");
        
        Serial.print("\treverseX = ");
        Serial.print(reverseX);
        Serial.print("\treverseY = ");
        Serial.print(reverseY);
        Serial.print("\tFast = ");
        Serial.println(isFastSampling);
    }
};

struct Fixture {
  public:
    MappingLayer *layer;
    uint8_t numLayer;
    MappingType type;
    
    Fixture(MappingLayer *mlayer, MappingType mtype = MappingType::NORMAL) {
        uint16_t inDX = mlayer->inputX2 - mlayer->inputX1 + 1;
        uint16_t outDX = mlayer->outX2 - mlayer->outX1 + 1;
        uint8_t scaleX = inDX / outDX;
        uint16_t inDY = mlayer->inputY2 - mlayer->inputY1 + 1;
        uint16_t outDY = mlayer->outY2 - mlayer->outY1 + 1;
        uint8_t scaleY = inDY / outDY;
        if ((mtype == MappingType::NORMAL) || (mtype == MappingType::REVERS_VERTICAL) || (mtype == MappingType::REVERS_FULL)){
            //one layer configuration
            createFixture(mlayer, 1, mtype);
        } else
        //multiple layer configuration
        if ((mtype == MappingType::ZIGZAG_UD_L) || (mtype == MappingType::ZIGZAG_UD_R) || (mtype == MappingType::ZIGZAG_DU_L) || (mtype == MappingType::ZIGZAG_DU_R)) {
            //VERTICAL STRIP
            MappingLayer *tlayer = new MappingLayer[outDX];
            for (int i = 0; i < outDX; i++){
                tlayer[i] = mlayer->getNewLayer(mlayer->inputX1 + i * scaleX, mlayer->inputY1,
                                        mlayer->inputX1 + ((i + 1) * scaleX - 1), mlayer->inputY2,
                                        mlayer->outX1 + i, mlayer->outY1,
                                        mlayer->outX1 + i, mlayer->outY2);
                
            }
            createFixture(tlayer, outDX, mtype);
            free(tlayer);
        } else {
            //HORIZONTAL STRIP
            MappingLayer *tlayer = new MappingLayer[outDY];
            for (int i = 0; i < outDY; i++){
                tlayer[i] = mlayer->getNewLayer(mlayer->inputX1, mlayer->inputY1 + i * scaleY,
                                        mlayer->inputX2, mlayer->inputY1 + ((i + 1) * scaleY - 1),
                                        mlayer->outX1, mlayer->outY1 + i,
                                        mlayer->outX2, mlayer->outY1 + i);
                
            }
            createFixture(tlayer, outDY, mtype);
            free(tlayer);
        }
    }
    
    Fixture(MappingLayer *mlayer, uint8_t nLayer,
            MappingType mtype = MappingType::NORMAL) {
        createFixture(mlayer, nLayer);
    }
    
    bool createFixture(MappingLayer *mlayer, uint8_t nLayer,
                       MappingType mtype = MappingType::NORMAL){
        type = mtype;
        numLayer = nLayer;
        layer = new MappingLayer[numLayer];
        for (int i = 0; i < numLayer; i++){
            layer[i] = mlayer[i].getNewLayer(mlayer[i].inputX1, mlayer[i].inputY1,
                                             mlayer[i].inputX2, mlayer[i].inputY2,
                                             mlayer[i].outX1, mlayer[i].outY1,
                                             mlayer[i].outX2, mlayer[i].outY2);
        }
        switch (type) {
            case MappingType::REVERS_VERTICAL: {
                for (int i = 0; i < numLayer; i++){
                    layer[i].reverseDirectionY();
                }
            } break;
            case MappingType::REVERS_HORIZONTAL: {
                for (int i = 0; i < numLayer; i++){
                    layer[i].reverseDirectionX();
                }
            } break;
            case MappingType::REVERS_FULL: {
                for (int i = 0; i < numLayer; i++){
                    layer[i].reverseDirectionX();
                    layer[i].reverseDirectionY();
                }
            } break;
                
            case MappingType::ZIGZAG_LR_U: {
                for (int i = numLayer - 2; i >= 0; i-=2){
                    layer[i].reverseDirectionX();
                }
            } break;
            case MappingType::ZIGZAG_LR_D: {
                for (int i = 1; i < numLayer; i+=2){
                    layer[i].reverseDirectionX();
                }
            } break;
            case MappingType::ZIGZAG_RL_U: {
                for (int i = numLayer - 1; i >= 0; i-=2){
                    layer[i].reverseDirectionX();
                }
            } break;
            case MappingType::ZIGZAG_RL_D: {
                for (int i = 0; i < numLayer; i+=2){
                    layer[i].reverseDirectionX();
                }
            } break;
                
                
            case MappingType::ZIGZAG_UD_L: {
                for (int i = 1; i < numLayer; i+=2){
                    layer[i].reverseDirectionY();
                }
            } break;
            case MappingType::ZIGZAG_UD_R: {
                for (int i = numLayer - 2; i >= 0; i-=2){
                    layer[i].reverseDirectionY();
                }
            } break;
            case MappingType::ZIGZAG_DU_L: {
                for (int i = 0; i < numLayer; i+=2){
                    layer[i].reverseDirectionY();
                }
            } break;
            case MappingType::ZIGZAG_DU_R: {
                for (int i = numLayer - 1; i >= 0; i-=2){
                    layer[i].reverseDirectionY();
                }
            } break;
            default: {
            } break;
        }
    }
    
    void print(){
        if (layer != NULL) {
            for (int i = 0; i < numLayer; i++){
                Serial.print(i);
                Serial.print(" ");
                layer[i].print();
            }
        } else {
            Serial.print("Layer is NULL");
        }
    }
};
#endif
