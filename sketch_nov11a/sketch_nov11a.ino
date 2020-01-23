#include <WiFi.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

//WiFi Settings
const char *ssid = "xxxxxxxxxx";
const char *password = "xxxxxxxxxx";

// LED Settings
const int numStrips = 6;
const int numLedsPerStrip = 165;
const int numLeds = numLedsPerStrip * numStrips; // Change if your setup has more or less LED's
const int numberOfChannels = numLeds * 3; // Total number of DMX channels you want to receive (1 led = 3 channels)
const int triOne = numLedsPerStrip;
const int triTwo = 2 * numLedsPerStrip;
const int triThree = 3 * numLedsPerStrip;
#define DATA_PIN_1 12
#define DATA_PIN_2 14
#define DATA_PIN_3 27
#define DATA_PIN_4 26
#define DATA_PIN_5 25
#define DATA_PIN_6 33
CRGB leds[numLedsPerStrip*numStrips];
CRGB * leds_1;
CRGB * leds_2;
CRGB * leds_3;
CRGB * leds_4;
CRGB * leds_5;
CRGB * leds_6;

// ArtNet Settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0); //personnaly i will put 510 instead of 512 beacuse per universes you can only have 170 Pixels max 
bool universesReceived[maxUniverses];
uint32_t syncmax1=0;
uint32_t syncmax2=0;
uint32_t sync=0;
uint32_t sync2=0;

bool sendFrame = 1;
int previousDataLength = 0;

// Initialize the WiFi client - if needed to manage the WiFi connection
WiFiClient client;

// Connect to WiFi – returns true if successful or false if not
boolean ConnectWifi(void) {
  boolean state = true;
  int i = 0;

  // Set the static IP address for the device
  //  IPAddress ip(192, 168, 1, 210);
  //  WiFi.config(ip);

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

// Callback, on DMX packet received
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t *data) {
  
  sendFrame = 1;
  
  // set brightness of the whole strip
  if (universe == 15) {
    FastLED.setBrightness(data[0]);
    FastLED.show();
  }
/*
  // Store which universe has got in
  if ((universe - startUniverse) < maxUniverses)
    universesReceived[universe - startUniverse] = 1;

  for (int i = 0 ; i < maxUniverses ; i++) {
    if (universesReceived[i] == 0) {
      //Serial.println("Broke");
      sendFrame = 0;
      break;
    }
  }
    */

    if ((universe - startUniverse)==0)
            {
              sync=1;
              sync2=0;
            }
             else
             {
               if((universe - startUniverse)<32)
                 sync=sync  | (1<<(universe - startUniverse));
               else
               sync2=sync2  | (1<<((universe - startUniverse)-32));
             
             }
  // read universe and put into the right part of the display buffer
  /*
  for (int i = 0; i < length / 3; i++) {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < numLeds) {
      if (led < numLedsPerStrip) { // 165 leds
        leds_1[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } else if (led < numLedsPerStrip * 2) { // 330 leds
        leds_2[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } else if (led < numLedsPerStrip * 3) { // 495 leds
        leds_3[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } else if (led < numLedsPerStrip * 4) { // 660 leds
        leds_4[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } else if (led < numLedsPerStrip * 5) { // 825 leds
        leds_5[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } else if (led < numLedsPerStrip * 6) { // 990 leds
        leds_6[i] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
      } 
    }
  }*/

// choose on of the two implementations.

//first code
int offset=(universe - startUniverse) * previousDataLength;

memcpy(&leds[offset],data,length);

  
  previousDataLength = length/3;

//end of first code

//second code
memcpy(&leds[previousDataLength],data,length);`
`
 previousDataLength += length/3;
//end of the second code


  

  if ((universe - startUniverse) == maxUniverses) {
    Serial.println("FLUSH PACKETS");
    client.flush();
  }
  
  if (sync==syncmax1 && sync2==syncmax2) {
    FastLED.show();

    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
    previousDataLength=0;
  }
}

void setup() {
  Serial.begin(115200);
  ConnectWifi();
  artnet.begin();
leds_1=leds;
leds_2=&leds[numLedsPerStrip];
leds_3=&leds[2*numLedsPerStrip];
leds_4=&leds[3*numLedsPerStrip];
leds_5=&leds[4*numLedsPerStrip];
leds_6=&leds[5*numLedsPerStrip];

if(maxUniverses<=32)
    {
        if(maxUniverses<32)
            syncmax1=(1<<maxUniverses)-1;
        else
            syncmax1=0xFFFFFFFF;
        syncmax2=0;
    }
    else
    {
            syncmax1=0xFFFFFFFF;
        if(maxUniverses-32<32)
            syncmax2=(1<<(maxUniverses-32))-1;
        else
            syncmax2=0xFFFFFFFF;
            //syncmax2=0;

    }
    
  FastLED.addLeds<NEOPIXEL, DATA_PIN_1>(leds_1, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_2>(leds_2, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_3>(leds_3, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_4>(leds_4, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_5>(leds_5, numLedsPerStrip);
  FastLED.addLeds<NEOPIXEL, DATA_PIN_6>(leds_6, numLedsPerStrip);

  // onDmxFrame will execute every time a packet is received by the ESP32
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop() {
  // we call the read function inside the loop
  artnet.read();
}
