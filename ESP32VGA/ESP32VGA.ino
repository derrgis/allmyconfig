/*
	Author: bitluni 2019	
	For further details check out: 
		https://youtube.com/bitlunislab
		https://github.com/bitluni
		http://bitluni.net
*/

#include <Arduino.h>
#include "src/ESP32Lib/src/i2s/I2S.h"
#include <soc/rtc.h>
#define NUM_VIRT_PINS 7
//#include "src/ESP32Lib/src/Graphics/Mesh.h"
//#include "src/ESP32Lib/src/Graphics/Sprites.h"

typedef unsigned short Color;

#include "gfx/font6x8.h"
#include "gfx/thinker.h"
//#include "gfx/venus.h"
//Font<Graphics<Color>> font(6, 8, font6x8::pixels);
//Mesh<Graphics<Color>> model(venus::vertexCount, venus::vertices, 0, 0, venus::triangleCount, venus::triangles, venus::triangleNormals);
////Mesh<Graphics<Color>> model2(thinker::vertexCount, thinker::vertices, 0, 0, thinker::triangleCount, thinker::triangles, thinker::triangleNormals);

//Note that the I/O GPIO pads are 0-19, 21-23, 25-27, 32-39, while the output GPIOs are 0-19, 21-23, 25-27, 32-33. GPIO pads 34-39 are input-only
//0, 2, 4, 5, 12,  13, 14, 15, 16,  17, 18, 19, 21,  22, 23, 27, 32, 33
//0 (boot mode: use only as blank output)
//5 (LED)
//21 (SDA), 22(SCL)
//25, 26 (DAC)
//1(TX), 3(RX)

//pin configuration, (you could use 32 and 33, for the syncs instead)
const int redPins[] = {2, 4, 12, 13, 14};
const int greenPins[] = {15, 16, 17, 18, 19};
const int bluePins[] = {21, 22, 23, 27};
const int hsyncPin = 0;
const int vsyncPin = 5;

//VGA Device (I2S0)
//I2SVGA graphics(0);
I2S pics(0);


void wc( uint16_t *buf, int x, uint16_t r, uint16_t g, uint16_t b,int channel)
{
  uint16_t k=1<<channel;
  //Serial.printf("%ld\n",k);
 // buf[0]=0xffffffff;
  //Serial.printf("%ld\n",buf);
   uint16_t pos=(0+1)*72*(NUM_VIRT_PINS+1)-(NUM_VIRT_PINS+1)-1;//(x+1)*96-1;
   for(int i=0;i<8;i++)
  {
    uint16_t l=(b&1)*k;
    b=b>>1;
    
 //Serial.printf("pos:%d\n",pos);
    
   /* buf[pos-1 ]=  l| buf[pos-1 ];
    buf[pos-2]=l |buf[pos-1 ];
   //Serial.println(l);
      buf[pos-3]=0xffffffff;
       buf[pos]=0;
    pos=pos-4;*/

    
    // buf[pos]=  l| buf[pos ];
     buf[pos-2]=  l| buf[pos-2];
     //buf[pos-3]=  l| buf[pos-3];
    // buf[pos-4]=  l| buf[pos-4];
    // buf[pos-5]=  l| buf[pos -5];
    // buf[pos-6]=  l| buf[pos -6];
     buf[pos-7]=  l| buf[pos -7];
    
   //Serial.println(l);
      //buf[pos-2]=0xFF7F;
      // buf[pos]=0;
   pos=pos-24;
  }
for(int i=0;i<8;i++)
  {
uint16_t l=(r&1)*k;
    r=r>>1;
//buf[pos]=  l| buf[pos ];
     buf[pos-2]=  l| buf[pos-2];
    // buf[pos-3]=  l| buf[pos-3];
    // buf[pos-4]=  l| buf[pos-4];
     //buf[pos-5]=  l| buf[pos -5];
     //buf[pos-6]=  l| buf[pos -6];
     buf[pos-7]=  l| buf[pos -7];
    
   //Serial.println(l);
      //buf[pos-2]=0xFF7F;
      // buf[pos]=0;
   pos=pos-24;
  }
  for(int i=0;i<8;i++)
  {
 uint16_t l=(g&1)*k;
    g=g>>1;
 //   buf[pos]=  l| buf[pos ];
     buf[pos-2]=  l| buf[pos-2];
     //buf[pos-3]=  l| buf[pos-3];
    // buf[pos-4]=  l| buf[pos-4];
    // buf[pos-5]=  l| buf[pos -5];
     //buf[pos-6]=  l| buf[pos -6];
     buf[pos-7]=  l| buf[pos -7];
    
   //Serial.println(l);
      //buf[pos-2]=0xFF7F;
      // buf[pos]=0;
   pos=pos-24;
  }
    /*
      for(int i=0;i<8;i++)
  {
    uint32_t l=(r&1)*k;
    r=r>>1;
    //writeSample(buf,1,pos-3);
        
     buf[pos-1 ]=  l| buf[pos-1 ];
    
   //Serial.println(l);
      buf[pos-2]=0xFF7F;
       buf[pos]=0;
   pos=pos-3;
  }
     for(int i=0;i<8;i++)
  {
    uint32_t l=(g&1)*k;
    g=g>>1;
    //writeSample(buf,1,pos-3);
        
     buf[pos-1 ]=  l| buf[pos-1 ];
    
   //Serial.println(l);
      buf[pos-2]=0xFF7F;
       buf[pos]=0;
   pos=pos-3;
  }*/

  
}
  void pu(uint16_t* buff)
    {
  memset((uint8_t*)buff,0,(NUM_VIRT_PINS+1)*8*3*3*2);
  for (int i=0;i<24*3;i++)
    {
     buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)-1]=0x8000;
      //buff[NUM_VIRT_PINS+i*(NUM_VIRT_PINS+1)]=0x02;
    }
    }
    void pu2(uint16_t* buff)
    {
  
    
     for (int j=0;j<24;j++)
      {
     // for (int i=0;i<NUM_VIRT_PINS;i++)
   // {
     //*buff=0x7FFF;
    // buff++;
     buff[1+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
      buff[0+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
       buff[3+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
        buff[2+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
         buff[5+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
          buff[4+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
           buff[7+j*(3*(NUM_VIRT_PINS+1))]=0x7FFF;
     //buff[NUM_VIRT_PINS-2+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
     //buff[NUM_VIRT_PINS-3+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
    // buff[NUM_VIRT_PINS-4+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
     //buff[NUM_VIRT_PINS-5+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
     //buff[NUM_VIRT_PINS-6+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
     //buff[NUM_VIRT_PINS-4+j*(3*(NUM_VIRT_PINS+1))]=0x00FF;
   // }
    //buff+=3*(NUM_VIRT_PINS+1)-NUM_VIRT_PINS; //13
      }
    }

void writeColor(uint8_t *buf, int x, uint8_t r, uint8_t g, uint8_t b) //uint8_t
{
  //Serial.println("pixel-------");
 //wc(buf,x,r,g,b);
  //return;
  uint16_t pos=(x+1)*2*96-2;//96 bit per color 4*24
 // Serial.println(pos);
 //Serial.println("bleu");
  for(int i=0;i<8;i++)
  {
    uint8_t l=b&1&3;
    b=b>>1;
    //writeSample(buf,1,pos-3);
    buf[pos-6]=l;
    buf[pos]=l;
   //Serial.println(l);
      buf[pos-4]=255;
       buf[pos-2]=0;
  
    pos=pos-8;
  }
  //Serial.println("rouge");
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint8_t l=r&1&3;
    r=r>>1;
 buf[pos-6]=l;
    buf[pos]=l;
   //Serial.println(l);
      buf[pos-4]=255;
       buf[pos-2]=0;
   
    pos=pos-8;
  }
  //Serial.println("vert");
  
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint8_t l=g&1&3;
    g=g>>1;
     
buf[pos-6]=l;
    buf[pos]=l;
   //Serial.println(l);
      buf[pos-4]=255;
       buf[pos-2]=0;
   //  Serial.println(pos-6);
    pos=pos-8;
  }
 // Serial.println(pos);
 //Serial.println("fin pixel-------");
}

void er()
{
  Serial.println("ee");
  
}

void writeColor3bid(uint8_t *buf, int x, uint16_t r, uint16_t g, uint16_t b)
{
  uint16_t pos=(x+1)*2*72-2;//96 bit per color 4*24
 // Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint16_t l=b&1;
    b=b>>1;
    //writeSample(buf,1,pos-3);
   // buf[pos-6]=1*3;
    buf[pos]=0;
   
      buf[pos-4]=1;
       buf[pos-2]=l*3;
  
    pos=pos-6;
  }
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint16_t l=r&1;
    r=r>>1;
    buf[pos]=0;
   
      buf[pos-4]=1;
       buf[pos-2]=l*3;
   
    pos=pos-6;
  }
  int8_t dummy[1];
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint16_t l=g&1;
    g=g>>1;
     
    buf[pos]=0;
   
      buf[pos-4]=1;
       buf[pos-2]=l*3;
   //  Serial.println(pos-6);
    pos=pos-6;
  }
 // Serial.println(pos);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("hello");

    Serial.println(esp_get_free_heap_size());
  Serial.println(ESP.getFlashChipSpeed());
  Serial.println(rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get()));
  int dmaBufferCount=10;
  int bytesPerPixel=3;
  pics.callback=&er;
  pics.dmaBufferCount=dmaBufferCount;
  pics.dmaBuffers = (DMABuffer **)malloc(sizeof(DMABuffer *) * dmaBufferCount);
  if(!pics.dmaBuffers)
    Serial.println("argh");


  int pinMap[24]={32,33,26,17,25,0,2,4,12,13,14,15,16,18,19,22,23,5,-1,-1,-1,-1,-1,-1};
   /* for (int i=0;i<24;i++)
    {
        pinMap[i]=-1;
    } */
    //pinMap[0]=0;
    
    //pinMap[15]=14;
      //pinMap[10]=13;

  /*
  //initializing i2s vga and frame buffers
  graphics.init(graphics.MODE200x150, redPins, greenPins, bluePins, hsyncPin, vsyncPin, 16);
  //select font
  graphics.setFont(font);*/
    Serial.println("eeez");
  pics.initParallelOutputMode(pinMap,2400000);
   Serial.println("eeez2");

for (int i = 0; i < dmaBufferCount; i++)
  {
    Serial.printf("buufer:%d\n",i);
    pics.dmaBuffers[i] = DMABuffer::allocate((NUM_VIRT_PINS+1)*3*8*3*2); //front porch + hsync + back porch + pixels
    if (i)
      pics.dmaBuffers[i - 1]->next(pics.dmaBuffers[i]);
  }
  pics.dmaBuffers[dmaBufferCount - 1]->next(pics.dmaBuffers[0]);
  //red


  //Serial.printf("buffer:%d\n",pics.dmaBuffers[1]->buffer);
  //empty();
  for(int i=0;i<dmaBufferCount;i++)
  {
     pu((uint16_t *)pics.dmaBuffers[i]->buffer);
  }

    for(int i=1;i<dmaBufferCount;i++)
  {
     pu2((uint16_t *)pics.dmaBuffers[i]->buffer);
  }

wc((uint16_t*)pics.dmaBuffers[1]->buffer, 0, 255, 0,255,1);


wc((uint16_t *)pics.dmaBuffers[2]->buffer, 0, 255, 0,0,1);
wc((uint16_t *)pics.dmaBuffers[2]->buffer, 0, 0, 255,0,2);
wc((uint16_t *)pics.dmaBuffers[3]->buffer, 0, 255, 0,0,1);
wc((uint16_t *)pics.dmaBuffers[4]->buffer, 0, 0, 0,255,14);

//writeColor(pics.dmaBuffers[0]->buffer, 0, 255, 0,0);
/*
pics.dmaBuffers[0]->buffer[0]=8;

pics.dmaBuffers[0]->buffer[1]=1;
pics.dmaBuffers[0]->buffer[2]=1;
pics.dmaBuffers[0]->buffer[3]=129;
pics.dmaBuffers[0]->buffer[4]=8;

pics.dmaBuffers[0]->buffer[5]=1;

//pics.dmaBuffers[0]->buffer[1]=255;
//pics.dmaBuffers[0]->buffer[2]=255;
//pics.dmaBuffers[0]->buffer[3]=255;
//pics.dmaBuffers[0]->buffer[4]=255;*/
/*for(int i=0;i<96;i++)
  {
    pics.dmaBuffers[0]->buffer[i*4+1]=1;
  }*/
  pics.startTX();
  /*for(int i=0;i<96;i++)
  {
    Serial.printf("%d: %d\n",i,pics.dmaBuffers[0]->buffer[i*4+1]);
  }*/
 // memset(pics.dmaBuffers[0]->buffer,0,4000);
 Serial.println("eee");
  delay(1500);
  Serial.println("eee");
}

//render 3d model


//show output
void empty()
{
  //memset(pics.dmaBuffers[0]->buffer,0,96*9*4);
  //memset(pics.dmaBuffers[1]->buffer,0,96*9*4);

/*pics.dmaBuffers[0]->buffer[12]=0;
pics.dmaBuffers[0]->buffer[14]=0;

 pics.dmaBuffers[0]->buffer[16]=1;
  pics.dmaBuffers[0]->buffer[18]=1;
  
 pics.dmaBuffers[0]->buffer[20]=0;
  pics.dmaBuffers[0]->buffer[22]=1;


pics.dmaBuffers[0]->buffer[24]=1;

pics.dmaBuffers[0]->buffer[26]=0;
pics.dmaBuffers[0]->buffer[28]=0;
pics.dmaBuffers[0]->buffer[30]=0;*/
 
//pics.dmaBuffers[0]->buffer[18]=1;

  for (int i=0;i<9;i++)
  {
   
  // wc((uint32_t *)pics.dmaBuffers[0]->buffer, i, 0, 0,0,0);
  }
  /*  for (int i=0;i<11;i++)
  {
    //Serial.println(i);
    writeColor(pics.dmaBuffers[1]->buffer, i, 0,0,0 );
  }*/
  
}

//just draw
int k=0;
void loop()
{
    //delay(200);
    //Serial.println("on ecrit");
  //draw();
//empty();


/*
  writeColor(pics.dmaBuffers[0]->buffer, 0, 0, 0,255 );
  writeColor(pics.dmaBuffers[0]->buffer, 1, 255, 0,0);
 writeColor(pics.dmaBuffers[0]->buffer, 2, 255, 0,0);
  writeColor(pics.dmaBuffers[0]->buffer, 3, 0, 255,0);
   writeColor(pics.dmaBuffers[0]->buffer, 4, 0, 0,255);
  pics.startTX();
delay(1);
empty();
  writeColor(pics.dmaBuffers[0]->buffer, 10, 255, 0,255 );
  writeColor(pics.dmaBuffers[0]->buffer, 9, 255, 0,0);
 writeColor(pics.dmaBuffers[0]->buffer, 7, 255, 0,0);
  writeColor(pics.dmaBuffers[0]->buffer, 2, 0, 255,0);
   writeColor(pics.dmaBuffers[0]->buffer, 1, 0, 0,255);*/
  /* 
  for(int j=0;j<20;j++)
   {
    for(int i=0;i<9;i++)
    {
      long d=2.55*i*i;
      wc((uint32_t *)pics.dmaBuffers[0]->buffer, (k+i)%9, 255*(j%3)*i*i*1/1000, 255*((j+1)%3)*i*i*1/1000,255*((j+2)%3)*i*i*i/1000,j);
   //  writeColor(pics.dmaBuffers[0]->buffer, (k+1+i)%11, 0, 0,255*i*i/100);
    }
   }*/
    pu((uint16_t *)pics.dmaBuffers[k]->buffer);
  pu2((uint16_t *)pics.dmaBuffers[k]->buffer);
   k=(k+1)%9;
  // delay(1);
  // Serial.println("on pusht");
  //pics.startTX();
   for (int i=0;i<9;i++)
   {
     wc((uint16_t *)pics.dmaBuffers[(k+i)%9]->buffer, 0, 0, 0,10*i,14);
   }

  
pics.startTX();
delay(100);
}
