

/*
#include <SDL/SDL.h>
#include <sys/time.h>
static SDL_Surface *screen;
static unsigned int frames;
static struct timeval tv1, tv2;



static int button_start, button_select, button_a, button_b, button_down, button_up, button_left, button_right;

void sdl_init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
}

int sdl_update(void)
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
			return 1;

		if(e.type == SDL_KEYDOWN)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_a:
					button_a = 1;
				break;
				case SDLK_s:
					button_b = 1;
				break;
				case SDLK_d:
					button_select = 1;
				break;
				case SDLK_f:
					button_start = 1;
				break;
				case SDLK_LEFT:
					button_left = 1;
				break;
				case SDLK_RIGHT:
					button_right = 1;
				break;
				case SDLK_DOWN:
					button_down = 1;
				break;
				case SDLK_UP:
					button_up = 1;
				break;
			}
		}

		if(e.type == SDL_KEYUP)
		{
			switch(e.key.keysym.sym)
			{
				case SDLK_a:
					button_a = 0;
				break;
				case SDLK_s:
					button_b = 0;
				break;
				case SDLK_d:
					button_select = 0;
				break;
				case SDLK_f:
					button_start = 0;
				break;
				case SDLK_LEFT:
					button_left = 0;
				break;
				case SDLK_RIGHT:
					button_right = 0;
				break;
				case SDLK_DOWN:
					button_down = 0;
				break;
				case SDLK_UP:
					button_up = 0;
				break;
			}
		}

	}
	return 0;
}

unsigned int sdl_get_buttons(void)
{
	return (button_start*8) | (button_select*4) | (button_b*2) | button_a;
}

unsigned int sdl_get_directions(void)
{
	return (button_down*8) | (button_up*4) | (button_left*2) | button_right;
}

unsigned int *sdl_get_framebuffer(void)
{
	return screen->pixels;
}

void sdl_frame(void)
{
	if(frames == 0)
  		gettimeofday(&tv1, NULL);
	
	frames++;
	if(frames % 1000 == 0)
	{
		gettimeofday(&tv2, NULL);
		printf("Frames %d, seconds: %d, fps: %d\n", frames, tv2.tv_sec - tv1.tv_sec, frames/(tv2.tv_sec - tv1.tv_sec));
	}
	SDL_Flip(screen);
}

void sdl_quit()
{
	SDL_Quit();
}
*/
//#include "SPI.h"
//#include "Adafruit_GFX.h"
//#include "Adafruit_ILI9341.h"

#define _cs   22   // 3 goes to TFT CS
#define _dc   21   // 4 goes to TFT DC
#define _mosi 23  // 5 goes to TFT MOSI
#define _sclk 19  // 6 goes to TFT SCK/CLK
#define _rst  18  // ESP RST to TFT RESET
#define _miso 25    // Not connected
#define _led   5
//       3.3V     // Goes to TFT LED  
//       5v       // Goes to TFT Vcc
//       Gnd      // Goes to TFT Gnd        

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define LED_PIN1    2
#define LED_PIN2    4
#define LED_PIN3    5
#define LED_PIN4    12

#define LED_PIN5    13
#define LED_PIN6    14
#define LED_PIN7    15
#define LED_PIN8    16
//--------//--------
#define LED_PIN9     17
#define LED_PIN10    18
#define LED_PIN11    19
#define LED_PIN12    21

#define LED_PIN13    22
#define LED_PIN14    23 //25
#define LED_PIN15    25 //33
#define LED_PIN16    26 //32
#define NUM_LEDS_PER_STRIP 123*3
#define OFFSET_LEDS 123*3
#define NUM_STRIPS 16
#define FASTLED_SHOW_CORE 0
#define FASTLED_ESP32_I2S 1
#include "FastLED.h"


static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;
CRGB leds[123*48];
CRGB leds2[123*48];
CRGB colortab[4] = {CRGB(130,135,11), CRGB(84,120,68), CRGB(48,85,69), CRGB(28,55,52)};

void replaceled()
{
    //return;
    
    // return;
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
    /*
     for (int i=0;i<LED_HEIGHT;i++)
     {
     leds[i*LED_WIDTH]=CRGB::Black;
     leds[(i+1)*LED_WIDTH-1]=CRGB::Black;
     }*/
    //trun();
}

void FastLEDshowESP322()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
        // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle2);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}


void FastLEDshowTask2(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
        /*
         if (anim==0)
         //  int i=0;
         // displayPicNew(artnet.getframe(),0,0,48,123); //for the artnet
         memcpy(leds,artnet.getframe(),123*48*sizeof(CRGB));
         else
         memcpy(leds,Tpic,123*48*sizeof(CRGB));
         
         replaceled();
         //delay(1),
         //controller.showPixels();*/
       memcpy(leds,leds2,123*48*sizeof(CRGB));
       // printf("show\n");
        replaceled();
        FastLED.show();
        
        userTaskHandle=0; //so we can't have two display tasks at the same time
        
    }
}


// Use hardware SPI (on ESP D4 and D8 as above)
//Adafruit_ILI9341 tft = Adafruit_ILI9341(_CS, _DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _mosi, _sclk, _rst, _miso);

void backlighting(bool state) {
  /*  if (!state) {
        digitalWrite(_led, LOW);
    }
    else {
        digitalWrite(_led, HIGH);
    }*/
}

#define GAMEBOY_HEIGHT 160
#define GAMEBOY_WIDTH 144
byte pixels[(GAMEBOY_HEIGHT * GAMEBOY_HEIGHT+ GAMEBOY_WIDTH)/ 4];

static int button_start, button_select, button_a, button_b, button_down, button_up, button_left, button_right;

byte getColorIndexFromFrameBuffer(int x, int y) {
    int offset = x + y * 160 ;//160;
  return (pixels[offset >> 2] >> ((offset & 3) << 1)) & 3;
}

uint8_t  gamma8[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
uint8_t  gammar[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
float gammaCorrection=1;
float gammaCorrectionr=1;
int tableBrightness=64;
void calculateGammaTable()
{
    Serial.print("bri");
    Serial.println(tableBrightness);
    Serial.println((float)tableBrightness/255);
    float correctGamma=powf((float)tableBrightness/255,1/gammaCorrection);
    Serial.print("gamma correction:");
    Serial.println(correctGamma);
    //correctGamma=1;
    for (int i=0;i<256;i++)
    {
        
        float newValue=255*powf((float)i/255,correctGamma*gammaCorrection);
        //Serial.println(newValue);
        gamma8[i]=(int)newValue;
        // Serial.println(gamma8[i]);
        newValue=255*powf((float)i/255,correctGamma*(gammaCorrectionr));
        gammar[i]=(int)newValue;
        //Serial.println(gamma8[i]);
    }
}



void SDL_Flip(byte *screen){
  //tft.fillScreen(ILI9341_BLACK);
 /* int i,j;
    int j1,i1;
   // fill_solid(leds,160*144,CRGB(0,0,0));
  for(i = 0;i<GAMEBOY_WIDTH;i++){
    for(j = 0;j<GAMEBOY_HEIGHT;j++){ //GAMEBOY_HEIGHT
        
        
        j1=j;
        i1=i-20;
        if(i1>=0 && j1 >=0 && j1<48 && i1<123)
        {
            if(j1%2==0)
                leds[j1*123+ i1]= color[getColorIndexFromFrameBuffer(j, i)];
            else
                 leds[(123<<1) * ( (j1>>1) + 1)   -(1+i1)]= colortab[getColorIndexFromFrameBuffer(j, i)];
        }
      }
    }*/
 // replaceled();
  // FastLED.show();
   memcpy(leds,leds2,123*48*sizeof(CRGB));
       FastLEDshowESP322();
    //memset(pixels,0,GAMEBOY_HEIGHT * GAMEBOY_WIDTH / 4*sizeof(byte));
}



void sdl_init(void)
{
    
    //calculateGammaTable();
    colortab[0]=CRGB(gammar[130],gamma8[135],gamma8[11]);
    colortab[1]=CRGB(gammar[84],gamma8[120],gamma8[68]);
    colortab[2]=CRGB(gammar[48],gamma8[85],gamma8[69]);
    colortab[3]=CRGB(gammar[28],gamma8[55],gamma8[52]);
   /*
    color[0]=CRGB(gammar[91],gamma8[137],gamma8[37]);
    color[1]=CRGB(gammar[59],gamma8[88],gamma8[74]);
    color[2]=CRGB(gammar[29],gamma8[43],gamma8[80]);
    color[3]=CRGB(gammar[18],gamma8[27],gamma8[53]);
    
    color[0]=CRGB(gammar[0],gamma8[255],gamma8[0]);
    color[1]=CRGB(gammar[0],gamma8[170],gamma8[0]);
    color[2]=CRGB(gammar[0],gamma8[85],gamma8[0]);
    color[3]=CRGB(gammar[0],gamma8[0],gamma8[0]);*/
    
     xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 2000, NULL,0, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
    FastLED.addLeds<LED_TYPE, LED_PIN1, COLOR_ORDER>(leds, 0, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN2, COLOR_ORDER>(leds, OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN3, COLOR_ORDER>(leds, 2 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN4, COLOR_ORDER>(leds, 3 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    
    FastLED.addLeds<LED_TYPE, LED_PIN5, COLOR_ORDER>(leds, 4 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN6, COLOR_ORDER>(leds, 5 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN7, COLOR_ORDER>(leds, 6 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN8, COLOR_ORDER>(leds, 7 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    
    FastLED.addLeds<LED_TYPE, LED_PIN9, COLOR_ORDER>(leds, 8 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN10, COLOR_ORDER>(leds, 9 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN11, COLOR_ORDER>(leds, 10 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN12, COLOR_ORDER>(leds, 11 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    
    FastLED.addLeds<LED_TYPE, LED_PIN13, COLOR_ORDER>(leds, 12 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN14, COLOR_ORDER>(leds, 13 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN15, COLOR_ORDER>(leds, 14 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.addLeds<LED_TYPE, LED_PIN16, COLOR_ORDER>(leds, 15 * OFFSET_LEDS, NUM_LEDS_PER_STRIP);
    FastLED.setBrightness(tableBrightness);
   /*
  tft.begin();
  pinMode(_led, OUTPUT);
  backlighting(true);
  
  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  tft.fillScreen(ILI9341_RED);
*/
  gpio_pad_select_gpio(GPIO_NUM_32);
  gpio_set_direction(GPIO_NUM_32, GPIO_MODE_INPUT);

  gpio_pad_select_gpio(GPIO_NUM_33);
  gpio_set_direction(GPIO_NUM_33, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(GPIO_NUM_27);
    gpio_set_direction(GPIO_NUM_27, GPIO_MODE_INPUT);
}
int sdl_update(void){
	//tft.fillScreen(ILI9341_RED);
   button_start = !gpio_get_level(GPIO_NUM_32);
    button_right = !gpio_get_level(GPIO_NUM_33);
     button_a = !gpio_get_level(GPIO_NUM_27);
	return 0;
}
unsigned int sdl_get_buttons(void)
{
	return (button_start*8) | (button_select*4) | (button_b*2) | button_a;
}

unsigned int sdl_get_directions(void)
{
	return (button_down*8) | (button_up*4) | (button_left*2) | button_right;
}
CRGB* get_leds()
{
    return leds ;
}
CRGB* get_colors()
{
    return colortab;
}
byte* sdl_get_framebuffer(void)
{
	return pixels;
}
long time2,time3;


void sdl_frame(void)
{
  /*
	if(frames == 0)
		gettimeofday(&tv1, NULL);
	
	frames++;
	if(frames % 1000 == 0)
	{
		gettimeofday(&tv2, NULL);
		printf("Frames %d, seconds: %d, fps: %d\n", frames, tv2.tv_sec - tv1.tv_sec, frames/(tv2.tv_sec - tv1.tv_sec));
	}
 
    */
    
    printf("FPS:%f\n",(float)(240000000/(ESP.getCycleCount()-time3)));
    replaceled();
   FastLED.show();
    //FastLEDshowESP322();
    time3=ESP.getCycleCount();
	//SDL_Flip(pixels);
}


