#include <Wire.h>
const long I2C_ConversionDelay = 175;  // Microseconds, ~200 on AVR
  const uint8_t I2C_Addr = 0x52; 
uint8_t dataOut[40];
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
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
Wire.begin();



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
    
}



void loop() {

    i2c_readDataArray( I2C_Addr,0x00,requestSize,  dataOut);
    verifyData(dataOut,requestSize);
    //Serial.println("new thread");
    if(dataOut[4]!=255 || dataOut[5]!=255)
    {
      Serial.printf("read %d %d \n",dataOut[4],dataOut[5]);
    }
    dataOut[4]=255;
    dataOut[5]=255;
    
    
  // put your main code here, to run repeatedly:

}
