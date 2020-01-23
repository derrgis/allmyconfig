#include <Wire.h>
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
boolean verifyData(const uint8_t * dataIn, uint8_t dataSize) {
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

  inline boolean i2c_writePointer( byte addr, byte ptr) {
    Wire.beginTransmission(addr);
    Wire.write(ptr);
    return Wire.endTransmission() == 0;  // 0 = No Error
  }
  
inline boolean i2c_requestMultiple( byte addr, uint8_t requestSize, uint8_t * dataOut) {
    uint8_t nBytesRecv = Wire.readBytes(dataOut,
      Wire.requestFrom(addr, requestSize));

    return (nBytesRecv == requestSize);  // Success if all bytes received
  }

inline boolean i2c_readDataArray( byte addr, byte ptr, uint8_t requestSize, uint8_t * dataOut) {
    if (!i2c_writePointer( addr, ptr)) { return false; }  // Set start for data read
    delayMicroseconds(I2C_ConversionDelay);  // Wait for data conversion
    return i2c_requestMultiple(addr, requestSize, dataOut);
}

  inline boolean i2c_writeRegister( byte addr, byte reg, byte value) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
  }


hw_timer_t * timerNES = NULL;

portMUX_TYPE DRAM_ATTR timerMux = portMUX_INITIALIZER_UNLOCKED;
struct nespoad{
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

volatile nespoad nespad;
int timerInterval=20000;

static  TaskHandle_t FastLEDshowTaskHandle = 0;
static  TaskHandle_t FastLEDshowTaskHandle2 = 0;
static  TaskHandle_t userTaskHandle = 0;

void FastLEDshowTask2(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
           
                     
         
           uint8_t dataOut[40];
          nespad.updated=false;
         // nespad.used=true;
           i2c_readDataArray( 0x52,0x00,6,  dataOut);
            verifyData(dataOut,requestSize);
           
            if(dataOut[4]!=255 || dataOut[5]!=255)
            {
              
              uint16_t value=~*((uint16_t *)&dataOut[4]);
              //Serial.printf("date %x %x %x\n",~*((uint16_t *)&dataOut[4]),~dataOut[4],~dataOut[5]);
                nespad.a= (BUTTON_A & value) >0 ? true:false; 
                nespad.b=(BUTTON_B & value) >0 ? true:false; 
                nespad.start=(BUTTON_START & value) >0 ? true:false; 
                nespad.select=(BUTTON_SELECT & value) >0 ? true:false; 
                nespad.up=(BUTTON_UP & value) >0 ? true:false; 
                nespad.down=(BUTTON_DOWN & value) >0 ? true:false; 
                nespad.left=(BUTTON_LEFT & value) >0 ? true:false; 
                nespad.right=(BUTTON_RIGHT & value) >0 ? true:false; 
                nespad.updated=true;

              
            }
           // delay(10);
       userTaskHandle=0;
        //nespad.used=false;
                 
                
           }
}

void IRAM_ATTR changeNes()
 {
   portENTER_CRITICAL(&timerMux);
  if ( userTaskHandle==0)
   {
   userTaskHandle = xTaskGetCurrentTaskHandle();
          BaseType_t xHigherP=pdFALSE;
          // -- Trigger the show task
          vTaskNotifyGiveFromISR(FastLEDshowTaskHandle2,&xHigherP);
          if(xHigherP)
          portYIELD_FROM_ISR();
   }
   portEXIT_CRITICAL(&timerMux);
 }

void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:
Wire.begin();

   xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 8192, NULL,1, &FastLEDshowTaskHandle2,1);
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
timerNES = timerBegin(0, 80, true);
timerAttachInterrupt(timerNES, &changeNes, true);
timerAlarmWrite(timerNES, timerInterval, true);
timerAlarmEnable(timerNES);


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("jjkjk");
  delay(100);
if(nespad.updated==true)
{
  Serial.printf("jj");
  //nespad.used=true;
  char str[30];
  sprintf(str,"%s%s%s%s%s%s%s%s", nespad.a ? "A":"_",nespad.b ? "B":"_",nespad.up ? "U":"_",nespad.down ? "D":"_",nespad.left ? "L":"_",nespad.right ? "R":"_",nespad.select ? "SEL":"___",nespad.start ? "STR":"___");
  /*sprintf(str,"%s%s", str,nespad.b ? "B":" ");
  sprintf(str,"%s%s", str,nespad.up ? "U":" ");
  sprintf(str,"%s%s", str,nespad.down ? "D":" ");
  sprintf(str,"%s%s", str,nespad.left ? "L":" ");
  sprintf(str,"%s%s", str,nespad.right ? "R":" ");
  sprintf(str,"%s%s", str,nespad.select ? "SEL":"   ");
  sprintf(str,"%s%s", str,nespad.start ? "STR":" ");*/
  Serial.printf("on a :%s\n",str);
  nespad.updated=false;
  // nespad.used=false;
} 


 
}



