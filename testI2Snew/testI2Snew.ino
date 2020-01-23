
#include <iostream> 
#include <unordered_map> 

using namespace std;
#define DOWN_RIGHT   0 //natural mode no transpostion to be made
#define DOWN_LEFT    1 //W2e turn 90° clock wise
#define UP_LEFT      2
#define UP_RIGHT     3
#define DOWN_RIGHT_INV   4 //natural mode no transpostion to be made
#define DOWN_LEFT_INV    5 //We turn 90° clock wise
#define UP_LEFT_INV      6
#define UP_RIGHT_INV 7
#define LED_WIDTH 50
#define NUM_STRIPS NBIS2SERIALPINS * 8  
#define LED_HEIGHT_PER_STRIP 1
#define LED_HEIGHT_PANEL  LED_HEIGHT_PER_STRIP
#define NUM_LEDS_PER_STRIP 50
#define LED_HEIGHT NUM_STRIPS*LED_HEIGHT_PER_STRIP
 //up to 22
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP
#define ESP32_VIRTUAL_DRIVER true
#define ESP_VIRTUAL_DRIVER_8 1
#define STATIC_COLOR_GRB 1
#define NBIS2SERIALPINS 8

/*

#include <WiFi.h>

#include <WiFiUdp.h>
*/
//#define RGB_ORDER RGB
#include "FastLED.h"
//#include "I2S.h"
//#include "fontamstrad.h"
#define LATCH_PIN 13
#define CLOCK_PIN 27
CRGB solidColor = CRGB(0, 0, 0);
CRGB bgColor = CRGB(10,10,10);
CRGB Color = CRGB :: Blue;
CRGB leds[NUM_LEDS];
CRGB Tpic[1];
CRGB TransColor=CRGB(1,1,1);

std::unordered_map<char, int> umap; 

void PixelOn(int x,int y,CRGB Color)
{
    //int offset=0;
    if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return ;
    
        leds[x+y*LED_WIDTH]=Color;
   
    
}

 CRGB getPixelValue(int x, int y)
{
  if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return CRGB(0,0,0);
    
        return leds[x+y*LED_WIDTH];
   
}


//#include "graphicfunction.h"
int tableOrientation=DOWN_RIGHT_INV;
    
int Pins[NBIS2SERIALPINS] = {12, 14, 26, 25, 33, 32, 15, 18};//,-1,-1,-1,-1,18,19,21,23,25,22,0,-1,3,16,15,33};//,-1,-1,-1,12};

//I2S controller(0);




 /*
 void afficheMessage2(char *mess,int x,int y)
{
    if (!mess)
        return;
    int taille=strlen(mess);
   // Serial.println(mess);
    //Serial.println(taille);
    int d=0;
    int dx=0;
    
    for (long int i=0;i<taille;i++)
    {
//Serial.printf("%d ",mess[i]);
  
       if(mess[i]=='\n')
      {
        d++;
        Serial.printf("\n d:%d \n",d);
        dx=i+1;
        if (y-d*8<=-8)
         return;
      }
      else
      {
       if(x+(i-dx)*8 > -8 and x+(i-dx)*8 <LED_WIDTH)
        afficheLettre2(mess[i]-32, x+(i-dx)*8,y-d*8);
        
        //if (x+i*8>= LED_WIDTH)
          //  return;
    }
    }
  
  //Serial.println("fin message");  
}
*/

void displayPicNew(CRGB *pica, int x0, int y0, int h, int w)
{

    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    int r;
    int g;
    int b;

   CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.printf("immossibnle de créer l'image im %d %d\n",wide,height);
        return;
    }
//Serial.println("debut dessin");
    for(int y=0;y<height;y++){
       /* for(int x=0;x<wide;x++){
          
            
        //    r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
         //   g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
          //  b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=pica[(y+offsety)*w+x+offsetx];
            //Serial.printf("%03X ",pica[(y+offsety)*w+x+offsetx]);//I have a program that switches directly from
        }*/
        // memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
        //Serial.println("ici create bitmaap");
        memcpy(bitmapRGB+wide*y,&pica[(y+offsety)*w+offsetx],wide*sizeof(CRGB));
     // Serial.println();
    }
    //putOrientation(bitmapRGB,newx,newy,height,wide);
    // Serial.println("fin dessin");
    putOrientation2(bitmapRGB,height,wide,newx,newy);
   // displaypic(bitmapRGB,newx,newy,height,wide);
    free(bitmapRGB);
    
}
//CRGB bgColor=CRGB(0,0,0);
/*
CRGB lettrefont2[8*9];
void afficheLettre2(int let,int x0,int y0)
{
    memset(lettrefont2,0,8*9*3);
    int taille=8;
    int hauteur=8;
    
    for(int x=x0;x<taille+x0;x++)
    {
        for (int y=y0;y<hauteur+y0;y++)
        {
            if ( ( (1 << (x0-x+taille-1)) &   fontamstrad[let][hauteur-1 - y + y0] ) > 0 )
            {
                lettrefont2[(x-x0)+(y-y0)*taille]=CRGB(255,255,0);//CRGB::Blue;
            }
            else
            {
                lettrefont2[(x-x0)+(y-y0)*taille]=TransColor;
            }
        }
         lettrefont2[(x-x0)+(hauteur)*taille]=TransColor;
    }
    //Serial.printf("pic :%d %d\n",x0,y0);
    displayPicNew(lettrefont2,x0,y0,hauteur+1,taille);
    
}
*/

CRGB  ghost[14][14] = {
  {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1},
  {1, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1},
  {0, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0}
};
CRGB ghostred[296];
CRGB ghostyellow[296];
CRGB ghostgreen[296];
CRGB ghostPurple[296];
CRGB ghostCyan[296];


CRGB fraise[11][10] = {
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 2, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
  {0, 1, 2, 1, 2, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 2, 1, 0},
  {1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 2, 1, 2, 1},
  {1, 2, 1, 2, 1, 3, 1, 1, 1, 1},
  {0, 1, 1, 1, 3, 3, 3, 1, 2, 0},
  {0, 0, 3, 3, 3, 2, 3, 3, 3, 0},
  {0, 0, 0, 0, 0, 2, 0, 0, 0, 0}
};


int YO = 0;

int cerise[196] = {
  100, 100, 100, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 0, 1, 1, 1, 1, 0, 100, 100, 100, 100, 100, 100,
  100, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 100, 100,
  100, 0, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 100,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 1, 0,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0,
  100, 100, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 0, 3, 0, 1, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 3, 0, 100, 0, 1, 1, 1, 1, 0, 100,
  100, 100, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 100, 100,
  100, 0, 0, 3, 0, 3, 3, 0, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

int mario[238]={
   100,3,3,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
   100,3,3,3,2,2,2,2,2,2,100,100,100,100,100,100,100, 
   100,100,3,3,2,2,2,2,2,2,2,2,2,2,3,3,100,
   100,100,100,100,2,2,2,2,4,2,2,2,2,2,3,3,100,
    100,5,5,1,1,1,1,2,2,2,2,4,2,2,3,3,100, 
  5,5,1,1,1,1,1,2,1,1,1,2,100,100,3,3,100, 
    5,5,1,1,1,1,2,1,1,1,2,1,1,100,100,3,100,
  100,100,100,100,100,5,5,5,5,5,5,5,1,1,100,100,100,
  100,100,100,3,3,5,5,5,5,6,6,6,6,1,100,100,100,
  100,100,100,3,5,3,3,5,5,5,6,5,5,5,1,100,100,
  100,100,100,3,5,3,5,5,5,6,5,5,5,1,1,100,100,
  100,100,100,100,3,3,3,5,5,6,5,100,1,1,1,100,100, 
  100,100,100,100,1,1,1,1,1,1,1,1,1,5,5,100,100,
  100,100,100,100,100,1,1,1,1,1,100,100,5,5,5,100,100            
};

CRGB palec[4] = {CRGB::Black, CRGB::Red, CRGB::White, CRGB::Green};
CRGB palpacman[2]={CRGB::Black,CRGB::Yellow};

CRGB palm[7]={CRGB:: Red,CRGB::Red, CRGB::Blue,CRGB(98,65,7),CRGB::Yellow,CRGB(238,151,64),CRGB::Black};
CRGB pall[7]={CRGB:: Red,CRGB::Green, CRGB::Blue,CRGB(98,65,7),CRGB::Yellow,CRGB(238,151,64),CRGB::Black};


int ghostp[72]={
  
  1,9,9,9,1,9,9,9,1,
    1,1,9,1,1,1,9,1,1,
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,
  1,3,2,1,1,3,2,1,1,
  1,2,2,1,1,2,2,1,9,
  9,1,1,1,1,1,1,1,9,
  9,9,1,1,1,1,1,9,9,
};
CRGB ghostpc[72];

CRGB palghost[4] = {CRGB::Black, CRGB::Red, CRGB::White, CRGB::Blue};



void calculghosts(CRGB color)
{
  for (int m = 0; m < 14; m++)
  {
    for (int y = 0; y < 14; y++)
    {
      if (ghost[m][y] == (CRGB)1)
      {
        ghostred[m * 14 + y] = CRGB :: Red;
        ghostyellow[m * 14 + y] = CRGB :: Yellow;
        ghostgreen[m * 14 + y] = CRGB :: Green;
        ghostPurple[m * 14 + y] = CRGB :: Purple;
        ghostCyan[m * 14 + y] = CRGB(64, 222, 255);
      }
      if (ghost[m][y] == (CRGB)2)
      {
        ghostred[m * 14 + y] = CRGB :: Blue;
        ghostyellow[m * 14 + y] = CRGB :: Blue;
        ghostgreen[m * 14 + y] = CRGB :: Blue;
        ghostPurple[m * 14 + y] = CRGB :: Blue;
        ghostCyan[m * 14 + y] = CRGB :: Blue;
      }

      if (ghost[m][y] == (CRGB)3)
      {
        ghostred[m * 14 + y] = CRGB :: White;
        ghostyellow[m * 14 + y] = CRGB :: White;
        ghostgreen[m * 14 + y] = CRGB :: White;
        ghostPurple[m * 14 + y] = CRGB :: White;
        ghostCyan[m * 14 + y] = CRGB :: White;
      }
      if (ghost[m][y] == (CRGB)0)
      {
        ghostred[m * 14 + y] = color;
        ghostyellow[m * 14 + y] = color;
        ghostgreen[m * 14 + y] = color;
        ghostPurple[m * 14 + y] = color;
        ghostCyan[m * 14 + y] = color;
      }

    }
  }
}




CRGB cerisecalc[196];

CRGB mariocalc[238];
CRGB luigicalc[238];
CRGB fraiseCalc[110];
void calculfraise(CRGB color)
{
  for (int m = 0; m < 11; m++)
  {
    for (int y = 0; y < 10; y++)
    {
      if (fraise[m][y] == (CRGB)1)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Red;

      }
      if (fraise[m][y] == (CRGB)2)
      {
        fraiseCalc[m * 10 + y] = CRGB :: White;

      }

      if (fraise[m][y] == (CRGB)3)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Green;

      }
      if (fraise[m][y] == (CRGB)0)
      {
        fraiseCalc[m * 10 + y] = color;

      }

    }
  }
}

void displayPicNewInv(CRGB *pica, int x0, int y0, int h, int w)
{

    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    int r;
    int g;
    int b;

   CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }
//Serial.println("debut dessin");
    for(int y=0;y<height;y++){
        for(int x=0;x<wide;x++){
          
            
        //    r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
         //   g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
          //  b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=pica[(y+offsety)*w+w-1-x-offsetx];
            //Serial.printf("%03X ",pica[(y+offsety)*w+x+offsetx]);//I have a program that switches directly from
        }
        // memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
        //Serial.println("ici create bitmaap");
      // memcpy(bitmapRGB+wide*y,&pica[(y+offsety)*w+offsetx],wide*sizeof(CRGB));
     // Serial.println();
    }
    //putOrientation(bitmapRGB,newx,newy,height,wide);
    // Serial.println("fin dessin");
    putOrientation2(bitmapRGB,height,wide,newx,newy);
   // displaypic(bitmapRGB,newx,newy,height,wide);
    free(bitmapRGB);
    
}


void putOrientation2(CRGB *pic,int h,int w,int x0,int y0)
{
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    if(!Tpic)
    {
        Serial.println("pas possible");
        return ;//NULL;
    }
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
         /*   for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( ((x+x0)+LED_WIDTH*(y+y0)) <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
            }
*/

        for(int y=0;y<h;y++)
            {

     
               memcpy((char *) leds +(LED_WIDTH*(y+y0) +x0)*sizeof(CRGB), (char *) pic +(w*y)*sizeof(CRGB), w*sizeof(CRGB));

       
              
            }
            
            
            
            break;
            
        case DOWN_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0 <NUM_LEDS && LED_HEIGHT-1-x-x0 >=0 && y+y0>=0 && LED_HEIGHT-1-x-x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                    Tpic[LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)) <NUM_LEDS  && (x+x0) >=0 && LED_WIDTH-1-(y+y0)>=0 && (x+x0)<LED_HEIGHT && LED_WIDTH-1-(y+y0) <LED_WIDTH  )
                    Tpic[LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && LED_WIDTH-1-(x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && LED_WIDTH-1-(x+x0) <LED_WIDTH )
                        Tpic[LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
//on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
      // Serial.printf("%d %d %d %d\n",x0,y0,h,w);
            for(int y=0;y<h;y++)
            {

              for (int x=0;x<w;x++)
                {
                   if(pic[w*y+x]!= TransColor)
                        leds[LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                 }

            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(x+x0)+y+y0 <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(x+x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+(y+y0)) <NUM_LEDS  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<LED_HEIGHT && (y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && (x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && (x+x0) <LED_WIDTH )
                        Tpic[(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
            
            
    }
     //return Tpic;
    
}


void calculpic(int *pic, CRGB *palette, CRGB color, int h, int w,CRGB *result)
{
  Serial.println("on est là");
  int to = h * w;
 // static CRGB result[296];
  /*for (int m = 0; m < h; m++)
  {
    for (int y = 0; y < w; y++)
    {
      result[m * w + y] = palette[pic[m * w + y]];
      //Serial.print(palette[pic[m * w + y]]);
      //Serial.printf(":%d\n", pic[m * w + y]);
      if (pic[m * w + y] == 100)
      {
        result[m * w + y] = color;
      }
      if (pic[m * w + y] == 9)
      {
        result[m * w + y] = color;
      }
    }
  }*/

  for(int i=0;i<h*w;i++)
  {
    result[i] = palette[pic[i]];
    if (pic[i] == 100)
      {
        result[i] = color;

      }
      if (pic[i] == 9)
      {
        result[i] = color;

      }
  }
  
 // return result;

}
/*
int syncmax1,syncmax2;

WiFiUDP Udp2;
char *artnetPacket2;
*/

void setup() {
  // put your setup code here, to run once:
     Serial.begin(115200);
/*

int nbNeededUniverses=12;
    if(nbNeededUniverses<=32)
    {
        if(nbNeededUniverses<32)
            syncmax1=(1<<nbNeededUniverses)-1;
        else
            syncmax1=0xFFFFFFFF;
        syncmax2=0;
    }
    else
    {
            syncmax1=0xFFFFFFFF;
        if(nbNeededUniverses-32<32)
            syncmax2=(1<<(nbNeededUniverses-32))-1;
        else
            syncmax2=0xFFFFFFFF;
            //syncmax2=0;

    }
     */

/*
     WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
    Serial.printf("Connecting ");
    WiFi.begin("WiFi-2.4-E19C", "yvesyves");
//WiFi.begin("DomainePutterie", "Jeremyyves");
int retrywifi=0; 
    while (WiFi.status() != WL_CONNECTED && (retrywifi<40)) {
      Serial.println(WiFi.status());
   
      retrywifi++;
        delay(200);
        //Serial.print(".");
    }
    if(WiFi.status() != WL_CONNECTED)
    {
              WiFi.begin("WiFi-2.4-E19C", "yvesyves");
        //WiFi.begin("DomainePutterie", "Jeremyyves");
        retrywifi=0;
            while (WiFi.status() != WL_CONNECTED && (retrywifi<40)) {
              Serial.println(WiFi.status());
           
              retrywifi++;
                delay(200);
                //Serial.print(".");
            }
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    if(WiFi.localIP()==INADDR_NONE)
      {
         //afficheMessage(WiFi.localIP(),0,5);
         delay(2000);
         ESP.restart();
      }
      */
CRGB l[3]={CRGB::Blue,CRGB::Red,CRGB::Green};
uint8_t *d= (uint8_t*)l;
uint8_t f[3]={0,0,255};

uint32_t v = 0x11223344 ;

Serial.printf("ddd %d:%d:%d %d\n",(uint8_t)v,*((uint8_t*)(&v)+1),(uint8_t)v,(uint8_t)v);
Serial.printf("%d:%d:%d %d\n",d[0],d[1],d[2],(uint32_t)*f);
//l=l>>=1;
//Serial.printf("%d:%d:%d\n",l.g,l.r,l.b);
  
 Serial.println("init ready");
 Serial.printf("nb pins %d %d %d\n",1,sizeof(Pins),sizeof(*Pins));
//controller.initled(leds,Pins,NBIS2SERIALPINS,12,27,NUM_STRIPS,NUM_LEDS_PER_STRIP);
 FastLED.addLeds<VIRTUAL_DRIVER,Pins,CLOCK_PIN, LATCH_PIN>(leds,NUM_LEDS_PER_STRIP);
 //FastLED.addLeds<VIRTUAL_DRIVER,Pins,CLOCK_PIN, LATCH_PIN>(leds,NUM_LEDS_PER_STRIP);

//controller.initled(leds,Pins,NUM_STRIPS,NUM_LEDS_PER_STRIP,0); more suitable for ws2811
Serial.printf("nb pins %d %d %d\n",1,sizeof(Pins),sizeof(*Pins));
fill_solid(leds,NUM_LEDS,CRGB::Black);
//fastLED.show(); //controller.showPixels();
//controller.setBrightness(20); //to be used instead of fastled.setbritg hness 
//delay(200);
 
       
        fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
       // fastLED.show(); //controller.showPixels();
        delay(14);
        int k=0;
 for(int pin=0;pin<8;pin++)
   {
      for (int line=0;line<5;line++)
      {
        leds[k%NUM_LEDS_PER_STRIP+NUM_LEDS_PER_STRIP*line+pin*NUM_LEDS_PER_STRIP*5]=CRGB::Blue;
      }
    }
    Serial.printf("ledprtstrip:%d\n",NUM_LEDS_PER_STRIP);
    leds[0]=CRGB::Blue;
    leds[1]=CRGB::Green;
    leds[2]=CRGB::Red;
    leds[3]=CRGB::Blue;
    //fastLED.show(); //controller.showPixels();
    Serial.println("stop");
    //delay(2005);
    leds[0]=CRGB::Blue;
    leds[1]=CRGB::Green;
    leds[2]=CRGB::Red;
    leds[3]=CRGB::Red;
   // fastLED.show(); //controller.showPixels();
    Serial.println("stop");
   // delay(2000);

     CRGB solidColor=CRGB(1,1,1);
  calculghosts(solidColor);
  calculfraise(solidColor);
  calculpic(cerise, palec, solidColor, 14, 14,cerisecalc);
   calculpic(mario, palm, solidColor, 14, 17,mariocalc);
     
  calculpic(mario, pall, bgColor, 14, 17, luigicalc);
bgColor=CRGB::Black;


/*
 Udp2.begin(100);
    // artnetPacket2=(char*)malloc((123*3*3+1)*sizeof(char));
     if(artnetPacket2==NULL)
      {
        Serial.println("impossible to create buffer");
        
      }
      else
      {
        Serial.println("buffer créé");
      }
      
*/
}


  void exchange(int a,int b)
  {
    //exchange 1 et 7
    for(int i=0;i<NUM_LEDS_PER_STRIP;i++)
    {
      CRGB k=leds[a*NUM_LEDS_PER_STRIP+i];
      leds[a*NUM_LEDS_PER_STRIP+i]=leds[b*NUM_LEDS_PER_STRIP+i];
      leds[b*NUM_LEDS_PER_STRIP+i]=k;
      
    }
  }
 
 
int k=0;
 int  dire=1;
  int r=1;
void loop() {
 /*controller.dmaBufferActive=0;
 controller.stopSignal=false;
        controller.startTX();
 */

 fill_solid(leds,NUM_LEDS,CRGB(10,10,10));
 uint16_t i=0;
/*
 for(uint16_t pin=0;pin<8;pin++)
   {
      for (uint16_t line=0;line<5;line++)
      {
        for(uint16_t i=0;i<pin*5+(line+1);i++)
           leds[(uint16_t)((k+i)%256+256*line+pin*256*5)]=CRGB::Blue;
          // Serial.println(line+NUM_LEDS_PER_STRIP*(line+pin*NUM_LEDS_PER_STRIP*5);
       // leds[(k+1)%NUM_LEDS_PER_STRIP+NUM_LEDS_PER_STRIP*line+pin*NUM_LEDS_PER_STRIP*5]=CRGB::Red;
      }
    }*/
    bgColor=CRGB(15,15,15);
fill_solid(leds, NUM_LEDS, bgColor);
char mess[40];
int f=250;
 int offset = LED_WIDTH;
 long     lastHandle3 = __clock_cycles();
 FastLED.setBrightness(64);
//Serial.printf("b:%d\n",k%256);



/*
for(int i=0;i<NUM_STRIPS;i++)
{
uint16_t y=LED_HEIGHT_PER_STRIP*i*LED_WIDTH;
  for(uint16_t l=0;l<LED_HEIGHT_PER_STRIP;l++)
  {
    for(uint16_t u=0;u<LED_WIDTH;u++)
    {
      leds[ y+l*LED_WIDTH+u]=CHSV((255*(l+k/2)/LED_HEIGHT_PER_STRIP)%255,255,90);
   // printf("%d\n",y+l*LED_WIDTH+u);
    }
  }
}
*/


int y=0;
  
     //displayPicDouble(cerisecalc, -k%f+ offset -20,cos_table[(int)(k/2)%LED_WIDTH], 14, 14);
  displayPicNewInv(fraiseCalc, -k%f+ offset + 15 , y, 11, 10);
    displayPicNewInv(ghostred, -k%f + offset + 32, y+10, 14, 14);
    displayPicNewInv(ghostyellow, -k%f + offset +46+3, y+20, 14, 14);
    displayPicNewInv(ghostgreen, -k%f + offset + 62+4, y+30, 14, 14);
    displayPicNewInv(ghostCyan, -k%f + offset +78+5, y+40, 14, 14);
    displayPicNewInv(ghostPurple, -k%f + offset + 94+6, y+50, 14, 14);
displayPicNewInv(mariocalc, -k%f + offset+110+7 , y+55, 14, 17);
   







long     lastHandle = __clock_cycles();
   FastLED.show(); //controller.showPixels();
 long   lasthandle2=__clock_cycles();
  Serial.printf("FPS calcul: %f \n", (float) 240000000L/(lastHandle-lastHandle3));
     Serial.printf("FPS fastled: %f \n", (float) 240000000L/(lasthandle2 - lastHandle));
       Serial.printf("total FPS: %f \n", (float) 240000000L/(lasthandle2 - lastHandle3));
   
    k++;
 delay(10);
 //Serial.printf("%d %d %d %d\n",GRB,RGB,RBG,GRB,BRG);

}
