#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WSout1 2  // Pin GPIO2
#define WSout2 2  
#define WSout3 2
#define WSout4 2
//#define WSout5 2

#define WSbit1 (1<<WSout1)
#define WSbit2 (1<<WSout2)
#define WSbit3 (1<<WSout3)
#define WSbit4 (1<<WSout4)
//#define WSbit5 (1<<WSout5)

// ARTNET CODES
#define ARTNET_DATA 0x50
#define ARTNET_POLL 0x20
#define ARTNET_POLL_REPLY 0x21
#define ARTNET_PORT 6454
#define ARTNET_HEADER 17

WiFiUDP udp;

//universe 0
uint8_t uniData1[512];
uint8_t universe1 = 0;
uint16_t uniSize1;

//universe 1
uint8_t uniData2[512];
uint8_t universe2 = 1;
uint16_t uniSize2;

//universe 3
uint8_t uniData3[512];
uint8_t universe3 = 2;
uint16_t uniSize3;

//universe 4
uint8_t uniData4[512];
uint8_t universe4 = 3;
uint16_t uniSize4;
#define  MAX_ARTNET_SIZE 600
uint8_t hData[MAX_ARTNET_SIZE];  //[ARTNET_HEADER + 1];
uint8_t net = 0;
uint8_t subnet = 3;


const char* ssid     = "";
const char* password = "";


IPAddress local_ip(192,168, 1, 115);
IPAddress gateway_ip(192,168, 1, 1);
IPAddress subnet_ip(255, 255, 255, 0);


void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(local_ip, gateway_ip, subnet_ip);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  udp.begin(ARTNET_PORT); // Open ArtNet port

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  pinMode(WSout1, OUTPUT);
  pinMode(WSout2, OUTPUT);
  pinMode(WSout3, OUTPUT);
  pinMode(WSout4, OUTPUT);
  //pinMode(WSout5, OUTPUT);
}


void sendWS1() {
  uint32_t writeBits;
  uint8_t  bitMask, time;
 os_intr_lock();
  for (uint16_t t = 0; t < uniSize1; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit1 );  // do ON bits // T=0
      }
      if ( uniData1[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit1;  // else it must be a zero, so write the off bit !
      }
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit1 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
  os_intr_unlock();
}


void sendWS2() {
  uint32_t writeBits;
  uint8_t  bitMask, time;
  os_intr_lock();
  for (uint16_t t = 0; t < uniSize2; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit2 );  // do ON bits // T=0
      }
      if ( uniData2[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit2;  // else it must be a zero, so write the off bit !
      }
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit2 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
  os_intr_unlock();
}


void sendWS3() {
  uint32_t writeBits;
  uint8_t  bitMask, time;
  os_intr_lock();
  for (uint16_t t = 0; t < uniSize3; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit3 );  // do ON bits // T=0
      }
      if ( uniData3[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit3;  // else it must be a zero, so write the off bit !
      }
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit3 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
  os_intr_unlock();
}


void sendWS4() {
  uint32_t writeBits;
  uint8_t  bitMask, time;
  os_intr_lock();
  for (uint16_t t = 0; t < uniSize4; t++) { // outer loop counting bytes
    bitMask = 0x80;
    while (bitMask) {
      // time=0ns : start by setting bit on
      time = 4;
      while (time--) {
        WRITE_PERI_REG( 0x60000304, WSbit4 );  // do ON bits // T=0
      }
      if ( uniData4[t] & bitMask ) {
        writeBits = 0;  // if this is a '1' keep the on time on for longer, so dont write an off bit
      }
      else {
        writeBits = WSbit4;  // else it must be a zero, so write the off bit !
      }
      time = 4;
      while (time--) {
          ( 0x60000308, writeBits );  // do OFF bits // T='0' time 350ns
      }
      time = 6;
      while (time--) {
        WRITE_PERI_REG( 0x60000308, WSbit4 );  // switch all bits off  T='1' time 700ns
      }
      // end of bite write time=1250ns
      bitMask >>= 1;
    }
  }
  os_intr_unlock();
}

int sync=0;
void loop() {
  if (udp.parsePacket()) {
    udp.read(hData, MAX_ARTNET_SIZE);
    if ( hData[0] == 'A' && hData[1] == 'r' && hData[2] == 't' && hData[3] == '-' && hData[4] == 'N' && hData[5] == 'e' && hData[6] == 't') {
      if ( hData[8] == 0x00 && hData[9] == ARTNET_DATA && hData[15] == net ) {

        if ( hData[14] == (subnet << 4) + universe1 ) { // UNIVERSE One
          //if (!uniSize1) { 
            uniSize1 = (hData[16] << 8) + (hData[17]);
          //}
         // udp.read(uniData1, uniSize1);
          memcpy(uniData1,hData + ARTNET_HEADER + 1,uniSize1);
          sync=1;
        }

        if ( hData[14] == (subnet << 4) + universe2 ) { // UNIVERSE two
         // if (!uniSize2) {
            uniSize2 = (hData[16] << 8) + (hData[17]);
          //}
          memcpy(uniData2,hData + ARTNET_HEADER + 1,uniSize2);
         // udp.read(uniData2, uniSize2);
          sync+=2;
        }

        if ( hData[14] == (subnet << 4) + universe3 ) { // UNIVERSE drei
          //if (!uniSize3) {
            uniSize3 = (hData[16] << 8) + (hData[17]);
          //}
          memcpy(uniData3,hData + ARTNET_HEADER + 1,uniSize3);
          //udp.read(uniData3, uniSize3);
          sync+=4;
        }
        if ( hData[14] == (subnet << 4) + universe4 ) { // UNIVERSE vier
         // if (!uniSize4) {
            uniSize4 = (hData[16] << 8) + (hData[17]);
          //}
          memcpy(uniData4,hData + ARTNET_HEADER + 1,uniSize4);
          sync+=8;
          //udp.read(uniData4, uniSize4);
          }
        //  os_intr_lock();
        if(sync==15)
        {
          sendWS1();
          sendWS2();
          sendWS3();
          sendWS4();
        }
         // os_intr_unlock();
         // sendWS5();
        

      } // if Artnet Data
    }
  }
}
