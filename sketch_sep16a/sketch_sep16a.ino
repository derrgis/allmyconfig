//**********************************************************************************************************//
// With this project its possible to drive Ws2812 Leds with Artnet over Wifi (with Jinx! or Glediator)      //
// and it´s also possible to switch to the SD-card version where animations can be recorded from Artnet and // 
// than be played back. Although a Matrix-clock can be switched on and off over the animations.             //
// You will need the Fastled Library from Samguyer https://github.com/samguyer/FastLED                      //
// and the Artnet-library of Yves Bazin: https://github.com/hpwit/artnet                                    //
//                                                                                                          //
// Created by Stefan Weickinger & Yves BAZIN 2018                                                           //
//**********************************************************************************************************//


//-------------------Set RTC-------------------------------------------------------
#include "time.h"


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//------------------------------ WiFi details------------------------------

const char *ssid  = "3WebCube568B";           //Setup your Wifi
const char *pass = "dcF52eC5";
//char ssid[] = "3WebCube568B";
//char pass[] = "dcF52eC5";
//const char *ssid     = "Weix";
//const char *password = "asdf1234!";

/*IPAddress local_ip(172, 20, 10, 10);
 IPAddress gateway_ip(172, 20, 10, 1);
 IPAddress subnet_ip(255, 255, 255, 240);
 */

IPAddress local_ip(192,168,137,10);
IPAddress gateway_ip(192,168,137,1);
IPAddress subnet_mask(255, 255, 255, 0);
IPAddress dns_ip(8,8,8,8);
byte arduino_mac[] = { 0x30, 0xAE, 0xA4, 0x18, 0x38, 0x94 };

//------------------------Wifi variables ------------------------------
bool isWifiConnected=false;
bool isBlynkConnected=false;
bool isTryingToConnect=false;
static TaskHandle_t checkWifiTaskHandle;
static TaskHandle_t userTaskHandle = 0;
int WAIT_WIFI_MAX=100;
int WAIT_WIFI = 0;

static bool wifistatus=true;
//----------------------------------BLYNK------------------------------------------

#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
/*
 #define BLYNK_USE_DIRECT_CONNECT
 
 #include <BlynkSimpleEsp32_BLE.h>
 #include <BLEDevice.h>
 #include <BLEServer.h>
 */
//char auth[] = "4ef690bc3df245299df2307ee2ea8b41";
char auth[] = "7a38277d88df4359ae5e62cc646e8105";

#include <SimpleTimer.h> 
SimpleTimer timer;
//--------------------------------Clock display---------------------------------

#include <Adafruit_GFX.h>
#include <FastLED.h>
#include <FastLED_NeoMatrix.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS1307.h>
RtcDS1307<TwoWire> Rtc(Wire);


#define mw 80
#define mh 32
#define NUMMATRIX (mw*mh)
#define LED_RUN_LENGTH 16//number of leds per column

CRGB matrixleds[NUMMATRIX];

FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(matrixleds,16,16,5,2,
                                                  NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS +NEO_TILE_ZIGZAG+
                                                  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                                                  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG );

const uint16_t colors[] = {
    matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255) };

//-------------------------------

#include <WiFi.h>
//#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <Artnet.h>

#include "SD.h"
#include "SPI.h"
//#define FASTLED_ALLOW_INTERRUPTS 0
//#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0


#define LED_WIDTH 80
#define LED_HEIGHT 32
#define NUM_LEDS LED_WIDTH*LED_HEIGHT
#define UNIVERSE_SIZE 170
#define PIXEL_PER_PIN NUM_LEDS/5
/*we will  use 5 pins each pin will drive two 16x16 panel hence 512 pix  it will take 15ms to refresh your entire panel instead of 76ms
 hence having a refresh rate of 65fps instead of 13fps*/
#define PIN1 15   //2    /(TDO)
#define PIN2 4    //4
#define PIN3 27
#define PIN4 25   //12 (TDI)
#define PIN5 13
//CRGB leds[NUM_LEDS];
#define BUFFER_SIZE 10
uint8_t readbuffer[NUM_LEDS*3];
//char filename[256];                                               // ?
char READ_NAME[]="savedata"; //the name of your save
Artnet artnet;

//Relay---
#define relay_pin 0


File root;
static File  myFile;

int blynk_wait, blynk_wait_max=0;
//int blynk_wait_max=400;

#define BRIGHTNESS bright

char datestring[20];
char blank[20];

int MatrixTime=0;
int Screen=0;
int bright=100;
int nextFile=1;

int color=0;
#define MILLI_AMPS 35000 // 35A max


//----------------------------RECORDING---------------------
char filename[256];
char SAVE_NAME[]="savedata"; //the name of your save

static uint32_t deb=0;
static uint32_t fin=0;

static bool new_record=true;
int recordingnumber=0;

int record=0;

bool stopped=false;

static uint8_t frame[NUM_LEDS*3];                                         // ?
//-----------------------------------------------------------



int status = WL_IDLE_STATUS;

BLYNK_CONNECTED() {
   Blynk.syncAll();
   //Blynk.syncVirtual(V0, V1, V3, V4, V5);
}

BLYNK_WRITE(V0)
{ Screen=param.asInt();
}

BLYNK_WRITE(V1) //
{ MatrixTime=param.asInt();
}

BLYNK_WRITE(V2) // Slider - Brightnes
{
    bright = param.asLong();

     if(bright==0){
      digitalWrite(relay_pin, LOW);
      Serial.println("Relay LOW");
    }
    else{
      digitalWrite(relay_pin, HIGH);
      Serial.println("Relay HIGH");
    }
}

BLYNK_WRITE(V3) // switch to the new file
{
    nextFile = param.asLong();
}

BLYNK_WRITE(V4) // Slider - Clockcolor
{
    color = param.asInt();
    CRGB c=CHSV(color*128,255,color);
    matrix->setTextColor(matrix->Color(c.r, c.g, c.b));
   
}

BLYNK_WRITE(V5) // switch to the new file
{
    record = param.asInt();
}

void checkWifi()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
        // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(checkWifiTaskHandle);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}



void checkWifiTask(void *pvParameters)
{
    //Serial.printf("djdj\n");
    
    
    
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
        while(1)
        {
            
            /* if((WiFi.status() == WL_CONNECTED))
             {
             artnet.begin(NUM_LEDS+160,UNIVERSE_SIZE,1);
             if(!Blynk.connected()){
             isBlynkConnected=false;
             Blynk.config(auth);
             Blynk.connect();
             delay(500);
             isTryingToConnect=true;
             Serial.println("Trying to connect Blynk");
             while( (!Blynk.connected()) && (blynk_wait<blynk_wait_max) )
             {
             delay(500);
             blynk_wait++;
             }
             blynk_wait=0;
             
             if(Blynk.connected())
             {
             isBlynkConnected=true;
             Serial.println("Blynk connected");
             isTryingToConnect=false;
             }
             
             }
             else
             {
             isBlynkConnected=true;
             }
             }*/
            if( (WiFi.status() != WL_CONNECTED)  && new_record && wifistatus) //this should avoid the check of the wifi during the the recording
            {
                Serial.print("Trying to connect to the Wifi");
                artnet.stop();
                isWifiConnected=false;
                isBlynkConnected=false;
                isTryingToConnect=false;
                Blynk.disconnect();
                WiFi.mode(WIFI_STA);
                WiFi.config(local_ip, gateway_ip, subnet_mask, dns_ip);
                
                WiFi.begin(ssid, pass);
                while ( (WiFi.status() != WL_CONNECTED) && (WAIT_WIFI<WAIT_WIFI_MAX) )
                {
                    Serial.println(WiFi.status());
                    delay(500);
                    //Serial.print(".");
                    WAIT_WIFI++;
                }
                WAIT_WIFI=0;
                if(WiFi.status()==WL_CONNECTED)
                {
                    Serial.print("Wifi Connected");
                    isWifiConnected=true;
                    
                }
            }
            
            // memcpy(leds,Tpic,LED_WIDTH*LED_HEIGHT*sizeof(CRGB));
            delay(1);
            
            
            while(!new_record)
            {
                
                // memcpy(leds,Tpic,LED_WIDTH*LED_HEIGHT*sizeof(CRGB));
                delay(1 );
                if(artnet.nbframeread+BUFFER_SIZE-1<artnet.frameslues)        //Buffersize was 10
                {
                    fin=millis();
                    memcpy(matrixleds,artnet.getframe(),NUM_LEDS*sizeof(CRGB)); 
                    memcpy(frame,matrixleds,NUM_LEDS*sizeof(CRGB)); //to allow the display
                    myFile.write(frame,NUM_LEDS*sizeof(CRGB));
                    
                    deb=deb+millis()-fin;
                    //userTaskHandle=0; //so we can't have two display tasks at the same time
                }
                
            }
            //userTaskHandle=0;
        }
        
        
    }
    
}




void selectNextFile()
{
    /*  memset(filename, 0, 255);
     sprintf(filename,"/%s.dat",READ_NAME);
     myFile = SD.open(filename);
     */
    myFile.close();
    myFile = root.openNextFile();
    if (! myFile)
    {
        root.rewindDirectory();
        myFile = root.openNextFile();
        if(!myFile)
        {
            Serial.println("No file on the card");
        }
    }
    
}

void openNextFileForRecording()
{
    memset(filename, 0, 255);
    sprintf(filename,"/%s_%03d.dat",SAVE_NAME,recordingnumber);
    myFile = SD.open(filename); //, FILE_WRITE);
    while(myFile)
    {
        myFile.close();
        recordingnumber++;
        memset(filename, 0, 255);
        sprintf(filename,"/%s_%03d.dat",SAVE_NAME,recordingnumber);
        myFile = SD.open(filename); //, FILE_WRITE);

    }
    myFile = SD.open(filename, FILE_WRITE);
}

void numberfiles()
{
    recordingnumber=0;
    myFile = root.openNextFile();
    while(myFile)
    {
        myFile.close();
        recordingnumber++;
        myFile = root.openNextFile();
    }
    recordingnumber++;
    root.rewindDirectory();
    Serial.printf("Number of files already stored:%d\n",recordingnumber-1);
}

//-----------------------------------------------SETUP---------------------------------------------
void setup() {
    
    Serial.begin(115200);

    timer.setInterval(600000L, selectNextFile);
    
    /*
     
     //WiFi.config(local_ip, gateway_ip, subnet_ip);
     delay(100);
     WiFi.begin("Weix", "asdf1234!");
     delay(1500);
     Blynk.config(auth);
     Blynk.connect();
     //Blynk.connectWiFi(ssid, pass);
     
     */
    /*
     if (WiFi.status() == WL_CONNECTED) {
     //Blynk.connectWiFi(ssid, pass);
     
     reconnect=false;
     
     
     if (WiFi.status() != WL_CONNECTED)
     {   Blynk.disconnect();
     }
     */
    Rtc.Begin();
    RTCtime();
   
    
    //artnet.begin(NUM_LEDS+160,UNIVERSE_SIZE,1);
    //Blynk.begin(auth);
    
    
    xTaskCreatePinnedToCore(checkWifiTask, "checkWifiTask", 3000, NULL,3, &checkWifiTaskHandle, FASTLED_SHOW_CORE);
    
    
    FastLED.setBrightness(BRIGHTNESS);
    
    
    if(!SD.begin(5,SPI,80000000)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    
    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    root=SD.open("/");
    /*memset(filename, 0, 255);
     sprintf(filename,"/%s.dat",READ_NAME);
     myFile = SD.open(filename);
     */
    numberfiles(); //set the recordingnumber to the number of files present on the SD cards (this will avoid erasing stuff)
    selectNextFile();
    selectNextFile();
    

    pinMode(relay_pin, OUTPUT); 
    digitalWrite(relay_pin, HIGH);
    
    FastLED.addLeds<WS2812B, PIN1, GRB>(matrixleds, 0*PIXEL_PER_PIN, PIXEL_PER_PIN);
    FastLED.addLeds<WS2812B, PIN2, GRB>(matrixleds, 1*PIXEL_PER_PIN, PIXEL_PER_PIN);
    FastLED.addLeds<WS2812B, PIN3, GRB>(matrixleds, 2*PIXEL_PER_PIN, PIXEL_PER_PIN);
    FastLED.addLeds<WS2812B, PIN4, GRB>(matrixleds, 3*PIXEL_PER_PIN, PIXEL_PER_PIN);
    FastLED.addLeds<WS2812B, PIN5, GRB>(matrixleds, 4*PIXEL_PER_PIN, PIXEL_PER_PIN);
    
    fill_solid(matrixleds,NUM_LEDS,CRGB::Black);
    FastLED.show();
    
    FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
    
    matrix->begin();
    matrix->setTextWrap(true);
    //matrix->setBrightness(100);
    matrix->setTextColor(colors[color]);
    checkWifi(); //launching the process to check the wifi on core 0
}

bool artnetdisplay=false;


///----------------------------------------LOOP---------------------------------------------

void loop() {
    if(isBlynkConnected){
        Blynk.run();
        RTCtime();
     }

    if(color<150)
    {
    timer.run();
    }
    
    if(WiFi.status() != WL_CONNECTED)
    {Blynk.disconnect();
        Screen=0;                             //if connection lost , switch back to sd-card and time on.
        MatrixTime=0;
    }

    FastLED.setBrightness(BRIGHTNESS);
    
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);

    
    if( (Screen==0) && !isTryingToConnect)
    {//delay(10);
        
        Serial.println("SD-Card");
        
        if (myFile.available())
        {
            myFile.read(readbuffer,NUM_LEDS*sizeof(CRGB));
            memcpy(matrixleds,readbuffer,NUM_LEDS*sizeof(CRGB));
            
            if(record==1)
                {
                  //myFile.close();
                  Serial.println(myFile.name());
                  SD.remove(myFile.name());
                  selectNextFile();
                  record=0; //otherwise you would erase all the files :)
                  Serial.println("file deleted");
                }
        }
        
        else
        {
            myFile.seek(0);
        }
        
        
    }
    else if(Screen==1 && artnet.running)
    {
        
        if((WiFi.status() == WL_CONNECTED))
        {
            Serial.println("Artnet-Wifi");
            
            
            
            
            //--------------Recordingpart-----------------------------------------
            
            while(record==1){
                Blynk.run();
                if(new_record)
                {
          artnet.nbframeread=0;
          artnet.frameslues=0;
          artnet.lostframes=0;
                    if(myFile)
                        myFile.close(); //if we had one open file it needs to be closed
                   // memset(filename, 0, 255);
                    //sprintf(filename,"/%s_%03d.dat",SAVE_NAME,recordingnumber);
                    //myFile = SD.open(filename, FILE_WRITE);
                    openNextFileForRecording(); //this will avoid to override existing files
                    new_record=false;
                    //checkWifi(); you do not need to do that as this function is a while loop
                    Serial.println("start new recording");
                }
                
                if(!new_record)
                {
                     artnet.read(); 
                    if(artnet.nbframeread==0) //because we already did a getframe();
                    {
                        Serial.println("Recording has started");
                        Serial.printf("Filename:%s\n",filename);
                    }
          FastLED.show();
                }
            }
            if(!new_record)
            {
                myFile.close();
                Serial.println("Recording stopped ...");
                Serial.printf("frames saved:%d frames lues:%d lost frames:%d\n",artnet.nbframeread,artnet.frameslues,artnet.lostframes)   ;
                Serial.printf("time per frame %f\n",(float)deb/(artnet.nbframeread));
                artnet.nbframeread=0;
                artnet.frameslues=0;
                artnet.lostframes=0;
                deb=0;
                stopped=true;
                new_record=true;
                recordingnumber++;
            }
            //----------Recording END-----------------------------------------------
            
            if(artnet.read()==1)
                memcpy(matrixleds,artnet.getframe(),NUM_LEDS*sizeof(CRGB));
            
            artnet.resetsync();
        }
    }
    
    
    
    
    if(MatrixTime==0){
        Serial.println("on");
        matrix->print (datestring);
    }
    else{
        Serial.println("off");
        matrix->print (blank);
    }
    
    FastLED.show(); 
    
    
    if((WiFi.status() == WL_CONNECTED) && new_record) //to not check the wifi during recording
    {
        if(!artnet.running){
            artnet.begin(NUM_LEDS+160,UNIVERSE_SIZE,BUFFER_SIZE); //2
        }
        if(!Blynk.connected())
        {   Blynk.disconnect();
            isBlynkConnected=false;
            Blynk.config(auth);
            Blynk.connect();
            delay(500);
            isTryingToConnect=true;
            Serial.println("Trying to connect Blynk");
            while( (!Blynk.connected()) && (blynk_wait<blynk_wait_max) )
            {
                delay(500);
                blynk_wait++;
            }
            blynk_wait=0;
            
            if(Blynk.connected())
            {
                isBlynkConnected=true;
                Serial.println("Blynk connected");
                isTryingToConnect=false;
            }
            
        } 
    }
    
    if(nextFile==1)
    {
        selectNextFile();
        nextFile=0;
    }  
}




//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
///----------------------------------------Print Time ---------------------------------------------
void printDateTime(const RtcDateTime& dt)
{
    sprintf(datestring,"%02u:%02u:%02u", dt.Hour(), dt.Minute(), dt.Second());
    sprintf(blank,"");
    Serial.print(datestring);
    matrix->clear();
    matrix->setCursor(1, 1);


    if(((dt.Hour() == 22) &&  (dt.Minute() == 30) &&  (dt.Second() == 00))) //||((dt.Hour() >= 23) && (dt.Hour() < 7)))         //NIGHT
    //if(((dt.Hour() == 10) &&  (dt.Minute() == 30) &&  (dt.Second() == 00)))// ||((dt.Hour() >= 10) && (dt.Hour() < 11)))         //NIGHT-Test
   
       { bright=0;
        //digitalWrite(relay_pin, LOW);
        Serial.println("Night Relay LOW");
        
       }

    if(((dt.Hour() == 06) &&  (dt.Minute() == 30) &&  (dt.Second() == 00))) //||((dt.Hour() >= 7) && (dt.Hour() < 22)))        //DAY       
   // if(((dt.Hour() == 10) &&  (dt.Minute() == 28) &&  (dt.Second() == 00)))        //DAY-Test       
   
       { 
        bright=70;
        Serial.println("Day Relay HIGH");
        Screen=0;                             //sd-card and time on.
        MatrixTime=0;
       }       

}


void RTCtime(){
    ///----------------------------------------RTC-Set---------------------------------------------
    
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    
    Rtc.Begin();
    
    RtcDateTime compiled;
    if(getLocalTime(&timeinfo))
    {
        
        compiled =RtcDateTime(
                              (uint16_t )timeinfo.tm_year,
                              (uint8_t) timeinfo.tm_mon,
                              (uint8_t) timeinfo.tm_mday,
                              (uint8_t) timeinfo.tm_hour,
                              (uint8_t) timeinfo.tm_min,
                              (uint8_t) timeinfo.tm_sec) ;
    }
    else
    {
        Serial.println("error");
    }
    
    
    if (!Rtc.IsDateTimeValid())
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }
    
    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
    
    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled)
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled)
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }
    Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
    
}
