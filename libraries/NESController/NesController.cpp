//
//  NesController.cpp
//  
//
//  Created by Yves BAZIN on 15/12/2019.
//

#include "NesController.h"
//
//  NesController.h
//
//
//  Cr
#include <Wire.h>


    boolean NesController::verifyData(const uint8_t * dataIn, uint8_t dataSize) {
        byte orCheck = 0x00;   // Check if data is zeroed (bad connection)
        byte andCheck = 0xFF;  // Check if data is maxed (bad init)
        
        for (int i = 0; i < dataSize; i++) {
            orCheck |= dataIn[i];
            andCheck &= dataIn[i];
        }
        
        if (orCheck == 0x00 || andCheck == 0xFF) {
            return false;  // No data or bad data
        }
        
        return true;
    }
    
    inline boolean NesController::i2c_writePointer( byte addr, byte ptr) {
        Wire.beginTransmission(addr);
        Wire.write(ptr);
        return Wire.endTransmission() == 0;  // 0 = No Error
    }
    
    inline boolean NesController::i2c_requestMultiple( byte addr, uint8_t requestSize, uint8_t * dataOut) {
        uint8_t nBytesRecv = Wire.readBytes(dataOut,
                                            Wire.requestFrom(addr, requestSize));
        
        return (nBytesRecv == requestSize);  // Success if all bytes received
    }
    
    inline boolean NesController::i2c_readDataArray( byte addr, byte ptr, uint8_t requestSize, uint8_t * dataOut) {
        if (!i2c_writePointer( addr, ptr)) { return false; }  // Set start for data read
        delayMicroseconds(I2C_ConversionDelay);  // Wait for data conversion
        return i2c_requestMultiple(addr, requestSize, dataOut);
    }
    
    inline boolean NesController::i2c_writeRegister( byte addr, byte reg, byte value) {
        Wire.beginTransmission(addr);
        Wire.write(reg);
        Wire.write(value);
        return Wire.endTransmission() == 0;
    }
    


    
    void NesController::_NESTask(void *pvParameters)
    {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
        // -- Run forever...
        for(;;) {
            // -- Wait for the trigger
            ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
            
            
           NesController  *nes=(NesController *)pvParameters;
            
            uint8_t dataOut[6];
            nes->nespad.updated=false;
            // nespad.used=true;
            i2c_readDataArray( 0x52,0x00,6,  dataOut);
            verifyData(dataOut,requestSize);
            
            if(dataOut[4]!=255 || dataOut[5]!=255)
            {
                
                uint16_t value=~*((uint16_t *)&dataOut[4]);
                //Serial.printf("date %x %x %x\n",~*((uint16_t *)&dataOut[4]),~dataOut[4],~dataOut[5]);
                nes->nespad.a= (BUTTON_A & value) >0 ? true:false;
                nes->nespad.b=(BUTTON_B & value) >0 ? true:false;
                nes->nespad.start=(BUTTON_START & value) >0 ? true:false;
                nes->nespad.select=(BUTTON_SELECT & value) >0 ? true:false;
                nes->nespad.up=(BUTTON_UP & value) >0 ? true:false;
                nes->nespad.down=(BUTTON_DOWN & value) >0 ? true:false;
                nes->nespad.left=(BUTTON_LEFT & value) >0 ? true:false;
                nes->nespad.right=(BUTTON_RIGHT & value) >0 ? true:false;
                nes->nespad.updated=true;
                
                
            }
            // delay(10);
            _NesuserTaskHandle=0;
            //nespad.used=false;
            
            
        }
    }
    
    void   IRAM_ATTR  NesController::changeNes()
    {
        portENTER_CRITICAL(&NEStimerMux);
       
        if ( _NesuserTaskHandle==0)
        {
            _NesuserTaskHandle = xTaskGetCurrentTaskHandle();
            BaseType_t xHigherP=pdFALSE;
            // -- Trigger the show task
            vTaskNotifyGiveFromISR(_NesTaskHandle,&xHigherP);
            if(xHigherP)
                portYIELD_FROM_ISR();
        }
        portEXIT_CRITICAL(&NEStimerMux);
    }
    
    NesController::NesController(){}
    
    bool NesController::begin(){
        //NEStimerMux= portMUX_INITIALIZER_UNLOCKED;
        Wire.begin();
        
        xTaskCreatePinnedToCore(NesController::_NESTask, "_NESTask", 1024, this,1, &_NesTaskHandle,1);
        //nespad.updated=false;
        Wire.beginTransmission(I2C_Addr);
        if (!i2c_writeRegister( I2C_Addr, 0xF0, 0x55)) { return false; }
        delay(10);
        if (!i2c_writeRegister(I2C_Addr, 0xFB, 0x00)) {return false; }
        delay(20);
        
        
        // put your setup code here, to run once:
        timerNES = timerBegin(3, 80, true);
        timerAttachInterrupt(timerNES, &NesController::changeNes, true);
        timerAlarmWrite(timerNES, timerInterval, true);
        timerAlarmEnable(timerNES);
        return true;
    }
    
     _nespad NesController::getpad()
            {
                _nespad pad={
                    .a=nespad.a,
                    .b=nespad.b,
                    .start=nespad.start,
                    .select=nespad.select,
                    .up=nespad.up,
                    .down=nespad.down,
                    .left=nespad.left,
                    .right=nespad.right,
                    .updated=false,
                    .used=false};
                if(nespad.updated)
                {
                    nespad.updated=false;
                    pad.used=true;
                        return pad;
                }
                else
                {
                    pad.used=false;
                   return pad;
                }
                
            }

