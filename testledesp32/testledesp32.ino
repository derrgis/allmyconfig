#include "soc/efuse_reg.h"
#include "esp_system.h"
#include "soc/rtc.h"
#include "rom/uart.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
int k=1;
#define NUM_LEDS 600
CRGB leds[NUM_LEDS];

int getChipRevision()
{
  return (REG_READ(EFUSE_BLK0_RDATA3_REG) >> (EFUSE_RD_CHIP_VER_RESERVE_S)&&EFUSE_RD_CHIP_VER_RESERVE_V) ;
}

void setup() {
#define DATA_PIN      3    // for Huzzah: Pins w/o special function:  #4, #5, #12, #13, #14; // #16 does not work :(
#define DATA_PIN2     4
#define LED_TYPE      WS2811
#define COLOR_ORDER   GRB


CRGB Color = CRGB :: Blue;
 FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, 0,NUM_LEDS/2);     
 //
 FastLED.addLeds<LED_TYPE, DATA_PIN2, COLOR_ORDER>(leds, NUM_LEDS/2,NUM_LEDS/2);// for WS2812 (Neopixel)
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS); // for APA102 (Dotstar)
  //FastLED.setCorrection(TypicalLEDStrip);
 // FastLED.setBrightness(32);
  //FastLED.setMaxPowerInVoltsAndMilliamps(5, MILLI_AMPS);
 // fill_solid(leds, NUM_LEDS, Color
 // FastLED.show();
//LEDS.addLeds<WS2811_PORTA,2>(leds, 600);
  
  Serial.begin(115200);
  delay(200);
  Serial.print("REG_READ(EFUSE_BLK0_RDATA3_REG) ");
  Serial.println(REG_READ(EFUSE_BLK0_RDATA3_REG), BIN);

  Serial.print("EFUSE_RD_CHIP_VER_RESERVE_S ");
  Serial.println(EFUSE_RD_CHIP_VER_RESERVE_S, BIN);

  Serial.print("EFUSE_RD_CHIP_VER_RESERVE_V ");
  Serial.println(EFUSE_RD_CHIP_VER_RESERVE_V, BIN);

   Serial.println();

  Serial.print("Chip Revision (official version): ");
  Serial.println(getChipRevision());
  
  Serial.print("Chip Revision from shift Opration ");
  Serial.println(REG_READ(EFUSE_BLK0_RDATA3_REG) >> 15, BIN);
  uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  Serial.println(esp_get_free_heap_size());
  Serial.println(ESP.getFlashChipSpeed());
  Serial.println(rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get()));
   // rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
  Serial.println(ESP.getFlashChipSize());
}

// the loop function runs over and over again forever
// chipid;  




void loop() {

leds[k-1]=CRGB::Black;
leds[k]=CRGB::Blue;
long time1=millis();
FastLED.show();
long time2=millis()-time1;
Serial.println((float)1000/time2);
Serial.println(time2);
k=(k+1)%NUM_LEDS;
	// FastLED.delay((int)(1000 /1000 )+ 1);

}

