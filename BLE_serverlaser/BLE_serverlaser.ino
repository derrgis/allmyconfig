/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <HCSR04.h>
#include "SoundData.h";
#include "Game_Audio.h";
#include <rom/rtc.h>
#include <esp_sleep.h>
#include "soc/efuse_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"
#include "esp32-hal-dac.h"
#include <driver/dac.h>
Game_Audio_Class GameAudio(25,0); 
Game_Audio_Wav_Class pmDeath(pacmanDeath); // pacman dyingsound
Game_Audio_Wav_Class pmWav(pacman); // pacman theme   
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"



#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
#define LOGO16_GLCD_HEIGHT 15 
#define LOGO16_GLCD_WIDTH  8 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{  
  B00011000,
  B00010100, 
  B00010010, 
  B00010001, 
  B10010010, 
  B01010100, 
  B00111000,
  B00010000, 
  B00111000, 
  B01010100,
  B10010010, 
  B00010001, 
  B00010010,
  B00010100,
  B00011000, };
  static const unsigned char PROGMEM logo16_glcd_bmp2[] =
{  
  B00011000,
  B00010100, 
  B00010010, 
  B00010001, 
  B10010010, 
  B01010100, 
  B00111000,
  B01010000, 
  B00111000, 
  B01010100,
  B10010010, 
  B00010001, 
  B00010010,
  B00010100,
  B00011000, };

    static const unsigned char PROGMEM logo16_glcd_bmp3[] =
{  
  B01111110,
  B11111111, 
  B11111111, 
  B10000001, 
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B10000001, 
  B10000001,
  B10000001,
  B10000001,
  B11111111,
  B11100111,
  B01111110, };
  
 static const unsigned char PROGMEM batterie[] =
 {
B00000111,B11111111,B11111000,
B00001000,B00000000,B00000100,
B00001000,0x0,B0100,
B00111000,0x0,B0100,
B01111000,0x0,B0100,
B01111000,0x0,B0100,
B00111000,0x0,B0100,
B00001000,0x0,B0100,
B00001000,B00000000,B00000100,
B00000111,B11111111,B11111000,
 };
//#include "XT_DAC_Audio.h";
BLECharacteristic *pCharacteristic;

//XT_DAC_Audio_Class DacAudio(25,1);          // Create the main player class object. Use GPIO 25, one of the 2 DAC pins and timer 0

//XT_Wav_Class ForceWithYou(Force);           // create WAV object and pass in the WAV data
                                      
//XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_PIANO,1);  // The music score object, pass in the Music data

//XT_MultiPlay_Class Sequence; 
//UltraSonicDistanceSensor distanceSensor(12,14);
bool deviceConnected = false;
float txValue = 0;
const int readPin = 32; // Use GPIO number. See ESP32 board pinouts
const int LED = 2; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.
bool canBip=false;
bool canBipHigh=false;
//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/


#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
        
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
             
    }
};


void bip()
{
  while(GameAudio.PlayWav(&pmWav, true, 1.0)==0)
  {
  
  }
  delay(500);
  GameAudio.StopPlaying();
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
   CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
  digitalWrite(25,LOW);
   pinMode(25,INPUT_PULLUP);

   digitalRead(25);
   dac_i2s_disable();
   dac_output_disable(DAC_CHANNEL_1);
}
long lowValue;
long highValue=1000;
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      //lowValue=txValue;

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();
      if(rxValue.find("SETLOW")!=-1)
      {
        Serial.print("setting lowvalue");
        lowValue=txValue;
        canBip=true;
        bip();
      }
      if(rxValue.find("SETHIGH")!=-1)
      {
        Serial.print("setting highvalue");
        highValue=txValue;
        canBipHigh=true;
        bip();
        
      }

        // Do stuff based on the command received from the app
        if (rxValue.find("A") != -1) { 
          Serial.println("Turning ON!");
          digitalWrite(LED, HIGH);
        }
        else if (rxValue.find("B") != -1) {
          Serial.println("Turning OFF!");
          digitalWrite(LED, LOW);
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};


void setup() {
  lowValue=1000;

 pinMode(33,ANALOG);
  Serial.begin(115200);
  GameAudio.Volume=255;
                   // This needs only be in your main loop once, suggest here at the top.
//  DacAudio.Play(&Music);  
  //pinMode(LED, OUTPUT);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay(); 
display.display();
Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    //while(1);
  }
  // Clear the buffer.
  display.clearDisplay();
  // Create the BLE Device
  BLEDevice::init("Squat Help Laser"); // Give it a name
 
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
 // DacAudio.FillBuffer();
  while(GameAudio.PlayWav(&pmWav, true, 1.0)==0)
  {
  
  }
  delay(2500);
  GameAudio.StopPlaying();
}

void loop() {

  // txValue=1000;
   
//DacAudio.FillBuffer(); 
 
   VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    //Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    //Serial.println(" out of range ");
  }
  display.clearDisplay();
 
    display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,16);
  
  display.printf("VALUE:%d\nSET L:%d\nSET H:%d",(measure.RangeMilliMeter/10),lowValue,highValue);
   display.setCursor(0,0);
   int d=analogRead(33);
   
   uint16_t d2=0;
   uint8_t p1=0;
   uint8_t p2=0x80;
   d2=(int)((((float)d*(2.35-1.021)/(2760-1110))+0.13)*14/2.35);
   Serial.printf("analog:%f:%d\n",((((float)d*(2.35-1.021)/(2760-1110))+0.13)),d);
   if(d2>0)
   {
     if(d2>=8)
     {
      p1=0xFF;
      p2=p2 | ((1<<(d2-8))-1);
     }
     else
     {
      p1= ((1<<(d2-1))-1);
      
     }
   }
   uint8_t df[]={p2,p1,
   p2,p1,
   p2,p1,
   p2,p1,
   p2,p1,
   p2,p1};
  // display.drawBitmap(127-22, 1,  batterie, 22, 10, 1);
   display.drawBitmap(127-22, 1,  batterie, 22, 10, 1);
   display.drawBitmap(127-22+4, 3,  df, 16, 6, 1); 
   
   
   
    // Fabricate some arbitrary junk for now...
    txValue = measure.RangeMilliMeter/10;
       // display.drawBitmap(0, 0,  logo16_glcd_bmp, 8, 15, 1);
  if (deviceConnected) {
           
    /* display.setTextSize(1);
      display.setTextColor(BLACK,WHITE);
     display.print("C");*/
      display.setCursor(0,0);
    display.drawBitmap(0, 0,  logo16_glcd_bmp3, 8, 15, 1);
   // display.setCursor(0,0);
  // This could be an actual sensor reading!

    // Let's convert the value to a char array:
    char txString[10]; // make sure this is big enuffz
    dtostrf(txValue, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer
    
//    pCharacteristic->setValue(&txValue, 1); // To send the integer value
//    pCharacteristic->setValue("Hello!"); // Sending a test message
    pCharacteristic->setValue(txString);
    
    pCharacteristic->notify(); // Send the value to the app!
    //Serial.print("*** Sent Value: ");
    //Serial.print(txString);
    //Serial.println(" ***");

    // You can add the rxValue checks down here instead
    // if you set "rxValue" as a global var at the top!
    // Note you will have to delete "std::string" declaration
    // of "rxValue" in the callback function.
//    if (rxValue.find("A") != -1) { 
//      Serial.println("Turning ON!");
//      digitalWrite(LED, HIGH);
//    }
//    else if (rxValue.find("B") != -1) {
//      Serial.println("Turning OFF!");
//      digitalWrite(LED, LOW);
//    }
  }
  else
  {

  
  display.setCursor(0,0);
          // display.drawBitmap(0, 0,  logo16_glcd_bmp3, 8, 15, 0);
    display.drawBitmap(0, 0,  logo16_glcd_bmp, 8, 15, 1);
    //Serial.println("off");
  }

 display.display();
  if(txValue<=lowValue and canBip==true and txValue!=-1)
{
  bip();
  Serial.println("done"); 
  canBip=false;
  
}

  if(txValue>=highValue and canBipHigh==true and txValue!=-1)
{
  bip();
  Serial.println("done"); 
  canBipHigh=false;
  
}

if(txValue>lowValue+10)
{
  canBip=true;
}


if(txValue<highValue-10)
{
  canBipHigh=true;
}

  delay(100);
}
