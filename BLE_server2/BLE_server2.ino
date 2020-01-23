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
#include "soc/rtc.h"
Game_Audio_Class GameAudio(25,0); 

Game_Audio_Wav_Class pmDeath(pacmanDeath); // pacman dyingsound
Game_Audio_Wav_Class pmWav(pacman); // pacman theme   
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pCharacteristicTX;
    BLECharacteristic *pCharacteristicRX;

//#include "XT_DAC_Audio.h";
BLECharacteristic *pCharacteristic;
/*int8_t PROGMEM TwinkleTwinkle[] = {
 NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
  NOTE_SILENCE,BEAT_5,SCORE_END
};
XT_DAC_Audio_Class DacAudio(25,1);          // Create the main player class object. Use GPIO 25, one of the 2 DAC pins and timer 0

//XT_Wav_Class ForceWithYou(Force);           // create WAV object and pass in the WAV data
                                      
XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_PIANO,1);  // The music score object, pass in the Music data

XT_MultiPlay_Class Sequence; */
UltraSonicDistanceSensor distanceSensor(14,12);
bool soundfix=false;
bool deviceConnected = false;
float txValue = 0;
const int readPin = 32; // Use GPIO number. See ESP32 board pinouts
const int LED = 2; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.
bool canBip=false;
bool canBipHigh=false;
//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//"6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID


#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" //"6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("coon");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void bip()
{
  /*
  for(int i=1;i<1000000;i++)
  {
  DacAudio.FillBuffer();
  }
  DacAudio.Play(&Music);  */
  while(GameAudio.PlayWav(&pmWav, true, 1.0)==0)
  {
  
  }
  delay(500);
  GameAudio.StopPlaying();
}
long lowValue;
long highValue;
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

bool isOff=false;
void setup() {
  lowValue=1000;
  highValue=1000;

  GameAudio.Volume=65;
  Serial.begin(115200);
                   // This needs only be in your main loop once, suggest here at the top.
 // DacAudio.Play(&Music);  
  //pinMode(LED, OUTPUT);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay(); 
display.display();
 display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("Squat Helper\nStarting");
  display.display();
  // Create the BLE Device
  BLEDevice::init("Blynk"); // Give it a name
/*
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
pServer->getAdvertising()->addServiceUUID(pService->getUUID());
  // Start advertising
  pServer->getAdvertising()->start();
  */

        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new MyServerCallbacks());
        
        // Create the BLE Service
        pService = pServer->createService(SERVICE_UUID);
        
        // Create a BLE Characteristic
        pCharacteristicTX = pService->createCharacteristic(
                                                           CHARACTERISTIC_UUID_TX,
                                                           BLECharacteristic::PROPERTY_NOTIFY
                                                           );
        
        pCharacteristicTX->addDescriptor(new BLE2902());
        
        pCharacteristicRX = pService->createCharacteristic(
                                                           CHARACTERISTIC_UUID_RX,
                                                           BLECharacteristic::PROPERTY_WRITE
                                                           );
        
        pCharacteristicRX->setCallbacks(new MyCallbacks());
        
        // Start the service
        pService->start();
        
        BLEAdvertising* advertising = pServer->getAdvertising();
        
        BLEAdvertisementData advertisingData;
        advertisingData.setCompleteServices(BLEUUID(SERVICE_UUID));
        advertising->setAdvertisementData(advertisingData);
       // Serial.println("e");
        // Start advertising
        advertising->start();
  Serial.println("Waiting a client connection to notify...");
     pinMode(19,INPUT);
pinMode(18 ,INPUT);
pinMode(05,INPUT);
   pinMode(15,OUTPUT);
   digitalWrite(15,LOW);
    pinMode(15,INPUT);
  while(GameAudio.PlayWav(&pmWav, true, 1.0)==0)
  {
  
  }
  delay(2500);
  GameAudio.StopPlaying();
  
 display.clearDisplay(); 
display.display();
 display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("Squat Helper\nReady");
  display.display();
  delay(1000);
  //DacAudio.FillBuffer();
}

void loop() {

   txValue=1000;

    txValue = distanceSensor.measureDistanceCm(); // This could be an actual sensor reading!
 
 if(!soundfix)
 {
   display.clearDisplay();
 display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf("Current:%d\n",(int)txValue);
 }
   // display.display();
    if(digitalRead(18)==HIGH)
  {

    if(soundfix)
    {

      if(GameAudio.Volume>10)
        GameAudio.Volume-=10;
         display.clearDisplay();
          display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.printf("SOUND\n%d",GameAudio.Volume);
        display.display();
    }
    else
    {
      Serial.println("on a appuyer sur 18");
      lowValue=txValue;
    }
    bip();
  }
  // put your main code here, to run repeatedly:
  ///Serial.println(digitalRead(05));
  if(digitalRead(05)==HIGH)
  {
    if(soundfix)
    {

      if(GameAudio.Volume<244)
        GameAudio.Volume+=10;
       
         display.clearDisplay();
          display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.printf("SOUND\n%d",GameAudio.Volume);
        display.display();
    }
    else
    {
    Serial.println("on a appuyer sur 05");
        highValue=txValue;
        
       
    }
    // display.display();
    bip();
  }
  // put your main code here, to run repeatedly:
 // Serial.println(digitalRead(17));
  if(digitalRead(15)==HIGH)
  {
    if(soundfix)

    {
          display.clearDisplay();
          display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.printf("Squat Helper\nSTOP..");
        display.display();
          GameAudio.PlayWav(&pmDeath, true, 1.0);
          while(GameAudio.IsPlaying())
        {   
        }
        delay(1000);
           display.ssd1306_command(SSD1306_DISPLAYOFF); 
            Serial.println(esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1));
            Serial.println("Going to sleep now");
        esp_deep_sleep_start();
    }

    else
    {
      display.clearDisplay();
          display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.printf("SOUND\n%d",GameAudio.Volume);
        display.display();
        soundfix=true;
        delay(1000);
    }
  }
  
 if(digitalRead(19)==HIGH)
  {
    if(soundfix)
      soundfix=false;
      else
      {
        highValue=1000;
        lowValue=1000;
      }
    bip();
  }

if(!soundfix)
    {
        if(lowValue==1000)
        {
          display.printf("low:not set\n");
        }
        else
         display.printf("low:%d\n",(int)lowValue);
         if(highValue==1000)
         display.printf("high:not set\n");
         else
          display.printf("high:%d",(int)highValue);
        display.display();
    }
  
//DacAudio.FillBuffer(); 
  if (deviceConnected) {
    // Fabricate some arbitrary junk for now...
    display.setCursor(120,0);
  display.printf("C");
  display.display();
    // Let's convert the value to a char array:
    char txString[10]; // make sure this is big enuffz
    dtostrf(txValue, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer
    
//    pCharacteristic->setValue(&txValue, 1); // To send the integer value
//    pCharacteristic->setValue("Hello!"); // Sending a test message
    pCharacteristicTX->setValue(txString);
    
    pCharacteristicTX->notify(); // Send the value to the app!
    Serial.print("*** Sent Value: ");
    Serial.print(txString);
    Serial.println(" ***");

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
 
    if(!deviceConnected)
    {
      display.setTextColor(BLACK);
         display.setCursor(120,0);
    display.printf("C");
    display.display();
    }
  
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
