#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"

int res=0;
void _addb_(int src,int dest) 
{
__asm__(
  
  "mov a2,%1\n\t"
  //"mov a1,%2\n\t"
  //"add a2,a2,a1\n\t"
  "movi a3,0x06\n\t"
  "ssl a3\n\t"
  "sll a1,a2\n\t"
  "sub a1,a1,a2\n\t"
  "sub a1,a1,a2\n\t"
  "sub a1,a1,a2\n\t"
  "sub a1,a1,a2\n\t" //60*dest
  "mov a2,%2\n\t"
  "add a2,a2,a1\n\t"
  
  "mov %0,a2\n\t"
  : "=r" (res): "r" (dest) , "r" (src) );
}
CRGB leds[3000];
int LED_WIDTH=60;
int LED_HEIGHT=50;
float maxi=0;
float mini=9999999;
CRGB line[60];

void memfill(void *dest, size_t destsize, size_t elemsize) {
   char   *nextdest = (char *) dest + elemsize;
   size_t movesize, donesize = elemsize;

   destsize -= elemsize;
   while (destsize) {
      movesize = (donesize < destsize) ? donesize : destsize;
      memcpy(nextdest, dest, movesize);
      nextdest += movesize; destsize -= movesize; donesize += movesize;
   }
}    
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

}


void straightLine(int x1,int y1,int x2,CRGB Color)
{
  if (y1<0  or y1>=LED_HEIGHT)
  return;
  if(x1 >= LED_WIDTH and x2>=LED_WIDTH)
  return;
 if(x1 <0 and x2<0)
  return;
if(x1<0)
  x1=0;
 if(x1>=LED_WIDTH)
  x1=LED_WIDTH-1;


if(x2<0)
  x2=0;
 if(x2>=LED_WIDTH)
  x2=LED_WIDTH-1;
  
  
  line[0]=Color;
  memfill(line, sizeof(line), sizeof(line[0]));
  int fx=0;
  int longueur=0;
  if(y1%2==0)
  {
    if(x1>x2)
      {
        fx=x2;
        longueur=x1-x2+1;
      }
      else
      {
        fx=x1;
        longueur=x2-x1+1;
      }
    memcpy((char *) leds +(fx+y1*LED_WIDTH)*sizeof(CRGB), line, longueur*sizeof(CRGB));
    
  }
  else
  {
    if(x1>x2)
      {
        fx=x1;
        longueur=x1-x2+1;
      }
      else
      {
        fx=x2;
        longueur=x2-x1+1;
      }
    memcpy((char *) leds +((LED_WIDTH<<1) * ((y1 >>1)  + 1) - 1 - fx)*sizeof(CRGB), line, longueur*sizeof(CRGB));
  }
}

void PixelOn(uint16_t x,uint16_t y,CRGB Color)
{
    //int offset=0;
   // size_t s=sizeof(CRGB);
    if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return ;
    if(y%2==0)
       // leds[x+(y<<6)-(y<<2) ]=Color;
        //leds[x+y*LED_WIDTH]=Color;
        //
    memcpy((char *) leds +(x+y*LED_WIDTH)*3, &Color, 3);
   else
    
       //int offset2=2 * LED_WIDTH * ((int)floor(y /2)  + 1) - 1 - x;
      //leds[(LED_WIDTH<<1) * ((y >>1)  + 1) - 1 - x]=Color;
     // memcpy((char *) leds +(LED_WIDTH*y)*3, &Color, 3);
   leds[(LED_WIDTH*y+LED_WIDTH)-(1+x)]=Color;
     
    

   // leds[ (y%2==0) ? x+y*LED_WIDTH : (LED_WIDTH<<1) * ((y >>1)  + 1) - 1 - x]=Color;
    //leds[offset]=Color;
     
}
void loop() {
  // put your main code here, to run repeatedly:
              long time1=ESP.getCycleCount();
              //for(int k=0;k<1000;k++)
              for (int i=0;i<LED_WIDTH;i++)
              {
                for (int j=0;j<LED_HEIGHT;j++)
                {
            //      __asm__(
                //   " s32e  a1, a5, -12\n\t"
//"s32e  a2, a5,  -8\n\t"
//"s32e  a3, a5,  -4\n\t"
//"rfwo\n\t"
//"nop\n\t"
                  //"mov a2,%1\n\t"
 
  //"movi a3,0x06\n\t"
 // "ssl a3\n\t"
  //"sll a1,a2\n\t"
  //"sub a1,a1,a2\n\t"
  //"sub a1,a1,a2\n\t"
 // "sub a1,a1,a2\n\t"
 // "sub a1,a1,a2\n\t" //60*dest
  //"mov a2,%2\n\t"
  //"add a2,a2,a1\n\t"
  
 // "mov %0,a2\n\t"

 // "l32e  a1, a5, -12 \n\t"  // restore a1 from call[i+1]’s frame
 //"    l32e  a2, a5,  -8 \n\t"  // restore a2 from call[i+1]’s frame
 //"    l32e  a3, a5,  -4 \n\t"  // restore a3 from call[i+1]’s frame
 // "   rfwu\n\t"
  //:: "r" (res) ,"r" (j) , "r" (i) );
                 // _addb_(i,j);
                  int h=60*j+i;
                 //Serial.printf("%d %d %d\n",i,j,res);
                }
                  //straightLine(20,30,30,CRGB::Red );
                  //Serial.printf("%d %d  %d\n",i,j,i+(j<<6) - (j<<2));
                  //PixelOn(i,j,CRGB::Red);
              }
              long time3=ESP.getCycleCount();
             
             
             
                                
  //Serial.println(sizeof(CRGB));
   //Serial.print((float)240000000/(time3-time1));
   //Serial.println(" FPS scroll total");
   if(maxi<(float)(time3-time1)/240)
   {
    maxi=(float)(time3-time1)/240;
    Serial.printf("new max:%f\n",maxi);
   }
      if(mini>(float)(time3-time1)/240)
   {
    mini=(float)(time3-time1)/240;
    Serial.printf("new min:%f\n",maxi);
   }
  // Serial.print((float)(time3-time1)/240);
  // Serial.println(" nanos seconds total");
  delay(1000);
}

