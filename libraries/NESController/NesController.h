//
//  NesController.h
//  
//
//  Created by Yves BAZIN on 15/12/2019.
//
#
#ifndef NesController_h
#define NesController_h
#include <Wire.h>
#include <Arduino.h>



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

 static portMUX_TYPE DRAM_ATTR NEStimerMux= portMUX_INITIALIZER_UNLOCKED;
static  TaskHandle_t _NesTaskHandle ;
static  TaskHandle_t _NesuserTaskHandle; 
class NesController {
    
   
public:
 
    
  static  const long I2C_ConversionDelay = 175;  // Microseconds, ~200 on AVR
  static  const uint8_t I2C_Addr = 0x52;
  static const uint16_t BUTTON_UP=0x100;
  static  const uint16_t BUTTON_DOWN=0x40;
  static const uint16_t BUTTON_RIGHT=0x80;
  static const uint16_t BUTTON_LEFT=0x200;
  static  const uint16_t BUTTON_SELECT=0x10;
   static const uint16_t BUTTON_START=0x4;
   static const uint16_t BUTTON_A=0x1000;
   static const uint16_t BUTTON_B=0x4000;
    static const uint8_t requestSize = 6;
  
   static const int timerInterval=20000;


     hw_timer_t * timerNES = NULL;

       volatile  _nespad nespad;
    
    static boolean verifyData(const uint8_t * dataIn, uint8_t dataSize);
    
    static inline boolean i2c_writePointer( byte addr, byte ptr) ;
    
   static  inline boolean i2c_requestMultiple( byte addr, uint8_t requestSize, uint8_t * dataOut);
    
   static inline boolean i2c_readDataArray( byte addr, byte ptr, uint8_t requestSize, uint8_t * dataOut);
   static inline boolean i2c_writeRegister( byte addr, byte reg, byte value) ;
    
      _nespad getpad();
   
    
    static void _NESTask(void *pvParameters);
    
    
     static void   changeNes();
    
    
    NesController();
    
    bool begin();
    

};

#endif /* NesController_h */
