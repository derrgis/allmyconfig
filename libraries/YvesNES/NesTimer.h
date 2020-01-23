//
//  NesTimer.h
//  
//
//  Created by Yves BAZIN on 26/12/2019.
//

#ifndef NesTimer_h
#define NesTimer_h
#include <Arduino.h>
#include <Wire.h>


int _NestimerInterval=20000;

static  TaskHandle_t _NesTimerNesHandle = 0;

static  TaskHandle_t _NesTimerNesuserTaskHandle = 0;
struct _nespad{
    bool a;
    bool b;
    bool start;
    bool select;
    bool up;
    bool down;
    bool left;
    bool right;
    bool updated;
    bool used;
};
volatile _nespad nespad={.a=false,
    .b=false,
    .start=false,
    .select=false,
    .up=false,
    .down=false,
    .left=false,
    .right=false,
    .updated=true,
    .used=false,
};

//static inline    _nespad nespad;
const long I2C_ConversionDelay = 175;  // Microseconds, ~200 on AVR
const uint8_t I2C_Addr = 0x52;
uint16_t BUTTON_UP=0x100;
uint16_t BUTTON_DOWN=0x40;
uint16_t BUTTON_RIGHT=0x80;
uint16_t BUTTON_LEFT=0x200;
uint16_t BUTTON_SELECT=0x10;
uint16_t BUTTON_START=0x4;
uint16_t BUTTON_A=0x1000;
uint16_t BUTTON_B=0x4000;
static const uint8_t requestSize = 6;
static hw_timer_t * _NesTimertimerNES = NULL;

portMUX_TYPE DRAM_ATTR _NesTimertimerMux = portMUX_INITIALIZER_UNLOCKED;
 void IRAM_ATTR _NesTimerchangeNes()
{
    portENTER_CRITICAL(&_NesTimertimerMux);
    if ( _NesTimerNesuserTaskHandle==0)
    {
        _NesTimerNesuserTaskHandle = xTaskGetCurrentTaskHandle();
        BaseType_t xHigherP=pdFALSE;
        // -- Trigger the show task
        vTaskNotifyGiveFromISR(_NesTimerNesHandle,&xHigherP);
        if(xHigherP)
            portYIELD_FROM_ISR();
    }
    portEXIT_CRITICAL(&_NesTimertimerMux);
}


class NesTimer
{
    
public:

    


 
 
    
    
    static void init()
    {
        Wire.begin();
        
        xTaskCreatePinnedToCore(_NesTimerNes, "_NesTimerNes", 8192, (void *)&nespad,1, &_NesTimerNesHandle,1);
        nespad.updated=false;
        Wire.beginTransmission(I2C_Addr);
        uint8_t      error = Wire.endTransmission();
        if (error == 0) {
            // device found
            //Serial.printf(" %02x", address);
            // sprintf(buff, " %02x", address);
            Serial.print("OK");
        } else if (error == 4) {
            // other error
            Serial.print(" XX");
        } else {
            // error = 2: received NACK on transmit of address
            // error = 3: received NACK on transmit of data
            Serial.print(" --");
        }
        
        
        
        if (!i2c_writeRegister( I2C_Addr, 0xF0, 0x55)) {  }
        delay(10);
        if (!i2c_writeRegister(I2C_Addr, 0xFB, 0x00)) { }
        delay(20);
        
        
        // put your setup code here, to run once:
        _NesTimertimerNES = timerBegin(0, 80, true);
        timerAttachInterrupt(_NesTimertimerNES, &_NesTimerchangeNes, true);
        timerAlarmWrite(_NesTimertimerNES, _NestimerInterval, true);
        timerAlarmEnable(_NesTimertimerNES);

    }
    
    
    

    
    
    static boolean isUpdated()
    {
        return nespad.updated;
    }


   static boolean verifyData(const uint8_t * dataIn, uint8_t dataSize) {
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
    
    static inline boolean i2c_writePointer( byte addr, byte ptr) {
        Wire.beginTransmission(addr);
        Wire.write(ptr);
        return Wire.endTransmission() == 0;  // 0 = No Error
    }
    
    static inline boolean i2c_requestMultiple( byte addr, uint8_t requestSize, uint8_t * dataOut) {
        uint8_t nBytesRecv = Wire.readBytes(dataOut,
                                            Wire.requestFrom(addr, requestSize));
        
        return (nBytesRecv == requestSize);  // Success if all bytes received
    }
    
    static inline boolean i2c_readDataArray( byte addr, byte ptr, uint8_t requestSize, uint8_t * dataOut) {
        if (!i2c_writePointer( addr, ptr)) { return false; }  // Set start for data read
        delayMicroseconds(I2C_ConversionDelay);  // Wait for data conversion
        return i2c_requestMultiple(addr, requestSize, dataOut);
    }
    
    static inline boolean i2c_writeRegister( byte addr, byte reg, byte value) {
        Wire.beginTransmission(addr);
        Wire.write(reg);
        Wire.write(value);
        return Wire.endTransmission() == 0;
    }
    static void _NesTimerNes(void *pvParameters){}
    
    
};

#endif /* NesTimer_h */
