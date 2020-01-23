// FastLED_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

//#define P32BY8X4
#define DISABLE_WHITE
// Use serialized output instead of parallel output
//#define SERIAL_OUTPUT
#define P64BY64
//#define P32BY64

#if defined(P32BY64) || defined(P64BY64)
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_SHOW_CORE 0
#endif

#include <Adafruit_GFX.h>
#include <FastLED_NeoMatrix.h>
#include <FastLED.h>

// Choose your prefered pixmap
//#include "heart24.h"
//#include "yellowsmiley24.h"
//#include "bluesmiley24.h"
#include "smileytongue24.h"
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif


// Allow temporaly dithering, does not work with ESP32 right now
#ifndef ESP32
#define delay FastLED.delay
#else
FASTLED_USING_NAMESPACE
// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle = 0;

void FastLEDshowESP32()
{
    if (userTaskHandle == 0) {
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
	// -- Store the handle of the current task, so that the show task can
	//    notify it when it's done
	userTaskHandle = xTaskGetCurrentTaskHandle();

	// -- Trigger the show task
	xTaskNotifyGive(FastLEDshowTaskHandle);

	// -- Wait to be notified that it's done
	ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
	userTaskHandle = 0;
    }
}

void FastLEDshowTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
	// -- Wait for the trigger
	ulTaskNotifyTake(pdTRUE,portMAX_DELAY);



	// -- Do the show (synchronously)
	FastLED.show();

	// -- Notify the calling task
	xTaskNotifyGive(userTaskHandle);
    }
}
#endif

#if defined(ESP32) or defined(ESP8266)
#define PIN 5
#else
#define PIN 13
#endif


#define P16BY16X4
//#define P32BY8X3
#if defined(P32BY8X4) || defined(P16BY16X4) || defined(P32BY8X3)
#define BM32
#endif

#ifdef BM32
#include "google32.h"
// Anything with black does not look so good with the naked eye (better on pictures)
//#include "linux32.h"
#endif

// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 255

// https://learn.adafruit.com/adafruit-neopixel-uberguide/neomatrix-library
// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.

#ifdef P64BY64
#define NUM_STRIPS 16
#define NUM_LEDS_PER_STRIP 3*123
// Define full matrix width and height.
#define mw 123
#define mh 48
#define NUMMATRIX (mw*mh)
#define MATRIX_TOTAL NUMMATRIX 
#define NUM_LEDS NUMMATRIX 
#define MATRIX_HEIGHT mh
#define MATRIX_WIDTH mw
#define leds matrixleds
CRGB leds[NUMMATRIX];
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, mw, mh, 
  NEO_MATRIX_BOTTOM      + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);

#elif defined(P32BY64)
#define NUM_STRIPS 8
#define NUM_LEDS_PER_STRIP 256
// Define full matrix width and height.
#define mw 32
#define mh 64
#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, mw, mh, 
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);

#elif defined(P32BY8X4)
// Define full matrix width and height.
#define mw 32
#define mh 32
#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 8, 32, 4, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG + 
// progressive vs zigzag makes no difference for a 4 arrays next to one another
    NEO_TILE_TOP + NEO_TILE_LEFT +  NEO_TILE_PROGRESSIVE);

#elif defined(P16BY16X4)
#define mw 32
#define mh 32
#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 16, 16, 2, 2, 
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_RIGHT +  NEO_TILE_PROGRESSIVE);

#elif defined(P32BY8X3)
#define mw 24
#define mh 32
#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, 8, 32, 3, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG + 
    NEO_TILE_TOP + NEO_TILE_LEFT +  NEO_TILE_PROGRESSIVE);

#else
#define mw 16
#define mh 16
#define NUMMATRIX (mw*mh)
CRGB leds[NUMMATRIX];
// Define matrix width and height.
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, mw, mh, 
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG);
#endif


void replaced()
{
    int offset=0;
  for(int i=0;i<123;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s;
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 offset=24*123;
  for(int i=0;i<24*123;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s; 
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 //on met les boards en noir

 for (int i=0;i<48;i++)
 {
  leds[i*123]=CRGB::Black;
  leds[(i+1)*123-1]=CRGB::Black;
 }
}


void matrix_show() {
#ifdef SERIAL_OUTPUT
    matrix->show();
#else
    replaced();
    FastLEDshowESP32();
    replaced();
#endif
}


// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK		0

#define LED_RED_VERYLOW 	(3 <<  11)
#define LED_RED_LOW 		(7 <<  11)
#define LED_RED_MEDIUM 		(15 << 11)
#define LED_RED_HIGH 		(31 << 11)

#define LED_GREEN_VERYLOW	(1 <<  5)   
#define LED_GREEN_LOW 		(15 << 5)  
#define LED_GREEN_MEDIUM 	(31 << 5)  
#define LED_GREEN_HIGH 		(63 << 5)  

#define LED_BLUE_VERYLOW	3
#define LED_BLUE_LOW 		7
#define LED_BLUE_MEDIUM 	15
#define LED_BLUE_HIGH 		31

#define LED_ORANGE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW		(LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW	(LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW		(LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM	(LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH		(LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW	(LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW		(LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM		(LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH		(LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW	(LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW		(LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM	(LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH		(LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

static const uint8_t PROGMEM
    mono_bmp[][8] =
    {
	{   // 0: checkered 1
	    B10101010,
	    B01010101,
	    B10101010,
	    B01010101,
	    B10101010,
	    B01010101,
	    B10101010,
	    B01010101,
			},

	{   // 1: checkered 2
	    B01010101,
	    B10101010,
	    B01010101,
	    B10101010,
	    B01010101,
	    B10101010,
	    B01010101,
	    B10101010,
			},

	{   // 2: smiley
	    B00111100,
	    B01000010,
	    B10100101,
	    B10000001,
	    B10100101,
	    B10011001,
	    B01000010,
	    B00111100 },

	{   // 3: neutral
	    B00111100,
	    B01000010,
	    B10100101,
	    B10000001,
	    B10111101,
	    B10000001,
	    B01000010,
	    B00111100 },

	{   // 4; frowny
	    B00111100,
	    B01000010,
	    B10100101,
	    B10000001,
	    B10011001,
	    B10100101,
	    B01000010,
	    B00111100 },
    };

static const uint16_t PROGMEM
    // These bitmaps were written for a backend that only supported
    // 4 bits per color with Blue/Green/Red ordering while neomatrix
    // uses native 565 color mapping as RGB.  
    // I'm leaving the arrays as is because it's easier to read
    // which color is what when separated on a 4bit boundary
    // The demo code will modify the arrays at runtime to be compatible
    // with the neomatrix color ordering and bit depth.
    RGB_bmp[][64] = {
      // 00: blue, blue/red, red, red/green, green, green/blue, blue, white
      {	0x100, 0x200, 0x300, 0x400, 0x600, 0x800, 0xA00, 0xF00, 
	0x101, 0x202, 0x303, 0x404, 0x606, 0x808, 0xA0A, 0xF0F, 
      	0x001, 0x002, 0x003, 0x004, 0x006, 0x008, 0x00A, 0x00F, 
	0x011, 0x022, 0x033, 0x044, 0x066, 0x088, 0x0AA, 0x0FF, 
	0x010, 0x020, 0x030, 0x040, 0x060, 0x080, 0x0A0, 0x0F0, 
	0x110, 0x220, 0x330, 0x440, 0x660, 0x880, 0xAA0, 0xFF0, 
	0x100, 0x200, 0x300, 0x400, 0x600, 0x800, 0xA00, 0xF00, 
	0x111, 0x222, 0x333, 0x444, 0x666, 0x888, 0xAAA, 0xFFF, },

      // 01: grey to white
      {	0x111, 0x222, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF, 
	0x222, 0x222, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF, 
	0x333, 0x333, 0x333, 0x555, 0x777, 0x999, 0xAAA, 0xFFF, 
	0x555, 0x555, 0x555, 0x555, 0x777, 0x999, 0xAAA, 0xFFF, 
	0x777, 0x777, 0x777, 0x777, 0x777, 0x999, 0xAAA, 0xFFF, 
	0x999, 0x999, 0x999, 0x999, 0x999, 0x999, 0xAAA, 0xFFF, 
	0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xAAA, 0xFFF, 
	0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, 0xFFF, },

      // 02: low red to high red
      {	0x001, 0x002, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F, 
	0x002, 0x002, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F, 
	0x003, 0x003, 0x003, 0x005, 0x007, 0x009, 0x00A, 0x00F, 
	0x005, 0x005, 0x005, 0x005, 0x007, 0x009, 0x00A, 0x00F, 
	0x007, 0x007, 0x007, 0x007, 0x007, 0x009, 0x00A, 0x00F, 
	0x009, 0x009, 0x009, 0x009, 0x009, 0x009, 0x00A, 0x00F, 
	0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00A, 0x00F, 
	0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, 0x00F, },

      // 03: low green to high green
      {	0x010, 0x020, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0, 
	0x020, 0x020, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0, 
	0x030, 0x030, 0x030, 0x050, 0x070, 0x090, 0x0A0, 0x0F0, 
	0x050, 0x050, 0x050, 0x050, 0x070, 0x090, 0x0A0, 0x0F0, 
	0x070, 0x070, 0x070, 0x070, 0x070, 0x090, 0x0A0, 0x0F0, 
	0x090, 0x090, 0x090, 0x090, 0x090, 0x090, 0x0A0, 0x0F0, 
	0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0A0, 0x0F0, 
	0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, 0x0F0, },

      // 04: low blue to high blue
      {	0x100, 0x200, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00, 
	0x200, 0x200, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00, 
	0x300, 0x300, 0x300, 0x500, 0x700, 0x900, 0xA00, 0xF00, 
	0x500, 0x500, 0x500, 0x500, 0x700, 0x900, 0xA00, 0xF00, 
	0x700, 0x700, 0x700, 0x700, 0x700, 0x900, 0xA00, 0xF00, 
	0x900, 0x900, 0x900, 0x900, 0x900, 0x900, 0xA00, 0xF00, 
	0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xA00, 0xF00, 
	0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, 0xF00, },

      // 05: 1 black, 2R, 2O, 2G, 1B with 4 blue lines rising right
      {	0x000, 0x200, 0x000, 0x400, 0x000, 0x800, 0x000, 0xF00, 
      	0x000, 0x201, 0x002, 0x403, 0x004, 0x805, 0x006, 0xF07, 
	0x008, 0x209, 0x00A, 0x40B, 0x00C, 0x80D, 0x00E, 0xF0F, 
	0x000, 0x211, 0x022, 0x433, 0x044, 0x855, 0x066, 0xF77, 
	0x088, 0x299, 0x0AA, 0x4BB, 0x0CC, 0x8DD, 0x0EE, 0xFFF, 
	0x000, 0x210, 0x020, 0x430, 0x040, 0x850, 0x060, 0xF70, 
	0x080, 0x290, 0x0A0, 0x4B0, 0x0C0, 0x8D0, 0x0E0, 0xFF0,
	0x000, 0x200, 0x000, 0x500, 0x000, 0x800, 0x000, 0xF00, },

      // 06: 4 lines of increasing red and then green
      { 0x000, 0x000, 0x001, 0x001, 0x002, 0x002, 0x003, 0x003, 
	0x004, 0x004, 0x005, 0x005, 0x006, 0x006, 0x007, 0x007, 
	0x008, 0x008, 0x009, 0x009, 0x00A, 0x00A, 0x00B, 0x00B, 
	0x00C, 0x00C, 0x00D, 0x00D, 0x00E, 0x00E, 0x00F, 0x00F, 
	0x000, 0x000, 0x010, 0x010, 0x020, 0x020, 0x030, 0x030, 
	0x040, 0x040, 0x050, 0x050, 0x060, 0x060, 0x070, 0x070, 
	0x080, 0x080, 0x090, 0x090, 0x0A0, 0x0A0, 0x0B0, 0x0B0, 
	0x0C0, 0x0C0, 0x0D0, 0x0D0, 0x0E0, 0x0E0, 0x0F0, 0x0F0, },

      // 07: 4 lines of increasing red and then blue
      { 0x000, 0x000, 0x001, 0x001, 0x002, 0x002, 0x003, 0x003, 
	0x004, 0x004, 0x005, 0x005, 0x006, 0x006, 0x007, 0x007, 
	0x008, 0x008, 0x009, 0x009, 0x00A, 0x00A, 0x00B, 0x00B, 
	0x00C, 0x00C, 0x00D, 0x00D, 0x00E, 0x00E, 0x00F, 0x00F, 
	0x000, 0x000, 0x100, 0x100, 0x200, 0x200, 0x300, 0x300, 
	0x400, 0x400, 0x500, 0x500, 0x600, 0x600, 0x700, 0x700, 
	0x800, 0x800, 0x900, 0x900, 0xA00, 0xA00, 0xB00, 0xB00, 
	0xC00, 0xC00, 0xD00, 0xD00, 0xE00, 0xE00, 0xF00, 0xF00, },

      // 08: criss cross of green and red with diagonal blue.
      {	0xF00, 0x001, 0x003, 0x005, 0x007, 0x00A, 0x00F, 0x000, 
	0x020, 0xF21, 0x023, 0x025, 0x027, 0x02A, 0x02F, 0x020, 
	0x040, 0x041, 0xF43, 0x045, 0x047, 0x04A, 0x04F, 0x040, 
	0x060, 0x061, 0x063, 0xF65, 0x067, 0x06A, 0x06F, 0x060, 
	0x080, 0x081, 0x083, 0x085, 0xF87, 0x08A, 0x08F, 0x080, 
	0x0A0, 0x0A1, 0x0A3, 0x0A5, 0x0A7, 0xFAA, 0x0AF, 0x0A0, 
	0x0F0, 0x0F1, 0x0F3, 0x0F5, 0x0F7, 0x0FA, 0xFFF, 0x0F0, 
	0x000, 0x001, 0x003, 0x005, 0x007, 0x00A, 0x00F, 0xF00, },

      // 09: 2 lines of green, 2 red, 2 orange, 2 green
      { 0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, 
	0x0F0, 0x0F0, 0x0FF, 0x0FF, 0x00F, 0x00F, 0x0F0, 0x0F0, },

      // 10: multicolor smiley face
      { 0x000, 0x000, 0x00F, 0x00F, 0x00F, 0x00F, 0x000, 0x000, 
	0x000, 0x00F, 0x000, 0x000, 0x000, 0x000, 0x00F, 0x000, 
	0x00F, 0x000, 0xF00, 0x000, 0x000, 0xF00, 0x000, 0x00F, 
	0x00F, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x00F, 
	0x00F, 0x000, 0x0F0, 0x000, 0x000, 0x0F0, 0x000, 0x00F, 
	0x00F, 0x000, 0x000, 0x0F4, 0x0F3, 0x000, 0x000, 0x00F, 
	0x000, 0x00F, 0x000, 0x000, 0x000, 0x000, 0x00F, 0x000, 
	0x000, 0x000, 0x00F, 0x00F, 0x00F, 0x00F, 0x000, 0x000, },
};

int XY2( int x, int y, bool wrap=false) { 
    return matrix->XY(x,MATRIX_HEIGHT-1-y);
}


uint16_t XY( uint8_t x, uint8_t y) {
    return matrix->XY(x,y);
}
void matrix_clear() {
    // clear does not work properly with multiple matrices connected via parallel inputs
    memset(leds, 0, sizeof(leds));
}

// Convert a BGR 4/4/4 bitmap to RGB 5/6/5 used by Adafruit_GFX
void fixdrawRGBBitmap(int16_t x, int16_t y, const uint16_t *bitmap, int16_t w, int16_t h) {
    uint16_t RGB_bmp_fixed[w * h];
    for (uint16_t pixel=0; pixel<w*h; pixel++) {
	uint8_t r,g,b;
	uint16_t color = pgm_read_word(bitmap + pixel);

	//Serial.print(color, HEX);
	b = (color & 0xF00) >> 8;
	g = (color & 0x0F0) >> 4;
	r = color & 0x00F;
	//Serial.print(" ");
	//Serial.print(b);
	//Serial.print("/");
	//Serial.print(g);
	//Serial.print("/");
	//Serial.print(r);
	//Serial.print(" -> ");
	// expand from 4/4/4 bits per color to 5/6/5
	b = map(b, 0, 15, 0, 31);
	g = map(g, 0, 15, 0, 63);
	r = map(r, 0, 15, 0, 31);
	//Serial.print(r);
	//Serial.print("/");
	//Serial.print(g);
	//Serial.print("/");
	//Serial.print(b);
	RGB_bmp_fixed[pixel] = (r << 11) + (g << 5) + b;
	//Serial.print(" -> ");
	//Serial.println(RGB_bmp_fixed[pixel], HEX);
    }
    matrix->drawRGBBitmap(x, y, RGB_bmp_fixed, w, h);
}

// In a case of a tile of neomatrices, this test is helpful to make sure that the
// pixels are all in sequence (to check your wiring order and the tile options you
// gave to the constructor).
void count_pixels() {
    matrix_clear();
    for (uint16_t i=0; i<mh; i++) {
	for (uint16_t j=0; j<mw; j++) {
	    matrix->drawPixel(j, i, i%3==0?LED_BLUE_HIGH:i%3==1?LED_RED_HIGH:LED_GREEN_HIGH);
	    // depending on the matrix size, it's too slow to display each pixel, so
	    // make the scan init faster. This will however be too fast on a small matrix.
	    #ifdef ESP8266
	    if (!(j%3)) matrix_show();
	    yield(); // reset watchdog timer
	    #elif ESP32
	    delay(1);
	    matrix_show();
	    #else 
	    matrix_show();
	    #endif
	}
    }
}

// Fill the screen with multiple levels of white to gauge the quality
void display_four_white() {
    matrix_clear();
    matrix->fillRect(0,0, mw,mh, LED_WHITE_HIGH);
    matrix->drawRect(1,1, mw-2,mh-2, LED_WHITE_MEDIUM);
    matrix->drawRect(2,2, mw-4,mh-4, LED_WHITE_LOW);
    matrix->drawRect(3,3, mw-6,mh-6, LED_WHITE_VERYLOW);
    matrix_show();
}

void display_bitmap(uint8_t bmp_num, uint16_t color) { 
    static uint16_t bmx,bmy;

    // Clear the space under the bitmap that will be drawn as
    // drawing a single color pixmap does not write over pixels
    // that are nul, and leaves the data that was underneath
    matrix->fillRect(bmx,bmy, bmx+8,bmy+8, LED_BLACK);
    matrix->drawBitmap(bmx, bmy, mono_bmp[bmp_num], 8, 8, color);
    bmx += 8;
    if (bmx >= mw) bmx = 0;
    if (!bmx) bmy += 8;
    if (bmy >= mh) bmy = 0;
    matrix_show();
}

void display_rgbBitmap(uint8_t bmp_num) { 
    static uint16_t bmx,bmy;

    fixdrawRGBBitmap(bmx, bmy, RGB_bmp[bmp_num], 8, 8);
    bmx += 8;
    if (bmx >= mw) bmx = 0;
    if (!bmx) bmy += 8;
    if (bmy >= mh) bmy = 0;
    matrix_show();
}

void display_lines() {
    matrix_clear();

    // 4 levels of crossing red lines.
    matrix->drawLine(0,mh/2-2, mw-1,2, LED_RED_VERYLOW);
    matrix->drawLine(0,mh/2-1, mw-1,3, LED_RED_LOW);
    matrix->drawLine(0,mh/2,   mw-1,mh/2, LED_RED_MEDIUM);
    matrix->drawLine(0,mh/2+1, mw-1,mh/2+1, LED_RED_HIGH);

    // 4 levels of crossing green lines.
    matrix->drawLine(mw/2-2, 0, mw/2-2, mh-1, LED_GREEN_VERYLOW);
    matrix->drawLine(mw/2-1, 0, mw/2-1, mh-1, LED_GREEN_LOW);
    matrix->drawLine(mw/2+0, 0, mw/2+0, mh-1, LED_GREEN_MEDIUM);
    matrix->drawLine(mw/2+1, 0, mw/2+1, mh-1, LED_GREEN_HIGH);

    // Diagonal blue line.
    matrix->drawLine(0,0, mw-1,mh-1, LED_BLUE_HIGH);
    matrix->drawLine(0,mh-1, mw-1,0, LED_ORANGE_MEDIUM);
    matrix_show();
}

void display_boxes() {
    matrix_clear();
    matrix->drawRect(0,0, mw,mh, LED_BLUE_HIGH);
    matrix->drawRect(1,1, mw-2,mh-2, LED_GREEN_MEDIUM);
    matrix->fillRect(2,2, mw-4,mh-4, LED_RED_HIGH);
    matrix->fillRect(3,3, mw-6,mh-6, LED_ORANGE_MEDIUM);
    matrix_show();
}

void display_circles() {
    matrix_clear();
    matrix->drawCircle(mw/2,mh/2, 2, LED_RED_MEDIUM);
    matrix->drawCircle(mw/2-1-min(mw,mh)/8, mh/2-1-min(mw,mh)/8, min(mw,mh)/4, LED_BLUE_HIGH);
    matrix->drawCircle(mw/2+1+min(mw,mh)/8, mh/2+1+min(mw,mh)/8, min(mw,mh)/4-1, LED_ORANGE_MEDIUM);
    matrix->drawCircle(1,mh-2, 1, LED_GREEN_LOW);
    matrix->drawCircle(mw-2,1, 1, LED_GREEN_HIGH);
    if (min(mw,mh)>12) matrix->drawCircle(mw/2-1, mh/2-1, min(mh/2-1,mw/2-1), LED_CYAN_HIGH);
    matrix_show();
}

void display_resolution() {
    matrix->setTextSize(1);
    // not wide enough;
    if (mw<16) return;
    matrix_clear();
    // Font is 5x7, if display is too small
    // 8 can only display 1 char
    // 16 can almost display 3 chars
    // 24 can display 4 chars
    // 32 can display 5 chars
    matrix->setCursor(0, 0);
    matrix->setTextColor(matrix->Color(255,0,0));
    if (mw>10) matrix->print(mw/10);
    matrix->setTextColor(matrix->Color(255,128,0)); 
    matrix->print(mw % 10);
    matrix->setTextColor(matrix->Color(0,255,0));
    matrix->print('x');
    // not wide enough to print 5 chars, go to next line
    if (mw<25) {
	if (mh==13) matrix->setCursor(6, 7);
	else if (mh>=13) {
	    matrix->setCursor(mw-11, 8);
	} else {
	    // we're not tall enough either, so we wait and display
	    // the 2nd value on top.
	    matrix_show();
	    delay(2000);
	    matrix_clear();
	    matrix->setCursor(mw-11, 0);
	}   
    }
    matrix->setTextColor(matrix->Color(0,255,128)); 
    matrix->print(mh/10);
    matrix->setTextColor(matrix->Color(0,128,255));  
    matrix->print(mh % 10);
    // enough room for a 2nd line
    if ((mw>25 && mh >14) || mh>16) {
	matrix->setCursor(0, mh-7);
	matrix->setTextColor(matrix->Color(0,255,255)); 
	if (mw>16) matrix->print('*');
	matrix->setTextColor(matrix->Color(255,0,0)); 
	matrix->print('R');
	matrix->setTextColor(matrix->Color(0,255,0));
	matrix->print('G');
	matrix->setTextColor(matrix->Color(0,0,255)); 
	matrix->print("B");
	matrix->setTextColor(matrix->Color(255,255,0)); 
	// this one could be displayed off screen, but we don't care :)
	matrix->print("*");

	// We have a big array, great, let's assume 32x32 and add something in the middle
	if (mh>24 && mw>25) {
	    for (uint16_t i=0; i<mw; i+=8) fixdrawRGBBitmap(i, mh/2-7+(i%16)/8*6, RGB_bmp[10], 8, 8);
	}
    }
    
    matrix_show();
}

void display_scrollText() {
    uint8_t size = max(int(mw/8), 1);
    matrix_clear();
    matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
    matrix->setTextSize(1);
    matrix->setRotation(0);
    for (int8_t x=7; x>=-42; x--) {
	yield();
	matrix_clear();
	matrix->setCursor(x,0);
	matrix->setTextColor(LED_GREEN_HIGH);
	matrix->print("Hello");
	if (mh>11) {
	    matrix->setCursor(-20-x,mh-7);
	    matrix->setTextColor(LED_ORANGE_HIGH);
	    matrix->print("World");
	}
	matrix_show();
       delay(50);
    }

    matrix->setRotation(3);
    matrix->setTextSize(size);
    matrix->setTextColor(LED_BLUE_HIGH);
    for (int16_t x=8*size; x>=-6*8*size; x--) {
	yield();
	matrix_clear();
	matrix->setCursor(x,mw/2-size*4);
	matrix->print("Rotate");
	matrix_show();
	// note that on a big array the refresh rate from show() will be slow enough that
	// the delay become irrelevant. This is already true on a 32x32 array.
        delay(50/size);
    }
    matrix->setRotation(0);
    matrix->setCursor(0,0);
    matrix_show();
}

// Scroll within big bitmap so that all if it becomes visible or bounce a small one.
// If the bitmap is bigger in one dimension and smaller in the other one, it will
// be both panned and bounced in the appropriate dimensions.
void display_panOrBounceBitmap (uint8_t bitmapSize) {
    // keep integer math, deal with values 16 times too big
    // start by showing upper left of big bitmap or centering if the display is big
    int16_t xf = max(0, (mw-bitmapSize)/2) << 4;
    int16_t yf = max(0, (mh-bitmapSize)/2) << 4;
    // scroll speed in 1/16th
    int16_t xfc = 6;
    int16_t yfc = 3;
    // scroll down and right by moving upper left corner off screen 
    // more up and left (which means negative numbers)
    int16_t xfdir = -1;
    int16_t yfdir = -1;

    for (uint16_t i=1; i<200; i++) {
	bool updDir = false;

	// Get actual x/y by dividing by 16.
	int16_t x = xf >> 4;
	int16_t y = yf >> 4;

	matrix_clear();
	// bounce 8x8 tri color smiley face around the screen
	if (bitmapSize == 8) fixdrawRGBBitmap(x, y, RGB_bmp[10], 8, 8);
	// pan 24x24 pixmap
	if (bitmapSize == 24) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap24, bitmapSize, bitmapSize);
#ifdef BM32
	if (bitmapSize == 32) matrix->drawRGBBitmap(x, y, (const uint16_t *) bitmap32, bitmapSize, bitmapSize);
#endif
	matrix_show();
	 
	// Only pan if the display size is smaller than the pixmap
	// but not if the difference is too small or it'll look bad.
	if (bitmapSize-mw>2) {
	    xf += xfc*xfdir;
	    if (xf >= 0)                      { xfdir = -1; updDir = true ; };
	    // we don't go negative past right corner, go back positive
	    if (xf <= ((mw-bitmapSize) << 4)) { xfdir = 1;  updDir = true ; };
	}
	if (bitmapSize-mh>2) {
	    yf += yfc*yfdir;
	    // we shouldn't display past left corner, reverse direction.
	    if (yf >= 0)                      { yfdir = -1; updDir = true ; };
	    if (yf <= ((mh-bitmapSize) << 4)) { yfdir = 1;  updDir = true ; };
	}
	// only bounce a pixmap if it's smaller than the display size
	if (mw>bitmapSize) {
	    xf += xfc*xfdir;
	    // Deal with bouncing off the 'walls'
	    if (xf >= (mw-bitmapSize) << 4) { xfdir = -1; updDir = true ; };
	    if (xf <= 0)           { xfdir =  1; updDir = true ; };
	}
	if (mh>bitmapSize) {
	    yf += yfc*yfdir;
	    if (yf >= (mh-bitmapSize) << 4) { yfdir = -1; updDir = true ; };
	    if (yf <= 0)           { yfdir =  1; updDir = true ; };
	}
	
	if (updDir) {
	    // Add -1, 0 or 1 but bind result to 1 to 1.
	    // Let's take 3 is a minimum speed, otherwise it's too slow.
	    xfc = constrain(xfc + random(-1, 2), 3, 16);
	    yfc = constrain(xfc + random(-1, 2), 3, 16);
	}
	delay(10);
    }
}


CRGB solidColor = CRGB::Blue;
CRGB solidRainColor = CRGB(60,80,90);
uint16_t tempMatrix[MATRIX_WIDTH+1][MATRIX_HEIGHT+1];

int wrapX(int x) { 
  if (x < 0 ) return 0;
  if (x >= MATRIX_WIDTH) return (MATRIX_WIDTH-1);
  return x;
}
int gHue=0;

void stormyRain()
{
  // ( Depth of dots, maximum brightness, frequency of new dots, length of tails, color, splashes, clouds, ligthening )
  rain(0, 90, map8(255,0,150)+60, 10, solidRainColor, true, true, true);
}
int lightning[MATRIX_WIDTH][MATRIX_HEIGHT];

void rain(byte backgroundDepth, byte maxBrightness, byte spawnFreq, byte tailLength, CRGB rainColor, bool splashes, bool clouds, bool storm)
{
  static uint16_t noiseX = random16();
  static uint16_t noiseY = random16();
  static uint16_t noiseZ = random16();

  FastLED.delay(1000/50);
  // Add entropy to random number generator; we use a lot of it.

  CRGB lightningColor = CRGB(72,72,80);
  CRGBPalette16 rain_p( CRGB::Black, rainColor );
#ifdef SMARTMATRIX
  CRGBPalette16 rainClouds_p( CRGB::Black, CRGB(35,44,44), CRGB(29,35,35), CRGB::Black );
#else
  CRGBPalette16 rainClouds_p( CRGB::Black, CRGB(15,24,24), CRGB(9,15,15), CRGB::Black );
#endif

  fadeToBlackBy( leds, NUM_LEDS, 255-tailLength);

  // Loop for each column individually
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    // Step 1.  Move each dot down one cell
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
      if (tempMatrix[x][i] >= backgroundDepth) {  // Don't move empty cells
        tempMatrix[x][i-1] = tempMatrix[x][i];
        tempMatrix[x][i] = 0;
      }
    }

    // Step 2.  Randomly spawn new dots at top
    if (random(255) < spawnFreq) {
      tempMatrix[x][MATRIX_HEIGHT-1] = random(backgroundDepth, maxBrightness);
    }

    // Step 3. Map from tempMatrix cells to LED colors
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      if (tempMatrix[x][y] >= backgroundDepth) {  // Don't write out empty cells
        leds[XY2(x,y)] = ColorFromPalette(rain_p, tempMatrix[x][y]);
      }
    }

    // Step 4. Add splash if called for
    if (splashes) {
      static uint_fast16_t splashArray[MATRIX_WIDTH];

      byte j = splashArray[x];
      byte v = tempMatrix[x][0];

      if (j >= backgroundDepth) {
        leds[XY2(x-2,0,true)] = ColorFromPalette(rain_p, j/3);
        leds[XY2(x+2,0,true)] = ColorFromPalette(rain_p, j/3);
        splashArray[x] = 0;   // Reset splash
      }

      if (v >= backgroundDepth) {
        leds[XY2(x-1,1,true)] = ColorFromPalette(rain_p, v/2);
        leds[XY2(x+1,1,true)] = ColorFromPalette(rain_p, v/2);
        splashArray[x] = v; // Prep splash for next frame
      }
    }

    // Step 5. Add lightning if called for
    if (storm) {
      
      if (random16() < 72) {    // Odds of a lightning bolt
      #ifdef ESP32a
      Serial.println("The lightening code hangs on ESP32, no idea why");
      // I verified that it works on ESP8266, teensy 3.x, but it hangs on ESP32 and I can't
      // find why...
      #else
        lightning[scale8(random8(), MATRIX_WIDTH-1)][MATRIX_HEIGHT-1] = 255;  // Random starting location
        for(int ly = MATRIX_HEIGHT-1; ly > 1; ly--) {
          for (int lx = 1; lx < MATRIX_WIDTH-1; lx++) {
            if (lightning[lx][ly] == 255) {
              lightning[lx][ly] = 0;
              // storm crashes here on ESP32. no idea why.
              uint8_t dir = random8(4);
              switch (dir) {
                case 0:
                  // storm also crashes here, again no idea why
                  leds[XY2(lx+1,ly-1,true)] = lightningColor;
                  // but not here
                  lightning[wrapX(lx+1)][ly-1] = 255; // move down and right
                break;
                case 1:
                  leds[XY2(lx,ly-1,true)] = CRGB(128,128,128);
                  lightning[lx][ly-1] = 255;    // move down
                break;
                case 2:
                  leds[XY2(lx-1,ly-1,true)] = CRGB(128,128,128);
                  lightning[wrapX(lx-1)][ly-1] = 255; // move down and left
                break;
                case 3:
                  leds[XY2(lx-1,ly-1,true)] = CRGB(128,128,128);
                  lightning[wrapX(lx-1)][ly-1] = 255; // fork down and left
                  leds[XY2(lx-1,ly-1,true)] = CRGB(128,128,128);
                  lightning[wrapX(lx+1)][ly-1] = 255; // fork down and right
                break;
              }
            }
          }
        }
      #endif
      }
    }

    // Step 6. Add clouds if called for
    if (clouds) {
      uint16_t noiseScale = 250;  // A value of 1 will be so zoomed in, you'll mostly see solid colors. A value of 4011 will be very zoomed out and shimmery
      const uint8_t cloudHeight = (MATRIX_HEIGHT*0.2)+1;
      // This is the array that we keep our computed noise values in
      static uint8_t noise[MATRIX_WIDTH][cloudHeight];
      int xoffset = noiseScale * x + gHue;

      for(int z = 0; z < cloudHeight; z++) {
        int yoffset = noiseScale * z - gHue;
        uint8_t dataSmoothing = 192;
        uint8_t noiseData = qsub8(inoise8(noiseX + xoffset,noiseY + yoffset,noiseZ),16);
        noiseData = qadd8(noiseData,scale8(noiseData,39));
        noise[x][z] = scale8( noise[x][z], dataSmoothing) + scale8( noiseData, 256 - dataSmoothing);
        nblend(leds[XY2(x,MATRIX_HEIGHT-z-1)], ColorFromPalette(rainClouds_p, noise[x][z]), (cloudHeight-z)*(250/cloudHeight));
      }
      noiseZ ++;
    }
  }
}


void loop() {
    // clear the screen after X bitmaps have been displayed and we
    // loop back to the top left corner
    // 8x8 => 1, 16x8 => 2, 17x9 => 6
    static uint8_t pixmap_count = ((mw+7)/8) * ((mh+7)/8);
/*
// You can't use millis to time frame fresh rate because it uses cli() which breaks millis()
// So I use my stopwatch to count 200 displays and that's good enough
#if 0
    // 200 displays in 13 seconds = 15 frames per second for 4096 pixels
    for (uint8_t i=0; i<100; i++) { 
	matrix->fillScreen(LED_BLUE_LOW);
	matrix_show();
	matrix->fillScreen(LED_RED_LOW);
	matrix_show();
    }
#endif

    Serial.println("Count pixels");
    count_pixels();
    Serial.println("Count pixels done");
//    delay(3000);
//    matrix_clear();
//return;

    display_four_white();
    delay(3000);

    Serial.print("Screen pixmap capacity: ");
    Serial.println(pixmap_count);

    // multicolor bitmap sent as many times as we can display an 8x8 pixmap
    for (uint8_t i=0; i<=pixmap_count; i++)
    {
	display_rgbBitmap(0);
    }
    delay(1000);

    Serial.println("Display Resolution");
    display_resolution();
    delay(3000);

    Serial.println("Display bitmaps");
    // Cycle through red, green, blue, display 2 checkered patterns
    // useful to debug some screen types and alignment.
    uint16_t bmpcolor[] = { LED_GREEN_HIGH, LED_BLUE_HIGH, LED_RED_HIGH };
    for (uint8_t i=0; i<3; i++)
    {
	display_bitmap(0, bmpcolor[i]);
 	delay(500);
	display_bitmap(1, bmpcolor[i]);
 	delay(500);
    }

    Serial.println("Display smileys");
    // Display 3 smiley faces.
    for (uint8_t i=2; i<=4; i++)
    {
	display_bitmap(i, bmpcolor[i-2]);
	// If more than one pixmap displayed per screen, display more quickly.
	delay(mw>8?500:1500);
    }
    // If we have multiple pixmaps displayed at once, wait a bit longer on the last.
    delay(mw>8?1000:500);

    Serial.println("Display lines, boxes and circles");
    display_lines();
    delay(3000);

    display_boxes();
    delay(3000);

    display_circles();
    delay(3000);
    matrix_clear();

    Serial.println("Display RGB bitmaps");
    for (uint8_t i=0; i<=(sizeof(RGB_bmp)/sizeof(RGB_bmp[0])-1); i++)
    {
	display_rgbBitmap(i);
	delay(mw>8?500:1500);
    }
    // If we have multiple pixmaps displayed at once, wait a bit longer on the last.
    delay(mw>8?1000:500);

    Serial.println("Scrolltext");
    display_scrollText();

#ifdef BM32
    Serial.println("bounce 32 bitmap");
    display_panOrBounceBitmap(32);
#endif
    // pan a big pixmap
    Serial.println("pan/bounce 24 bitmap");
    display_panOrBounceBitmap(24);
    // bounce around a small one
    Serial.println("pan/bounce 8 bitmap");
    display_panOrBounceBitmap(8);

    Serial.println("Demo loop done, starting over");
    */
    stormyRain();
      
    //gHue=(gHue+1)%256;  // slowly cycle the "base color" through the rainbow
 
matrix->show();
}

void setup() {
#if defined(P32BY64) || defined(P64BY64)
    #ifdef SERIAL_OUTPUT
	// https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
	FastLED.addLeds<WS2812B, 0, GRB>(leds, 0*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B, 2, GRB>(leds, 1*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B, 4, GRB>(leds, 2*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B, 5, GRB>(leds, 3*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B,12, GRB>(leds, 4*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B,13, GRB>(leds, 5*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B,14, GRB>(leds, 6*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2812B,15, GRB>(leds, 7*NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP); 
    #else
	xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
	// hardcoded out put for pins 0,2,3 (RX),4,5,12-19,21,22,23
	// 16 17 18 19 21 22 23 24
	// https://raw.githubusercontent.com/hpwit/fastled-esp32-16PINS/master/Perf.png
	// https://raw.githubusercontent.com/hpwit/fastled-esp32-16PINS/master/README.md
	// https://github.com/hpwit/fastled-esp32-16PINS
	//FastLED.addLeds<WS2812B_PORTA,NUM//_STRIPS,0b11011>(leds, NUM_LEDS_PER_STRIP); 
	FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,0b110111011111111000000110100>(leds, NUM_LEDS_PER_STRIP); 
	//FastLED.addLeds<WS2812B_PORTA,NUM_STRIPS,((1<<0) + (1<<2) + (1<<4) + (1<<5) + (1<<12) + (1<<13) + (1<<14) + (1<<15))>(leds, NUM_LEDS_PER_STRIP); 
	//FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,0>(leds, NUM_LEDS_PER_STRIP); 
    #endif
#elif defined(P32BY8X3)
    // Parallel output
    FastLED.addLeds<WS2811_PORTA,3>(leds, NUMMATRIX/3).setCorrection(TypicalLEDStrip);
#else
    FastLED.addLeds<NEOPIXEL,PIN>(  leds, NUMMATRIX  ).setCorrection(TypicalLEDStrip);
#endif
    
    // Time for serial port to work?
    delay(1000);
    Serial.begin(115200);
    Serial.print("Init on pin: ");
    Serial.println(PIN);
    Serial.print("Matrix Size: ");
    Serial.print(mw);
    Serial.print(" ");
    Serial.print(mh);
    Serial.print(" ");
    Serial.println(NUMMATRIX);
    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(64);
    Serial.println("If the code crashes here, decrease the brightness or turn off the all white display below");
    // Test full bright of all LEDs. If brightness is too high
    // for your current limit (i.e. USB), decrease it.
#ifndef DISABLE_WHITE
    matrix->fillScreen(LED_WHITE_HIGH);
    matrix_show();
    delay(5000);
    matrix_clear();
#endif
}

// vim:sts=4:sw=4

/*
 platforms/esp/32/clockless_esp32.h
* ESP32 support is provided using the RMT peripheral device -- a unit
* on the chip designed specifically for generating (and receiving)
* precisely-timed digital signals. Nominally for use in infrared
* remote controls, we use it to generate the signals for clockless
* LED strips. The main advantage of using the RMT device is that,
* once programmed, it generates the signal asynchronously, allowing
* the CPU to continue executing other code. It is also not vulnerable
* to interrupts or other timing problems that could disrupt the signal.
*
* The implementation strategy is borrowed from previous work and from
* the RMT support built into the ESP32 IDF. The RMT device has 8
* channels, which can be programmed independently to send sequences
* of high/low bits. Memory for each channel is limited, however, so
* in order to send a long sequence of bits, we need to continuously
* refill the buffer until all the data is sent. To do this, we fill
* half the buffer and then set an interrupt to go off when that half
* is sent. Then we refill that half while the second half is being
* sent. This strategy effectively overlaps computation (by the CPU)
* and communication (by the RMT).
*
* Since the RMT device only has 8 channels, we need a strategy to
* allow more than 8 LED controllers. Our driver assigns controllers
* to channels on the fly, queuing up controllers as necessary until a
* channel is free. The main showPixels routine just fires off the
* first 8 controllers; the interrupt handler starts new controllers
* asynchronously as previous ones finish. So, for example, it can
* send the data for 8 controllers simultaneously, but 16 controllers
* would take approximately twice as much time.
*
* There is a #define that allows a program to control the total
* number of channels that the driver is allowed to use. It defaults
* to 8 -- use all the channels. Setting it to 1, for example, results
* in fully serial output:
*
*     #define FASTLED_RMT_MAX_CHANNELS 1
*
* OTHER RMT APPLICATIONS
*
* The default FastLED driver takes over control of the RMT interrupt
* handler, making it hard to use the RMT device for other
* (non-FastLED) purposes. You can change it's behavior to use the ESP
* core driver instead, allowing other RMT applications to
* co-exist. To switch to this mode, add the following directive
* before you include FastLED.h:
*
*      #define FASTLED_RMT_BUILTIN_DRIVER
*
* There may be a performance penalty for using this mode. We need to
* compute the RMT signal for the entire LED strip ahead of time,
* rather than overlapping it with communication. We also need a large
* buffer to hold the signal specification. Each bit of pixel data is
* represented by a 32-bit pulse specification, so it is a 32X blow-up
* in memory use.
*
*
* Based on public domain code created 19 Nov 2016 by Chris Osborn <fozztexx@fozztexx.com>
* http://insentricity.com *
*
*/
