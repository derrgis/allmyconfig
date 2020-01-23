// An example of the SdFatSoftSpi template class.
// This example is for an Adafruit Data Logging Shield on a Mega.
// Software SPI is required on Mega since this shield connects to pins 10-13.
// This example will also run on an Uno and other boards using software SPI.
//

#define ENABLE_SOFTWARE_SPI_CLASS 1

#include <SPI.h>
#include "SdFat.h"
#if ENABLE_SOFTWARE_SPI_CLASS  // Must be set in SdFat/SdFatConfig.h
//
// Pin numbers in templates mu2st be constants.
const uint8_t SOFT_MISO_PIN = 22;
const uint8_t SOFT_MOSI_PIN = 2;
const uint8_t SOFT_SCK_PIN  =4;
//
// Chip select may be constant or RAM variable.
const uint8_t SD_CHIP_SELECT_PIN =5; 

// SdFat software SPI template
SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

// Test file.
SdFile file;
SdFile file2;


void testFileIO( const char * path){
  
  // File file2;// = fs.open(path);
    static uint8_t buf[80*32*3];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
   /* if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 80*32*3){
                toRead = 80*32*3;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }*/


    
    if(!file2.open(path, O_CREAT | O_RDWR)){
      //Serial.begin(115200);
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<1000; i++){
        file2.write(buf,48*123*3 );
    }
    end = millis() - start;
   // Serial.begin(115200);
    Serial.printf("time for one frame %f ms\n", (float)end/1000);
    file2.close();

        
    if(!file2.open(path, O_CREAT | O_RDWR)){
        Serial.println("Failed to open file for writing");
        return;
    }

    //size_t i;
    start = millis();
    for(i=0; i<1000; i++){
        file2.write(buf,510 );
    }
    end = millis() - start;
    Serial.printf("time for one universe %f ms\n", (float)end/1000);
    file2.close();
}


void setup() {
  Serial.begin(115200);
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  Serial.println("Type any character to start");


  if (!sd.begin(SD_CHIP_SELECT_PIN)) {
    sd.initErrorHalt();
  }

  if (!file.open("SoftSPI.txt", O_CREAT | O_RDWR)) {
    sd.errorHalt(F("open failed"));
  }
  file.println(F("This line was printed using software SPI."));

  file.rewind();
  
  while (file.available()) {
    Serial.write(file.read());
  }

  file.close();

 testFileIO( "test.txt");

  Serial.println(F("Done."));
}
//------------------------------------------------------------------------------
void loop() {}
#else  // ENABLE_SOFTWARE_SPI_CLASS
#error ENABLE_SOFTWARE_SPI_CLASS must be set non-zero in SdFat/SdFatConfig.h
#endif  //ENABLE_SOFTWARE_SPI_CLASS
