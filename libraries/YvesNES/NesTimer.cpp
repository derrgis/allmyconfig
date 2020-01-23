//
//  NesTimer.cpp
//  
//
//  Created by Yves BAZIN on 26/12/2019.
//

#include <stdio.h>
#include "NesTimer.h"
volatile bool toto;

void NesTimer::_NesTimerNes(void *pvParameters)
{
    
    _nespad *nespad =(_nespad *)pvParameters;
    
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
        //Serial.println("her");
        
        
        uint8_t dataOut[40];
        nespad->updated=true;
        toto=true;
        // nespad.used=true;
        Serial.println("e");
        i2c_readDataArray( 0x52,0x00,6,  dataOut);
        verifyData(dataOut,requestSize);
        
        if(dataOut[4]!=255 || dataOut[5]!=255)
        {
            
            uint16_t value=~*((uint16_t *)&dataOut[4]);
            //Serial.printf("date %x %x %x\n",~*((uint16_t *)&dataOut[4]),~dataOut[4],~dataOut[5]);
            nespad->a= (BUTTON_A & value) >0 ? true:false;
            nespad->b=(BUTTON_B & value) >0 ? true:false;
            nespad->start=(BUTTON_START & value) >0 ? true:false;
            nespad->select=(BUTTON_SELECT & value) >0 ? true:false;
            nespad->up=(BUTTON_UP & value) >0 ? true:false;
            nespad->down=(BUTTON_DOWN & value) >0 ? true:false;
            nespad->left=(BUTTON_LEFT & value) >0 ? true:false;
            nespad->right=(BUTTON_RIGHT & value) >0 ? true:false;
            nespad->updated=true;
            Serial.println("we have something");
            
            
        }
        // delay(10);
        _NesTimerNesuserTaskHandle=0;
        //nespad.used=false;
        
        
    }
}
