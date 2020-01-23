#include <Arduino.h>
#include "lcd.h"
#include "cpu.h"
#include "interrupt.h"
#include "sdl.h"
#include "mem.h"
#define FASTLED_ESP32_I2S 1
#include "FastLED.h"


static int lcd_line;
static int lcd_ly_compare;


/* LCD STAT */
static int ly_int;	/* LYC = LY coincidence interrupt enable */
static int mode2_oam_int;
static int mode1_vblank_int;
static int mode0_hblank_int;
static int ly_int_flag;
static int lcd_mode;

/* LCD Control */
static int lcd_enabled;
static int window_tilemap_select;
static int window_enabled;
static int tilemap_select;
static int bg_tiledata_select;
static int sprite_size;
static int sprites_enabled;
static int bg_enabled;
static int scroll_x, scroll_y;
static int window_x, window_y;

static byte bgpalette[] = {3, 2, 1, 0};
static byte sprpalette1[] = {0, 1, 2, 3};
static byte sprpalette2[] = {0, 1, 2, 3};
static unsigned long colours[4] = {0xFFFFFF, 0xC0C0C0, 0x808080, 0x000000};

static int offset_led;
static int dir;

static    CRGB* l;
static  CRGB* cl;

uint8_t linerender[160];



static TaskHandle_t lcdtask = 0;
static TaskHandle_t userlcdtaskHandle = 0;

void render_line_task()
{
    if (userlcdtaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
        // noInterrupts();
        userlcdtaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(lcdtask);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}


void lcdcycleTask(void *pvParameters)
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
      //lcd_cycle();
        render_line(lcd_line);
        userlcdtaskHandle=0; //so we can't have two display tasks at the same time
        
    }
}



struct sprite {
	int y, x, tile, flags;
};

enum {
	PRIO  = 0x80,
	VFLIP = 0x40,
	HFLIP = 0x20,
	PNUM  = 0x10
};

unsigned char lcd_get_stat(void)
{

	return (ly_int)<<6 | lcd_mode;
}

void lcd_write_bg_palette(unsigned char n)
{
	bgpalette[0] = (n>>0)&3;
	bgpalette[1] = (n>>2)&3;
	bgpalette[2] = (n>>4)&3;
	bgpalette[3] = (n>>6)&3;
}

void lcd_write_spr_palette1(unsigned char n)
{
	sprpalette1[0] = 0;
	sprpalette1[1] = (n>>2)&3;
	sprpalette1[2] = (n>>4)&3;
	sprpalette1[3] = (n>>6)&3;
}

void lcd_write_spr_palette2(unsigned char n)
{
	sprpalette2[0] = 0;
	sprpalette2[1] = (n>>2)&3;
	sprpalette2[2] = (n>>4)&3;
	sprpalette2[3] = (n>>6)&3;
}

void lcd_write_scroll_x(unsigned char n)
{
//	printf("x scroll changed to %02x\n", n);
	scroll_x = n;
}

void lcd_write_scroll_y(unsigned char n)
{
	scroll_y = n;
}

int lcd_get_line(void)
{
	return lcd_line;
}

void lcd_write_stat(unsigned char c)
{
	ly_int = !!(c&0x40);
}

void lcd_write_control(unsigned char c)
{
//	printf("LCDC set to %02x\n", c);
//	cpu_print_debug();
	bg_enabled            = !!(c & 0x01);
	sprites_enabled       = !!(c & 0x02);
	sprite_size           = !!(c & 0x04);
	tilemap_select        = !!(c & 0x08);
	bg_tiledata_select    = !!(c & 0x10);
	window_enabled        = !!(c & 0x20);
	window_tilemap_select = !!(c & 0x40);
	lcd_enabled           = !!(c & 0x80);
}

void lcd_set_ly_compare(unsigned char c)
{
	lcd_ly_compare = c;
}

void lcd_set_window_y(unsigned char n) {
	window_y = n;
}

void lcd_set_window_x(unsigned char n) {
	window_x = n;
}

static void swap(struct sprite *a, struct sprite *b)
{
	struct sprite c;

	 c = *a;
	*a = *b;
	*b =  c;
}

static void sort_sprites(struct sprite *s, int n)
{
	int swapped, i;

	do
	{
		swapped = 0;
		for(i = 0; i < n-1; i++)
		{
			if(s[i].x < s[i+1].x)
			{
				swap(&s[i], &s[i+1]);
				swapped = 1;
			}
		}
	}
	while(swapped);
}




void lcd_init()
{
    l=get_leds();
  cl=get_colors();
   //xTaskCreatePinnedToCore(lcdcycleTask, "lcdcycleTask", 2000, NULL,0, &lcdtask, 0);
}


void drawColorIndexToFrameBuffer(int x, int y, byte idx, byte *b) {
  //return;
  int offset;
 // int offset = x + y * 160;
  //b[offset >> 2] |= (idx << ((offset & 3) << 1));
/*  CRGB* l;
  CRGB* cl;
  l=get_leds();
  cl=get_colors();*/
  y=143-y-95;
  x=x-37;
  if(x<123 && y< 48 && x>=0  && y >=0)
  {
    if(y%2==0)
     offset= x+y*123;
      else
      offset= (123<<1) * ( (y>>1) + 1)   -(1+x);
      l[offset]=cl[idx];
  }
}

void newdrawColorIndexToFrameBuffer(int line)
{

 // l=get_leds();
 // cl=get_colors();
  line=143-line;//-95;
  if( line >47 || line <0)
  return;
 // printf("%d\n",line);
if(line%2==0)
{
  int offset=123*line;
    for(int x=0;x<123;x++)
    {
      l[offset]=cl[linerender[x]];
      offset++;
    }

}
else
{
   int offset=(123<<1) * ( (line>>1) + 1)   -1;
    for(int x=0;x<123;x++)
    {
      l[offset]=cl[linerender[x]];
      offset--;
    }
}
  
}

static void draw_bg_and_window(byte *b, int line)
{
	int x;
  unsigned int  xm, ym;
  unsigned int map_select, map_offset, tile_num, tile_addr;
    unsigned char b1, b2, mask, colour;
  bool m;

if(line >= window_y && window_enabled && line - window_y < 144)
    {
     // xm = x;
      ym = line - window_y;
      map_select = window_tilemap_select;
      m=false;
    }
    else {
      if(!bg_enabled)
      {
        //b[line*640 + x] = 0;
       //drawColorIndexToFrameBuffer(x,line,0,b);
        return;
      }
     // xm = (x + scroll_x)%256;
      ym = (line + scroll_y)%256;
      map_select = tilemap_select;
      m=true;
    }

int off2=((ym%8)<<1);
int off3=((ym%8)<<1)+1;
	for(x = 0; x < 123; x++) //160
	{
		
/*
		// Convert LCD x,y into full 256*256 style internal coords 
		if(line >= window_y && window_enabled && line - window_y < 144)
		{
			xm = x;
			ym = line - window_y;
			map_select = window_tilemap_select;
		}
		else {
			if(!bg_enabled)
			{
				//b[line*640 + x] = 0;
       //drawColorIndexToFrameBuffer(x,line,0,b);
				return;
			}
			xm = (x + scroll_x)%256;
			ym = (line + scroll_y)%256;
			map_select = tilemap_select;
		}
   */

   
   if(m)
     xm = (x+scroll_x) & 0xFF; //(x + scroll_x)%256;
   else
     xm=x;

		/* Which pixel is this tile on? Find its offset. */
		/* (y/8)*32 calculates the offset of the row the y coordinate is on.
		 * As 256/32 is 8, divide by 8 to map one to the other, this is the row number.
		 * Then multiply the row number by the width of a row, 32, to find the offset.
		 * Finally, add x/(256/32) to find the offset within that row. 
		 */
	//	map_offset = (ym/8)*32 + xm/8;

    map_offset = ((ym>>3)<<5) +  (xm>>3);
    
		tile_num = mem_get_raw(0x9800 + map_select*0x400 + map_offset);
		if(bg_tiledata_select)
			tile_addr = 0x8000 + (tile_num<<4);
		else
			tile_addr = 0x9000 + (((signed char)tile_num)<<4);

		b1 = mem_get_raw(tile_addr+off2);     //((ym%8)<<1)
		b2 = mem_get_raw(tile_addr+off2+1);    //((ym%8)<<1)+1
		mask =128>>(xm&7); //128>>(xm%8);
		colour = (!!(b2&mask)<<1) | !!(b1&mask);
		//b[line*640 + x] = colours[bgpalette[colour]];
	  //drawColorIndexToFrameBuffer(x,line,bgpalette[colour],b);
   //linerender[x]=bgpalette[colour];
     l[offset_led+x*dir]=cl[bgpalette[colour]];
	}
}

static void draw_sprites(byte *b, int line, int nsprites, struct sprite *s)
{
	int i;

	for(i = 0; i < nsprites; i++)
	{
		unsigned int b1, b2, tile_addr, sprite_line, x;

		/* Sprite is offscreen */
		if(s[i].x < -7 || s[i].x>=123)
			continue;

		/* Which line of the sprite (0-7) are we rendering */
		sprite_line = s[i].flags & VFLIP ? (sprite_size ? 15 : 7)-(line - s[i].y) : line - s[i].y;

		/* Address of the tile data for this sprite line */
		tile_addr = 0x8000 + (s[i].tile*16) + sprite_line*2;

		/* The two bytes of data holding the palette entries */
		b1 = mem_get_raw(tile_addr);
		b2 = mem_get_raw(tile_addr+1);
    byte *pal;
   pal = (s[i].flags & PNUM) ? sprpalette2 : sprpalette1;
		/* For each pixel in the line, draw it */
		for(x = 0; x < 8; x++)
		{
			unsigned char mask, colour;
			//byte *pal;

			if((s[i].x + x) >= 123 ) //160
        continue;
        
			mask = s[i].flags & HFLIP ? 128>>(7-x) : 128>>x;
			colour = ((!!(b2&mask))<<1) | !!(b1&mask);
			if(colour == 0)
				continue;


			//pal = (s[i].flags & PNUM) ? sprpalette2 : sprpalette1;
			/* Sprite is behind BG, only render over palette entry 0 */
     //change yves
			if(s[i].flags & PRIO)
			{
				unsigned int temp =linerender[x + s[i].x]; //b[line*640+(x + s[i].x)];
				if(temp != colours[bgpalette[0]])
					continue;
			}
			//b[line*640+(x + s[i].x)] = colours[pal[colour]];
		  //drawColorIndexToFrameBuffer(x + s[i].x,line,pal[colour],b);
     //linerender[x + s[i].x]=pal[colour]; //x + s[i].x
    // printf("line: %d led :%d\n",line,offset_led+(x + s[i].x)*dir);
     l[offset_led+(x + s[i].x)*dir]=cl[pal[colour]];
		}
	}
}



static void render_line(int line)
{
	int i, c =0,line2;
  
 

	struct sprite s[10];
	byte *b = sdl_get_framebuffer();

	for(i = 0; i<40; i++)
	{
		int y;

		y = mem_get_raw(0xFE00 + (i*4)) - 16;
		if(line < y || line >= y + 8+(sprite_size*8))
			continue;

		s[c].y     = y;
		s[c].x     = mem_get_raw(0xFE00 + (i*4) + 1)-8;
		s[c].tile  = mem_get_raw(0xFE00 + (i*4) + 2);
		s[c].flags = mem_get_raw(0xFE00 + (i*4) + 3);
		c++;

		if(c == 10)
			break;
	}
 //if(line==143 || line==142)
 //printf("%u %u %u %u %u %u %u\n",s[0].y,s[1].y,s[2].y,s[3].y,s[4].y,s[5].y,s[6].y);
line2=143-line-14;
 if(line2<0 || line2 >47)
 return;
 if (line2%2==0)
 {
    offset_led=123*line2;
    dir=1;
 }
 else
 {
    offset_led=(123<<1) * ( (line2>>1) + 1)   -1;
    dir=-1;
 }

	if(c)
		sort_sprites(s, c);
//memset(linerender,0,160);
	/* Draw the background layer */
	draw_bg_and_window(b, line);

	draw_sprites(b, line, c, s);
 // newdrawColorIndexToFrameBuffer(line);

}

static void draw_stuff(void)
{
  byte *b = sdl_get_framebuffer();
	int y, tx, ty;

	for(ty = 0; ty < 24; ty++)
	{
	for(tx = 0; tx < 16; tx++)
	{
	for(y = 0; y<8; y++)
	{
		unsigned char b1, b2;
		int tileaddr = 0x8000 +  ty*0x100 + tx*16 + y*2;

		b1 = mem_get_raw(tileaddr);
		b2 = mem_get_raw(tileaddr+1);
   /*
		b[(ty*640*8)+(tx*8) + (y*640) + 0 + 0x1F400] = colours[(!!(b1&0x80))<<1 | !!(b2&0x80)];
		b[(ty*640*8)+(tx*8) + (y*640) + 1 + 0x1F400] = colours[(!!(b1&0x40))<<1 | !!(b2&0x40)];
		b[(ty*640*8)+(tx*8) + (y*640) + 2 + 0x1F400] = colours[(!!(b1&0x20))<<1 | !!(b2&0x20)];
		b[(ty*640*8)+(tx*8) + (y*640) + 3 + 0x1F400] = colours[(!!(b1&0x10))<<1 | !!(b2&0x10)];
		b[(ty*640*8)+(tx*8) + (y*640) + 4 + 0x1F400] = colours[(!!(b1&0x8))<<1 | !!(b2&0x8)];
		b[(ty*640*8)+(tx*8) + (y*640) + 5 + 0x1F400] = colours[(!!(b1&0x4))<<1 | !!(b2&0x4)];
		b[(ty*640*8)+(tx*8) + (y*640) + 6 + 0x1F400] = colours[(!!(b1&0x2))<<1 | !!(b2&0x2)];
		b[(ty*640*8)+(tx*8) + (y*640) + 7 + 0x1F400] = colours[(!!(b1&0x1))<<1 | !!(b2&0x1)];
   */
	}
	}
	}
}

int lcd_cycle(void)
{
  
	int cycles = cpu_get_cycles();
	int this_frame, subframe_cycles;
	static int prev_line;

	if(sdl_update())
		return 0;

	this_frame = cycles % (70224/8);//(70224/4);
	lcd_line = this_frame / (456/8); //(456/4);
//printf("line:%d\n",lcd_line);
	if(this_frame < 204/4)
		lcd_mode = 2;
	else if(this_frame < 284/4)
		lcd_mode = 3;
	else if(this_frame < 456/4)
		lcd_mode = 0;
	if(lcd_line >= 144)
		lcd_mode = 1;
		
	if(lcd_line != prev_line && lcd_line < 144)
 //render_line_task();
		render_line(lcd_line);

	if(ly_int && lcd_line == lcd_ly_compare)
		interrupt(INTR_LCDSTAT);

	if(prev_line == 143 && lcd_line == 144)
	{
  
		//draw_stuff();
		interrupt(INTR_VBLANK);
   //while (userlcdtaskHandle>0)
		sdl_frame();
	}
	prev_line = lcd_line;
	return 1;
}

