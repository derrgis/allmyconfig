//
//  PacmanClass.cpp
//  
//
//  Created by Yves BAZIN on 29/12/2019.
//

#include "PacmanClass.h"
#define PACMAN_WIDTH 28
#define PACMAN_HEIGHT 31
#include "barpc.h"



#define DOWN_RIGHT   0 //natural mode no transpostion to be made
#define DOWN_LEFT    1 //We turn 90° clock wise
#define UP_LEFT      2
#define UP_RIGHT     3
#define DOWN_RIGHT_INV   4 //natural mode no transpostion to be made
#define DOWN_LEFT_INV    5 //We turn 90° clock wise
#define UP_LEFT_INV      6
#define UP_RIGHT_INV     7
#define MAX_DISTANCE 99999999L
int tableOrientation=DOWN_RIGHT; //DOWN_RIGHT_INV;
int directionpac;
CRGB pacmancalc[256];
CRGB palpacman[4]={CRGB::Black,CRGB::Yellow,CRGB::White,CRGB::Blue};
CRGB pastilleman[4]={CRGB::Black,CRGB(120,120,120),CRGB::White,CRGB::Blue};
CRGB pacmanafraid[4]={CRGB::Black,CRGB::DarkBlue,CRGB::Black,CRGB::White};
int pastille[16]={
    9,9,9,9,
    9,1,1,9,
    9,1,1,9,
    9,9,9,9
};
int boule[16]={
    9,2,2,9,
    1,2,2,2,
    2,2,2,2,
    9,2,2,9
};

CRGB pastillepic[16];
CRGB boulepic[16];

int pacmanFace;
int pacmanFace2;
int pacmab[256]={
    9,9,1,1,1,1,9,9,
    9,1,1,1,1,1,1,9,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    9,1,1,1,1,1,1,9,
    9,9,1,1,1,1,9,9,
  
    9,9,1,1,1,1,9,9,
    9,1,1,1,1,1,1,9,
    1,1,1,1,1,1,1,1,
    1,1,1,1,9,9,9,9,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    9,1,1,1,1,1,1,9,
    9,9,1,1,1,1,9,9,
    
    9,9,1,1,1,1,9,9,
    9,1,1,1,1,1,1,9,
    1,1,1,1,1,1,9,9,
    1,1,1,1,9,9,9,9,
    1,1,1,1,1,1,9,9,
    1,1,1,1,1,1,1,1,
    9,1,1,1,1,1,1,9,
    9,9,1,1,1,1,9,9,

    
};

int ghost[64]=
{
    1,1,9,1,1,9,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,
    1,3,2,1,1,3,2,1,
    1,2,2,1,1,2,2,1,
    9,1,1,1,1,1,1,9,
    9,9,1,1,1,1,9,9,
};

CRGB blinkmap[4]={CRGB::Black,CRGB::Red,CRGB::White,CRGB::Blue};
CRGB blinkycalc[64];
CRGB clydemap[4]={CRGB::Black,CRGB::Orange,CRGB::White,CRGB::Blue};
CRGB clydecalc[64];
CRGB pinkymap[4]={CRGB::Black,CRGB(160,0,160),CRGB::White,CRGB::Blue};
CRGB pinkycalc[64];
CRGB inkymap[4]={CRGB::Black,CRGB(78,246,231),CRGB::White,CRGB::Blue};
CRGB inkycalc[64];
uint8_t  gamma8[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
uint8_t  gammar[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
int pacboard[28*31]
{
    6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,13,
    1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15, 2,
    1,15, 3, 9, 9, 9, 9, 9, 9, 9, 9,10,15, 3,10,15, 3, 9, 9, 9, 9, 9, 9, 9, 9,10,15, 2,
    1,15, 4, 8, 8, 8, 8,13, 6, 8, 8,11,15, 2, 1,15, 4, 8, 8,13, 6, 8, 8, 8, 8,11,15, 2,
    1,15,15,15,15,15,15, 2, 1,15,15,15,15, 2, 1,15,15,15,15, 2, 1,15,15,15,15,15,15, 2,
    5, 9,10,15, 3,10,15, 2, 1,15, 3, 9, 9,12, 5, 9, 9,10,15, 2, 1,15, 3,10,15, 3, 9,12,
    6, 8,11,15, 2, 1,15, 4,11,15, 4, 8, 8, 8, 8, 8, 8,11,15, 4,11,15, 2, 1,15, 4, 8,13,
    1,17,15,15, 2, 1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15, 2, 1,15,15,17, 2,
    1,15, 3, 9,12, 1,15, 3, 9, 9, 9,10,15, 3,10,15, 3, 9, 9, 9,10,15, 2, 5, 9,10,15, 2,
    1,15, 4, 8, 8,11,15, 4, 8, 8, 8,11,15, 2, 1,15, 4, 8, 8, 8,11,15, 4, 8, 8,11,15, 2,
    1,15,15,15,15,15,15,15,15,15,15,15,15, 2, 1,15,15,15,15,15,15,15,15,15,15,15,15, 2,
    5, 9, 9, 9, 9,10,15, 3,10,16, 3, 9, 9,12, 5, 9, 9,10,16, 3,10,15, 3, 9, 9, 9, 9,12,
   16,16,16,16,16, 1,15, 2, 1,16, 4, 8, 8, 8, 8, 8, 8,11,16, 2, 1,15, 2,16,16,16,16,16,
   16,16,16,16,16, 1,15, 2, 1,16,16,16,16,16,16,16,16,16,16, 2, 1,15, 2,16,16,16,16,16,
   16,16,16,16,16, 1,15, 2, 1,16, 3, 9, 9, 9, 9, 9, 9,10,16, 2, 1,15, 2,16,16,16,16,16,
    8, 8, 8, 8, 8,11,15, 4,11,16, 2,16,16,16,16,16,16, 1,16, 4,11,15, 4, 8, 8, 8, 8, 8,
    8,16,16,16,16,16,15,16,16,16, 2,16,16,16,16,16,16, 1,16,16,16,15,16,16,16,16,16, 8,
    9, 9, 9, 9, 9,10,15, 3,10,16, 2,16,16,16,16,16,16, 1,16, 3,10,15, 3, 9, 9, 9, 9, 9,
   16,16,16,16,16, 1,15, 2, 1,16, 4, 8, 8, 8, 8, 8, 8,11,16, 2, 1,15, 2,16,16,16,16,16,
   16,16,16,16,16, 1,15, 2, 1,16,16,16,16,16,16,16,16,16,16, 2, 1,15, 2,16,16,16,16,16,
   16,16,16,16,16, 1,15, 2, 5, 9, 9,10,16, 3,10,16, 3, 9, 9,12, 1,15, 2,16,16,16,16,16,
    6, 8, 8, 8, 8,11,15, 2, 6, 8, 8,11,16, 2, 1,16, 4, 8, 8,13, 1,15, 4, 8, 8, 8, 8,13,
    1,15,15,15,15,15,15, 2, 1,15,15,15,15, 2, 1,15,15,15,15, 2, 1,15,15,15,15,15,15, 2,
    1,15, 3, 9, 9,10,15, 2, 1,15, 3, 9, 9,12, 5, 9, 9,10,15, 2, 1,15, 3, 9, 9,10,15, 2,
    1,15, 4, 8, 8,11,15, 4,11,15, 4, 8, 8, 8, 8, 8, 8,11,15, 4, 11,15,4, 8, 8,11,15, 2,
    1,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15, 2,
    1,15, 3, 9, 9,10,15, 3, 9, 9, 9,10,15, 3,10,15, 3, 9, 9, 9,10,15, 3, 9, 9,10,15, 2,
    1,17, 2,16,16, 1,15, 2,16,16,16, 1,15, 2, 1,15, 2,16,16,16, 1,15, 2,16,16, 1,17, 2,
    1,15, 4, 8, 8,11,15, 4, 8, 8, 8,11,15, 2, 1,15, 4, 8, 8, 8,11,15, 4, 8, 8,11,15, 2,
    1,15,15,15,15,15,15,15,15,15,15,15,15, 2, 1,15,15,15,15,15,15,15,15,15,15,15,15, 2,
    5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,12, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,12
    
    
    
};


hw_timer_t * timerGhost = NULL;
portMUX_TYPE timerPacmanMux = portMUX_INITIALIZER_UNLOCKED;
CRGB palepac[5]={CRGB::Black,CRGB::Blue,CRGB(255,0,255),CRGB::Yellow};

CRGB *boardpacman;

Pacman::Pacman(CRGB *leds){
    Pacman(123,48,leds);
};
Pacman::Pacman(int width,int height,CRGB *leds)
{
    this->width=width;
    this->height=height;
    this->leds=leds;
    
}

void Pacman::start()
{
    decalDisplay=0; //on repmace l'affichage en bas;
    pacmanFace=0;
    pacmanPosition.x=2;
    pacmanPosition.y=16;
    pacmanPosition.px=pacmanPosition.x*4-2;
    pacmanPosition.py=pacmanPosition.y*4-2;
    directionpac=0;
    pacmanDirection.vx=0;
    pacmanDirection.vy=0;
    resetpossible=false;
    
    targetPacmanPosition.x=pacmanPosition.x;
    targetPacmanPosition.px=pacmanPosition.px;
    targetPacmanPosition.py=pacmanPosition.py;
    targetPacmanPosition.y=pacmanPosition.y;
    
    // targetPacmanPosition
    Blinky.x=14;
    Blinky.y=19;
    Blinky.px=(Blinky.x*4);
    Blinky.py=(Blinky.y*4-2);
    Blinky.path=shortpathBlinky;
    scatterBlinky.x=26;
    scatterBlinky.y=29;
    Blinky.scatter=&scatterBlinky;
    Blinky.direction=LEFT;
    //Blinky.scatterghostx=26;
    //Blinky.scatterghosty=31;
    blinkyDirection.vx=0;
    blinkyDirection.vy=0;
    Clyde.x=11;
    Clyde.y=16;
    Clyde.inbox=true;
    scatterClyde.x=0;
    scatterClyde.y=1;
    Clyde.scatter=&scatterClyde;
    Clyde.px=(Clyde.x*4);
    Clyde.py=(Clyde.y*4-2);
    Clyde.path=shortpathClyde;
    
    Pinky.x=13;
    Pinky.y=16;
    //Pinky.scatterghostx=26;
    //Pinky.scatterghosty=0;
    scatterPinky.x=1;
    scatterPinky.y=29;
    Pinky.scatter=&scatterPinky;
    Pinky.direction=UP;
    Pinky.px=(Pinky.x*4);
    Pinky.py=(Pinky.y*4-2);
    Pinky.inbox=true;
    Pinky.path=shortpathPinky;

    Inky.x=15;
    Inky.y=16;
    Inky.px=(Inky.x*4);
    Inky.py=(Inky.y*4-2);
    scatterInky.x=26;
    scatterInky.y=-1;
    Inky.scatter=&scatterInky;
    Inky.inbox=true;
    Inky.path=shortpathInky;
    
    isPacmanMoving=false;
    //calculpath(&Blinky,targetPacmanPosition);
     //calculpath(&Clyde,targetPacmanPosition);
    //calculpath(&Pinky,targetPacmanPosition);
    //calculpath(&Inky,targetPacmanPosition);
    inPacman=true;
    //fill(CRGB(0,0,0));
    pacmanInterval=90000;
    gohstInterval=40000;
    startime=millis();
    
    
    xTaskCreatePinnedToCore(Pacman::_PACTask, "_PACTask", 1024, this,1, &_GhostTaskHandle,1);
    calculpic(pacmab, palpacman, CRGB(0,0,0),32,8,pacmancalc);
    calculpic(ghost, blinkmap, CRGB(0,0,0),8,8,blinkycalc);
    calculpic(ghost, clydemap, CRGB(0,0,0),8,8,clydecalc);
    calculpic(ghost, pinkymap, CRGB(0,0,0),8,8,pinkycalc);
    calculpic(ghost, inkymap, CRGB(0,0,0),8,8,inkycalc);
    calculpic(pastille, pastilleman, CRGB(0,0,0),4,4,pastillepic);
      calculpic(boule, pastilleman, CRGB(0,0,0),4,4,boulepic);
    timerGhost = timerBegin(1, 80, true);
    timerAttachInterrupt(timerGhost, &moveGhosts, true);
    timerAlarmWrite(timerGhost, gohstInterval, true);
    timerAlarmEnable(timerGhost);
}

void Pacman::displayPacman()
{
    
    
    if(isafraid)
    {
        if((float)(ESP.getCycleCount()-timewhite)/240000000L>10)
        {
            isafraid=false;
            timewhite=0;
            calculpic(ghost, blinkmap, CRGB(0,0,0),8,8,blinkycalc);
            calculpic(ghost, clydemap, CRGB(0,0,0),8,8,clydecalc);
            calculpic(ghost, pinkymap, CRGB(0,0,0),8,8,pinkycalc);
            calculpic(ghost, inkymap, CRGB(0,0,0),8,8,inkycalc);
        }
    }
    pacmanFace2=(pacmanFace2+1)%17000;
    pacmanFace=((pacmanFace2)/10)%3;
    
    if(pacmanPosition.py>this->height/2)
    {
        decalDisplay=this->height/2-pacmanPosition.py;
        
        virtualPacmanY=this->height/2;
           if(-decalDisplay>PACMAN_HEIGHT*4-this->height)
            {
                decalDisplay=-PACMAN_HEIGHT*4+this->height;
                virtualPacmanY=decalDisplay+pacmanPosition.py;
            }
        
    }
    else
    {
        decalDisplay=0;
        virtualPacmanY=pacmanPosition.py;
    }
   // Serial.println(decalDisplay);
    displayBitmapFromProgmem(pacboardfull,1,decalDisplay,31*4,112);
    for(int i=0;i<PACMAN_WIDTH;i++)
        for(int j=0;j<PACMAN_HEIGHT;j++)
        {
            if(pacboard[i+j*PACMAN_WIDTH]==15)
                displayPicNew(pastillepic,i*4+1,j*4+decalDisplay,4,4);
            if(pacboard[i+j*PACMAN_WIDTH]==17)
                displayPicNew(boulepic,i*4+1,j*4+decalDisplay,4,4);
        }
    
  displaygifdir(pacmancalc,pacmanPosition.px+1,virtualPacmanY,8,8,pacmanFace,directionpac);
    displayPicNew(blinkycalc,Blinky.px+1,Blinky.py+decalDisplay,8,8);
    displayPicNew(clydecalc,Clyde.px+1,Clyde.py+decalDisplay,8,8);
    displayPicNew(pinkycalc,Pinky.px+1,Pinky.py+decalDisplay,8,8);
    displayPicNew(inkycalc,Inky.px+1,Inky.py+decalDisplay,8,8);
    //Serial.println(Blinky.py+decalDisplay);
    //replaceled();
}





void Pacman::putOrientation2(CRGB *pic,int h,int w,int x0,int y0)
{
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    /*if(!Tpic)
     {
     Serial.println("pas possible");
     return ;//NULL;
     }*/
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
            /*   for(int y=0;y<h;y++)
             {
             for (int x=0;x<w;x++)
             {
             if( ((x+x0)+this->width*(y+y0)) <NUM_this->leds && x+x0 >=0 && y+y0>=0 && x+x0<this->width && y+y0 <this->height)
             Tpic[x+x0+this->width*(y+y0)]=pic[w*y+x];
             }
             }
             */
            
            for(int y=0;y<h;y++)
            {
                
                // if(pic[w*y+x]!=bgColor)
                //{
                if((y+y0)%2==0)
                {
                    /* for (int x=0;x<w;x++)
                     {
                     // if( this->width-1-x-x0+this->width*(y+y0) <NUM_this->leds && this->width-1-x-x0 >=0 && y+y0>=0 && this->width-1-x-x0<this->width && y+y0 <this->height)
                     //this->leds[this->width-1-x-x0+this->width*(y+y0)]=pic[w*y+x];
                     //  if(pic[w*y+x]!=bgColor)
                     this->leds[x+x0+this->width*(y+y0)]=pic[w*y+x];
                     }*/
                    memcpy((char *) this->leds +(this->width*(y+y0) +x0)*sizeof(CRGB), (char *) pic +(w*y)*sizeof(CRGB), w*sizeof(CRGB));
                }
                else
                {
                    for (int x=0;x<w;x++)
                    {
                        // if( this->width-1-x-x0+this->width*(y+y0) <NUM_this->leds && this->width-1-x-x0 >=0 && y+y0>=0 && this->width-1-x-x0<this->width && y+y0 <this->height)
                        //this->leds[(this->width <<1)* ( (y>>1) + 1) - (1 + x)]
                        // this->leds[this->width-1-x-x0+ this->width*(y+y0)]=pic[w*y+x];
                        //if(pic[w*y+x]!=bgColor)
                        this->leds[ (this->width<<1)*( ((y+y0)>>1)+1)     -( 1+x+x0)]   =pic[w*y+x];
                        
                    }
                    //  memcpy((char *) this->leds +(  (this->width<<1) * ( ((y+y0)>>1)  + 1) - (1+x0+w-1) )*sizeof(CRGB), (char *) pic +(w*y)*sizeof(CRGB), w*sizeof(CRGB));
                }
                //  }
                
            }
            
            
            
            break;
     /*
        case DOWN_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( this->width*(this->height-1-x-x0)+y+y0 <NUM_this->leds && this->height-1-x-x0 >=0 && y+y0>=0 && this->height-1-x-x0<this->height && y+y0 <this->width)
                        Tpic[this->width*(this->height-1-x-x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (this->width*((x+x0))+this->width-1-(y+y0)) <NUM_this->leds  && (x+x0) >=0 && this->width-1-(y+y0)>=0 && (x+x0)<this->height && this->width-1-(y+y0) <this->width  )
                        Tpic[this->width*((x+x0))+this->width-1-(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( this->width-1-(x+x0)+this->width*(this->height-1-(y+y0)) <NUM_this->leds  && this->width-1-(x+x0)>=0 && this->width*(this->height-1-(y+y0))>=0 && (this->height-1-(y+y0))<this->height && this->width-1-(x+x0) <this->width )
                        Tpic[this->width-1-(x+x0)+this->width*(this->height-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
            //on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
            // Serial.printf("%d %d %d %d\n",x0,y0,h,w);
            for(int y=0;y<h;y++)
            {
                
                if((y+y0)%2==0)
                {
                    for (int x=0;x<w;x++)
                    {
                        // if( this->width-1-x-x0+this->width*(y+y0) <NUM_this->leds && this->width-1-x-x0 >=0 && y+y0>=0 && this->width-1-x-x0<this->width && y+y0 <this->height)
                        this->leds[this->width-1-x-x0+this->width*(y+y0)]=pic[w*y+x];
                        // this->leds[x+x0+this->width*(y+y0)]=pic[w*y+x];
                    }
                }
                else
                {
                    for (int x=0;x<w;x++)
                    {
                        // if( this->width-1-x-x0+this->width*(y+y0) <NUM_this->leds && this->width-1-x-x0 >=0 && y+y0>=0 && this->width-1-x-x0<this->width && y+y0 <this->height)
                        //this->leds[(this->width <<1)* ( (y>>1) + 1) - (1 + x)]
                        // this->leds[this->width-1-x-x0+ this->width*(y+y0)]=pic[w*y+x];
                        this->leds[ (this->width<<1)*( ((y+y0)>>1)+1)     -( 1+this->width-1-x-x0)]   =pic[w*y+x];
                    }
                }
            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( this->width*(x+x0)+y+y0 <NUM_this->leds && x+x0 >=0 && y+y0>=0 && x+x0<this->height && y+y0 <this->width)
                        Tpic[this->width*(x+x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (this->width*((x+x0))+(y+y0)) <NUM_this->leds  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<this->height && (y+y0) <this->width  )
                        Tpic[this->width*((x+x0))+(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (x+x0)+this->width*(this->height-1-(y+y0)) <NUM_this->leds  && (x+x0)>=0 && this->width*(this->height-1-(y+y0))>=0 && (this->height-1-(y+y0))<this->height && (x+x0) <this->width )
                        Tpic[(x+x0)+this->width*(this->height-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            */
            
            
    }
    //return Tpic;
    
}




void Pacman::displayBitmapFromProgmem(const unsigned char *pic,int x0,int y0,int h,int w )
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
        max_wide=this->width;
    else
        max_wide=this->height;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<=max_wide)
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
        max_height=this->height;
    else
        max_height=this->width;
    
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
    
    for(int y=0;y<height;y++){
        for(int x=0;x<wide;x++){
            
            r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
            g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
            b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=CRGB(r,g,b); //I have a program that switches directly from
        }
    }
    //displayPicNew(bitmapRGB,newx,newy,height,wide);
    
    putOrientation2(bitmapRGB,height,wide,newx,newy);
    
    free(bitmapRGB);
    
}

void Pacman::displayPicNew(CRGB *pica, int x0, int y0, int h, int w)
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
        max_wide=this->width;
    else
        max_wide=this->height;
    
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
        max_height=this->height;
    else
        max_height=this->width;
    
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
        // memcpy(&this->leds[this->width * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
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




void Pacman::displayPicNewInv(CRGB *pica, int x0, int y0, int h, int w)
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
        max_wide=this->width;
    else
        max_wide=this->height;
    
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
        max_height=this->height;
    else
        max_height=this->width;
    
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
        // memcpy(&this->leds[this->width * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
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




void Pacman::displaygif( CRGB *pica, long int x0, long int y0, long int h, long int w,long pic)
{
    
    CRGB *bitmapRGB =(CRGB*)malloc(w*h*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }
    
    memcpy ( bitmapRGB, pica+(h*w*pic), h*w*sizeof(CRGB) );
    displayPicNew( bitmapRGB, x0, y0,  h,  w);
    
    free(bitmapRGB);
}


void Pacman::displaygifdir( CRGB *pica, long int x0, long int y0, long int h, long int w,long  pic,int direc)
{
    
    CRGB *bitmapRGB =(CRGB*)malloc(w*h*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }
    
    memcpy ( bitmapRGB, pica+(h*w*pic), h*w*sizeof(CRGB) );
    //Serial.println((h*w*pic)*sizeof(CRGB));
    switch(direc){
            
        case LEFT:
            for(int i=0 ;i<h;i++)
            {
                for(int j=0;j<int(w/2);j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[i*w+(w-1-j)];
                    bitmapRGB[i*w+(w-j-1)]=dd;
                }
                
            }
            
            break;
            
            
        case DOWN:
            
            //Symetrie diagoonale
            for(int i=0 ;i<=h-1;i++)
            {
                for(int j=i+1;j<w;j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[j*w+i];
                    bitmapRGB[j*w+i]=dd;
                }
                
            }
            
            //sympetrie horizonatle
            
            for(int i=0 ;i<int(h/2);i++)
            {
                for(int j=0;j<w;j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[(h-1-i)*w+j];
                    bitmapRGB[(h-1-i)*w+j]=dd;
                }
                
            }
            
            break;
            
        case UP:
            
            for(int i=0 ;i<h;i++)
            {
                for(int j=0;j<int(w/2);j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[i*w+(w-1-j)];
                    bitmapRGB[i*w+(w-j-1)]=dd;
                }
                
            }
            
            
            //Symetrie diagoonale
            for(int i=0 ;i<=h-1;i++)
            {
                for(int j=i+1;j<w;j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[j*w+i];
                    bitmapRGB[j*w+i]=dd;
                }
                
            }
            
            //sympetrie horizonatle
            
            for(int i=0 ;i<int(h/2);i++)
            {
                for(int j=0;j<w;j++)
                {
                    CRGB dd=bitmapRGB[i*w+j];
                    bitmapRGB[i*w+j]=bitmapRGB[(h-1-i)*w+j];
                    bitmapRGB[(h-1-i)*w+j]=dd;
                }
                
            }
            
            
            
            
            
            break;
            
            
            
    }
    
    displayPicNew( bitmapRGB, x0, y0,  h,  w);
    
    free(bitmapRGB);
}




void Pacman::calculpic(int *pic, CRGB *palette, CRGB color, int h, int w,CRGB *result)
{
    //Serial.println("on est là");
    int to = h * w;
    
    
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
        if (pic[i] == 0)
        {
            result[i] = color;
            
        }
    }
    
    // return result;
    
}


nodedir Pacman::nextFree(nodedir nd)
{
    node pos=nd.d;
    int direction=nd.direction;
    node temp=pos;
    switch (direction)
    {
        case LEFT:
            temp.x-=4;
            nd.d=temp;
            return nd;
            temp.x--;
            if(isFreeTile(temp))
            {
                nd.d=temp;
                return nd;
            }
            else
            {
                temp=pos;
                temp.y++;
                if(isFreeTile(temp))
                {
                    nd.direction=4;
                    nd.d=temp;
                    return nd;
                }
                else
                {
                    temp=pos;
                    temp.y--;
                    if(isFreeTile(temp))
                    {
                        nd.direction=2;
                        nd.d=temp;
                        return nd;
                    }
                    else
                    {
                        return nd;
                    }
                    
                }
            }
        break;
        case DOWN:
            temp.y-=4;
            nd.d=temp;
            return nd;
            temp.y--;
            if(isFreeTile(temp))
            {
                nd.d=temp;
                return nd;
            }
            else
            {
                temp=pos;
                temp.x++;
                if(isFreeTile(temp))
                {
                     nd.direction=3;
                    nd.d=temp;
                    return nd;
                }
                else
                {
                    temp=pos;
                    temp.x--;
                    if(isFreeTile(temp))
                    {
                        nd.direction=1;
                        nd.d=temp;
                        return nd;
                        
                    }
                    else
                        return nd;
                    
                }
            }
        break;
        case RIGHT:
            temp.x+=4;
            nd.d=temp;
            return nd;
            temp.x++;
            if(isFreeTile(temp))
            {
                nd.d=temp;
                return nd;
            }
            else
            {
                temp=pos;
                temp.y++;
                if(isFreeTile(temp))
                {
                    nd.direction=4;
                    nd.d=temp;
                    return nd;
                }
                else
                {
                    temp=pos;
                    temp.y--;
                    if(isFreeTile(temp))
                    {
                        nd.direction=2;
                        nd.d=temp;
                        return nd;
                       
                    }
                    else
                        return nd;
                    
                }
            }
        break;
        case UP:
            temp.y+=4;
            nd.d=temp;
            return nd;
            temp.y++;
            if(isFreeTile(temp))
            {
                nd.d=temp;
                return nd;
            }
            else
            {
                temp=pos;
                temp.x++;
                if(isFreeTile(temp))
                {
                    nd.direction=3;
                    nd.d=temp;
                    return nd;
                }
                else
                {
                    temp=pos;
                    temp.x--;
                    if(isFreeTile(temp))
                    {
                        nd.direction=1;
                        nd.d=temp;
                        return nd;
                    }
                    else
                        return nd;
                    
                }
            }
        break;
    
    }
    
}



bool  Pacman::canMoveup()
{
    node d;
    //if(directionpa)
    if((pacmanPosition.px+2)%4>0)
        return false;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x;
    d.y=pacmanPosition.y+1;
    return isFreeTile(d) ;
}


bool  Pacman::canMovedown()
{
    node d;
    if((pacmanPosition.px+2)%4>0)
        return false;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x;
    d.y=pacmanPosition.y-1;
    return isFreeTile(d) ;
}

bool  Pacman::canMoveleft()
{
    node d;
    if((pacmanPosition.py+2)%4>0)
        return false;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x-1;
    d.y=pacmanPosition.y;
    return isFreeTile(d) ;
}

bool  Pacman::canMoveright()
{
    node d;
    if((pacmanPosition.py+2)%4>0)
        return false;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x+1;
    d.y=pacmanPosition.y;
    return isFreeTile(d) ;
}

void Pacman::checkpastille()
{
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    if(pacboard[pacmanPosition.x+pacmanPosition.y*PACMAN_WIDTH]==15)
    {
        pacboard[pacmanPosition.x+pacmanPosition.y*PACMAN_WIDTH]=16;
    }
    if(pacboard[pacmanPosition.x+pacmanPosition.y*PACMAN_WIDTH]==17)
    {
        pacboard[pacmanPosition.x+pacmanPosition.y*PACMAN_WIDTH]=16;
        calculpic(ghost, pacmanafraid, CRGB(0,0,0),8,8,blinkycalc);
        calculpic(ghost, pacmanafraid, CRGB(0,0,0),8,8,clydecalc);
        calculpic(ghost, pacmanafraid, CRGB(0,0,0),8,8,pinkycalc);
        calculpic(ghost, pacmanafraid, CRGB(0,0,0),8,8,inkycalc);
        timewhite=ESP.getCycleCount();
        isafraid=true;
    }
}



void Pacman::ghostMovement()
{
    calculpath(&Blinky,targetPacmanPosition);
   calculpath(&Clyde,targetPacmanPosition);
    calculpath(&Inky,targetPacmanPosition);
    nodedir nd;
    nd.d=targetPacmanPosition;
    nd.direction=directionpac;
    nodedir d=nextFree(nd);
    d=nextFree(d);
    d=nextFree(d);
     d=nextFree(d);
    calculpath(&Pinky,d.d);
    //pacboard[d.d.x+d.d.y*PACMAN_WIDTH]=18;
}

void Pacman::moveleft()
{
      node d;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
     d.x=pacmanPosition.x-1;
     d.y=pacmanPosition.y;
    directionpac=LEFT;
    //Serial.printf("position %d %d\n",d.x,d.y);
    if((pacmanPosition.px+2)%4>0)
    {
         pacmanPosition.px-=1;
        // directionpac=LEFT;
    }

    else
    {
        if(d.x==0 && d.y==16)
        {
            pacmanPosition.y=16;
            pacmanPosition.x=24;
            pacmanPosition.px=pacmanPosition.x*4-2;
            pacmanPosition.py=pacmanPosition.y*4-2;
            //targetPacmanPosition.x=pacmanPosition.x-1;
            //ghostMovement();
            
        }
        
        else
        {
            if(isFreeTile(d))
            {
                // Serial.println("LEFT");
                pacmanPosition.px-=1;
                //pacmanDirection.vy=0;
                //targetPacmanPosition.x=pacmanPosition.x-1;
                //directionpac=LEFT;
                //emptyClosedList();
                //ghostMovement();
                //calculpath(Blinky,targetPacmanPosition);
            }
        }
    }
    checkpastille();
 }





void Pacman::moveup(){
     node d;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x;
    d.y=pacmanPosition.y+1;
    directionpac=UP;
    if((pacmanPosition.py+2)%4>0)
    {
        pacmanPosition.py+=1;
    }
    else if(isFreeTile(d))
    {
       // pacmanDirection.vx=0;
        pacmanPosition.py+=1;
       targetPacmanPosition.y=pacmanPosition.y+1;
        //directionpac=4;
      // emptyClosedList();
       // ghostMovement();
        //calculpath(Blinky,targetPacmanPosition);
    }
    checkpastille();
}


void Pacman::movedown(){
     node d;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x;
    d.y=pacmanPosition.y-1;
    directionpac=DOWN;
    if((pacmanPosition.py+2)%4>0)
    {
        pacmanPosition.py-=1;
        //directionpac=2;
    }
   else if(isFreeTile(d))
    {
        //pacmanDirection.vx=0;
        pacmanPosition.py-=1;
        targetPacmanPosition.y=pacmanPosition.y-1;
        //directionpac=2;
       // ghostMovement();
        //calculpath(Blinky,targetPacmanPosition);
    }
    checkpastille();
    
}
void Pacman::moveright(){
     node d;
    pacmanPosition.x=(pacmanPosition.px+2)/4;
    pacmanPosition.y=(pacmanPosition.py+2)/4;
    d.x=pacmanPosition.x+1;
    d.y=pacmanPosition.y;
    directionpac=RIGHT;
    //Serial.printf("position %d %d\n",pacmanPosition.px,d.y);
    if((pacmanPosition.px+2)%4>0)
    {
        pacmanPosition.px+=1;
        //directionpac=3;
    }
   else
   {
       if(d.x==27 && d.y==16)
       {
           pacmanPosition.y=16;
           pacmanPosition.x=1;
           pacmanPosition.px=pacmanPosition.x*4-2;
           pacmanPosition.py=pacmanPosition.y*4-2;
           targetPacmanPosition.x=pacmanPosition.x+1;
           //ghostMovement();
           
       }
       
       else
       {
           if(isFreeTile(d))
           {
               //Serial.println("on va a droite");
               pacmanPosition.px+=1;
               //pacmanDirection.vy=0;
               //if(pacmanPosition=PACMAN_WIDTH-1)
               //   targetPacmanPosition.x=0;
               // else
               targetPacmanPosition.x=pacmanPosition.x+1;
               //directionpac=3;
               emptyClosedList();
               //ghostMovement();
               //calculpath(Blinky,targetPacmanPosition);
           }
       }
   }
    
    checkpastille();
}


int Pacman::getTile(int x,int y)
{
    return pacboard[x+y*PACMAN_WIDTH];
}
 bool  Pacman::isFreeTile(struct node n)
{
    //Serial.printf("%d %d %d\n",n.x,n.y,pacboard[n.x+n.y*PACMAN_WIDTH]);
    if(pacboard[n.x+n.y*PACMAN_WIDTH]>=15)
        return true;
    else
        return false;
}



void Pacman::emptyOpenList()
{
    resetpossible=false;
    while(incalcul){}
    openListSize=0;
    resetpossible=true;
}


void Pacman::emptyClosedList()
{
    resetpossible=false;
    while(incalcul){}
    closedListSize=0;
    resetpossible=true;
}
void Pacman::addInOpenList(node n)
{
    openList[openListSize]=n;
    openListSize++;
}

void Pacman::addInClosedList(node n)
{
    closedList[closedListSize]=n;
    closedListSize++;
}

bool Pacman::isInOpenList(node n)
{
    
    for(int i=0;i<openListSize;i++)
    {
        node p=openList[i];
        if (n.x==p.x && n.y==p.y)
            return true;
    }
    return false;
}


bool Pacman::isInShortPath(node target,node n)
{
    
    for(int i=0;i<target.followupPath+1;i++)
    {
        node p=target.path[i];
        if (n.x==p.x && n.y==p.y)
            return true;
    }
    return false;
}

bool Pacman::isInClosedList(node n)
{
    
    for(int i=0;i<closedListSize;i++)
    {
        node p=closedList[i];
        if (n.x==p.x && n.y==p.y)
            return true;
    }
    return false;
}

node Pacman::getParent(node n)
{
    node r;
    r.x=0;
    r.y=0;
    for(int i=0;i<closedListSize;i++)
    {
        node p=closedList[i];
        if (n.pax==p.x && n.pay==p.y)
            return p;
    }
    return  r;
}

int Pacman::getNode(node n)
{
    node r;
    r.x=0;
    r.y=0;
    for(int i=0;i<openListSize;i++)
    {
        node p=openList[i];
        if (n.x==p.x && n.y==p.y)
            return i;
    }
    return  0;
}


node Pacman::newMinimumNode()
{
    long score=9999999999;
    int minnode=0;
    for(int i=0;i<openListSize;i++)
    {
        node p=openList[i];
        if(score>p.cost+p.pathlength)
        {
            minnode=i;
            score=p.cost+p.pathlength;
        }
        
    }
    // Serial.printf("found in %d\n",minnode);
    return openList[minnode];
}

void Pacman::calculpath(node *start, node target)
{
    resetpossible=false; //do not touch the gohjst
    emptyClosedList();
    emptyOpenList();
    node n;
    n.x=start->x;
    n.y=start->y;
    n.pax=0;
    n.pay=0;
    n.pathlength=0;
    // addInOpenList(n);
    addInClosedList(n);
    while(!isInClosedList(target))
    {
        //on ajoute les voisins
        //Serial.printf("ici %d %d\n",n.x,n.y);
        
        node n1;
        n1.x=n.x+1;
        n1.y=n.y;
        n1.pax=n.x;
        n1.pay=n.y;
       // Serial.printf("right %d\n",pacboard[n1.x+n1.y*PACMAN_WIDTH]);
        if(!isInClosedList(n1) &&  pacboard[n1.x+n1.y*PACMAN_WIDTH]>=15 )
        {
            //Serial.println("add right");
            n1.cost=calculCost(n1);  //distance from pacman
            n1.pathlength=getParent(n1).pathlength+1;
            
            if(isInOpenList(n1))
            {
                //Serial.println("on remplace");
                int g=getNode(n1);
                if(openList[g].cost+openList[g].pathlength>n1.cost+n1.pathlength)
                    openList[g]=n1;
            }
            else
                addInOpenList(n1);
        }
        
        n1.x=n.x-1;
        n1.y=n.y;
        n1.pax=n.x;
        n1.pay=n.y;
        
        //Serial.printf("left %d\n",pacboard[n1.x+n1.y*PACMAN_WIDTH]);
        if(!isInClosedList(n1) &&  pacboard[n1.x+n1.y*PACMAN_WIDTH]>=15)
        {
            //Serial.println("add left");
            n1.cost=calculCost(n1);  //distance from pacman
            n1.pathlength=getParent(n1).pathlength+1;
            
            if(isInOpenList(n1))
            {
//Serial.println("on remplace");
                int g=getNode(n1);
                if(openList[g].cost+openList[g].pathlength>n1.cost+n1.pathlength)
                    openList[g]=n1;
            }
            else
                addInOpenList(n1);
        }
        
        n1.x=n.x;
        n1.y=n.y-1;
        n1.pax=n.x;
        n1.pay=n.y;
       // Serial.printf("down %d\n",pacboard[n1.x+n1.y*PACMAN_WIDTH]);
        if(!isInClosedList(n1) &&  pacboard[n1.x+n1.y*PACMAN_WIDTH]>=15)
        {
             //Serial.println("add down");
            n1.cost=calculCost(n1);  //distance from pacman
            n1.pathlength=getParent(n1).pathlength+1;
            
            if(isInOpenList(n1))
            {
                // Serial.println("on remplace");
                int g=getNode(n1);
                if(openList[g].cost+openList[g].pathlength>n1.cost+n1.pathlength)
                    openList[g]=n1;
            }
            else
                addInOpenList(n1);
        }
        
        n1.x=n.x;
        n1.y=n.y+1;
        n1.pax=n.x;
        n1.pay=n.y;
        //Serial.printf("up %d\n",pacboard[n1.x+n1.y*PACMAN_WIDTH]);
        if(!isInClosedList(n1) &&  pacboard[n1.x+n1.y*PACMAN_WIDTH]>=15)
        {
            //Serial.println("add up");
            n1.cost=calculCost(n1);  //distance from pacman
            n1.pathlength=getParent(n1).pathlength+1;
            
            if(isInOpenList(n1))
            {
                //  Serial.println("on remplace");
                int g=getNode(n1);
                if(openList[g].cost+openList[g].pathlength>n1.cost+n1.pathlength)
                    openList[g]=n1;
            }
            else
                addInOpenList(n1);
        }
        
        
        n=newMinimumNode();
        //Serial.printf("minium node %d %d out of %d\n",n.x,n.y,openListSize);
        node k;
        k.x=255;
        k.y=255;
        k.cost=9999999999;
        k.pathlength=999999999999;
        //Serial.printf("node %d\n",getNode(n));
        openList[getNode(n)]=k;
        addInClosedList(n);
        
    }
   // Serial.printf("node %d\n",getNode(target));
    node p;
    int i=0;
    start->shortpathlength=0;
    while(n.x!=start->x or n.y!=start->y)
    {
        start->path[start->shortpathlength]=n;
        n=getParent(n);
        start->shortpathlength++;
    }
    
    
    //Serial.printf("done %d\n",start->shortpathlength);
    start->followupPath=start->shortpathlength-1;
    //displayarray();
    resetpossible=true;
    
}


void Pacman::ghostDirection( struct node n)
{
    long costmoin=999999;
    struct node voisin;
    struct directionst fin;
    // int finvx,finvy;
    value=0;
    finvx=0;
    finvy=0;
    //we get the cell around it
    //above
    
    incalcul=true;
    
    
    
    if( pacboard[n.x+(n.y+1)*PACMAN_WIDTH]>=15 ) //on peut se déplacer sur la cellule
    {
        voisin.x=n.x;
        voisin.y=n.y+1;
        if(!isInClosedList(voisin))
            if(costmoin>=calculCost(voisin))
            {
                //voisin.x=n.x;
                //voisin.y=n.y;
                //  finalDirection.vx=0;
                //finalDirection.vy=1;
                finvx=0;
                finvy=1;
                value=1;
                costmoin=calculCost(voisin);
                
            }
    }
    
    if( pacboard[n.x+(n.y-1)*PACMAN_WIDTH]>=15 ) //on peut se déplacer sur la cellule
    {
        voisin.x=n.x;
        voisin.y=n.y-1;
        if(!isInClosedList(voisin))
            if(costmoin>=calculCost(voisin))
            {
                finvx=0;
                finvy=-1;
                value=2;
                costmoin=calculCost(voisin);
            }
        
    }
    
    
    
    
    byte newx=0;
    if (n.x==27)
        newx=0;
    else
        newx=n.x+1;
    if( pacboard[newx+n.y*PACMAN_WIDTH]>=15 ) //on peut se déplacer sur la cellule
    {
        
        voisin.x=newx;
        
        voisin.y=n.y;
        if(!isInClosedList(voisin))
            if(costmoin>=calculCost(voisin))
            {
                finvx=1;
                finvy=0;
                value=3;
                costmoin=calculCost(voisin);
                
            }
        
    }
    byte newx2=0;
    if (n.x==0)
        newx2=27;
    else
        newx2=n.x-1;
    if( pacboard[newx2+n.y*PACMAN_WIDTH]>=15 ) //on peut se déplacer sur la cellule
    {
        
        voisin.x=newx2;
        
        voisin.y=n.y;
        if(!isInClosedList(voisin))
            if(costmoin>=calculCost(voisin))
            {
                finvx=-1;
                finvy=0;
                value=4;
                costmoin=calculCost(voisin);
            }
        
    }
    //return value;
    //finalDirection.vx=finvx;
    //finalDirection.vy=finvy;
    incalcul=false;
    switch(value)
    {
        case 1:
            voisin.x=n.x;
            voisin.y=n.y+1;
            addInClosedList(voisin);
            break;
            
        case 2:
            voisin.x=n.x;
            voisin.y=n.y-1;
            addInClosedList(voisin);
            break;
            
        case 3:
            voisin.x=newx;
            voisin.y=n.y;
            addInClosedList(voisin);
            break;
            
        case 4:
            voisin.x=newx2;
            voisin.y=n.y;
            addInClosedList(voisin);
            break;
            
            
    }
    
    
}

long  Pacman::calculCost(struct node n)
{
    return  abs(targetPacmanPosition.x-n.x)+abs(targetPacmanPosition.y-n.y); //(targetPacmanPosition.x-n.x)*(targetPacmanPosition.x-n.x)+(targetPacmanPosition.y-n.y)*(targetPacmanPosition.y-n.y);
}

long  Pacman::calculCostTwoPoints(node n,node n1)
{
    
    /*if(n.y<=0 || n1.y<=0 || n.x<=0 || n1.x <=0)
    {
        return  abs((int)((int)n1.x-(int)n.x))+abs((int)((int)n1.y-(int)n.y));
    }*/
    if(isFreeTile(n1))
    //    return  (n1.x-n.x)*(n1.x-n.x)+(n1.y-n.y)*(n1.y-n.y);
    return  abs((int)((int)n1.x-(int)n.x))+abs((int)((int)n1.y-(int)n.y));
    else return MAX_DISTANCE;
}

long  Pacman::calculCostTwoPointsabs(node n,node n1)
{
    
    /*if(n.y<=0 || n1.y<=0 || n.x<=0 || n1.x <=0)
     {
     return  abs((int)((int)n1.x-(int)n.x))+abs((int)((int)n1.y-(int)n.y));
     }*/
    if(isFreeTile(n1))
        return  abs((int)((int)n1.x-(int)n.x))+abs((int)((int)n1.y-(int)n.y)); //(targetPacmanPosition.x-n.x)*(targetPacmanPosition.x-n.x)+(targetPacmanPosition.y-n.y)*(targetPacmanPosition.y-n.y);
    else return MAX_DISTANCE;
}



void IRAM_ATTR Pacman::moveGhosts()
{
   //Serial.println("move");
   
    //portENTER_CRITICAL(&timerPacmanMux);
    
    if ( _userGhostTaskTaskHandle==0)
    {
        _userGhostTaskTaskHandle = xTaskGetCurrentTaskHandle();
        BaseType_t xHigherP=pdFALSE;
        // -- Trigger the show task
        vTaskNotifyGiveFromISR(_GhostTaskHandle,&xHigherP);
        if(xHigherP)
            portYIELD_FROM_ISR();
       // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // _userGhostTaskTaskHandle=0;
    }
    //portEXIT_CRITICAL(&timerPacmanMux);
    
    }



void  Pacman::deplaceGhost(node *ghost,node target)
{
    
    

    long dright,dup,ddown,dleft;
    ghost->x=(ghost->px+2)/4;
    ghost->y=(ghost->py+2)/4;
    up.x=ghost->x;
    up.y=ghost->y;
    down.x=ghost->x;
    down.y=ghost->y;
    left.x=ghost->x;
    left.y=ghost->y;
    right.x=ghost->x;
    right.y=ghost->y;
    switch (ghost->direction)
    {
        case UP:
            if((ghost->py+2)%4>0)
            {
                ghost->py++;
                ghost->x=(ghost->px+2)/4;
                ghost->y=(ghost->py+2)/4;
                return;
            }
            
            break;
        case DOWN:
            if((ghost->py+2)%4>0)
            {
                ghost->py--;
                ghost->x=(ghost->px+2)/4;
                ghost->y=(ghost->py+2)/4;
                return;
            }
            
            break;
        case LEFT:
            if((ghost->px+2)%4>0)
            {
                ghost->px--;
                ghost->x=(ghost->px+2)/4;
                ghost->y=(ghost->py+2)/4;
                return;
            }
            
            break;
        case RIGHT:
            if((ghost->px+2)%4>0)
            {
                ghost->px++;
                ghost->x=(ghost->px+2)/4;
                ghost->y=(ghost->py+2)/4;
                return;
            }
            
            break;
            
    }
    
    switch (ghost->direction)
    {
        case UP:
            //up=*ghost;
            
            //if(!isFreeTile(up)) //on check que devant nous on ce n'est pas un mur et on check gauche droite et un case devant
              //  up.y--;
            left=up;
            left.x--;
            right=up;
            right.x++;
            up.y++;
             dup=calculCostTwoPoints(target,up);
            if(right.x>=22 && right.y==16)
                dright=MAX_DISTANCE;
            else
             dright=calculCostTwoPoints(target,right);
            
            if(left.x<=5 && left.y==16)
                dleft=MAX_DISTANCE;
            else
                dleft=calculCostTwoPoints(target,left);
            
            
            
            if(dup<=dright && up.y!=ghost->y)
            {
                if(dup<=dleft)
                {
                    
                    ghost->py++;
                }
                else
                {
                    
                    ghost->px--;
                    ghost->direction=LEFT;
                }
            }
            else
            {
                if(dright<=dleft)
                {
                   
                    ghost->px++;
                    ghost->direction=RIGHT;
                }
                else
                {
                    ghost->direction=LEFT;
                    ghost->px--;
                }
            }
            ghost->x=(ghost->px+2)/4;
            ghost->y=(ghost->py+2)/4;
            return;
            break;
            
        case DOWN:
            //down=*ghost;
            
            
            left=down;
            left.x--;
            right=down;
            right.x++;
            down.y--;
             ddown=calculCostTwoPoints(target,down);
            if(right.x>=22 && right.y==16)
                dright=MAX_DISTANCE;
            else
             dright=calculCostTwoPoints(target,right);
            if(left.x<=5 && left.y==16)
                dleft=MAX_DISTANCE;
            else
                dleft=calculCostTwoPoints(target,left);
           
            if(ddown<dright && down.y!=ghost->y)
            {
                if(dleft<=ddown)
                {
                    ghost->direction=LEFT;
                    ghost->px--;
                }
                else
                {
                    ghost->direction=DOWN;
                    ghost->py--;
                }
            }
            else
            {
                if(dright<=dleft)
                {
                    ghost->direction=RIGHT;
                    ghost->px++;
                }
                else
                {
                    ghost->direction=LEFT;
                    ghost->px--;
                }
            }
            ghost->x=(ghost->px+2)/4;
            ghost->y=(ghost->py+2)/4;
            return;
            break;
  
        case LEFT:
            //left=*ghost;
            
            //if(!isFreeTile(left)) //on check que devant nous on ce n'est pas un mur et on check gauche droite et un case devant
             //   left.x++;
            down=left;
            down.y--;
            up=left;
            up.y++;
            
            left.x--;
            if(left.x<=5 && left.y==16)
                dleft=MAX_DISTANCE;
            else
                dleft=calculCostTwoPoints(target,left);
             ddown=calculCostTwoPoints(target,down);
             dup=calculCostTwoPoints(target,up);
        
            if(dleft<dup && left.x!=ghost->x)
            {
                if(dleft<=ddown)
                {
                 //ghost->direction=UP;
                    ghost->px--;
                }
                else
                {
                     ghost->direction=DOWN;
                    ghost->py--;
                }
            }
            else
            {
                if(ddown<dup)
                {
                       ghost->direction=DOWN;
                    ghost->py--;
                }
                else
                {
                     ghost->direction=UP;
                    ghost->py++;
                }
            }
            ghost->x=(ghost->px+2)/4;
            ghost->y=(ghost->py+2)/4;
            return;
            break;
  
            
            
        case RIGHT:
            //right=*ghost;
            
            //if(!isFreeTile(right)) //on check que devant nous on ce n'est pas un mur et on check gauche droite et un case devant
             //   right.x--;
            down=right;
            down.y--;
            up=right;
            up.y++;
            right.x++;
        
            if(right.x>=22 && right.y==16)
                dright=MAX_DISTANCE;
            else
                dright=calculCostTwoPoints(target,right);
             ddown=calculCostTwoPoints(target,down);
             dup=calculCostTwoPoints(target,up);
           // if(dright==MAX_DISTANCE && ddown=MAX_DISTANCE && )
            if(dright<dup && right.x!=ghost->x)
            {
                if(dright<=ddown)
                {
                    //ghost->direction=UP;
                    ghost->px++;
                }
                else
                {
                      ghost->direction=DOWN;
                    ghost->py--;
                }
            }
            else
            {
                if(dup<=ddown)
                {
                      ghost->direction=UP;
                    ghost->py++;
                }
                else
                {
                     ghost->direction=DOWN;
                    ghost->py--;
                }
            }
            ghost->x=(ghost->px+2)/4;
            ghost->y=(ghost->py+2)/4;
            return;
            break;
            
    }
    
    //on est forcement sur un case
    
    
}


void Pacman::drawGhost(node *ghost)
{
    
    if(ghost->followupPath==0)
    {
        calculpath(&(*ghost),pacmanPosition);
        //Serial.println("ici");
        //return;
    }
    int x=(ghost->px+2)/4;
    int y=(ghost->py+2)/4;
    
    node f=ghost->path[ghost->followupPath];
    // node f2=pac->Blinky.path[pac->Blinky.followupPath+1];
    //pac->Blinky.x=f.x;
    //pac->Blinky.y=f.y;
    if(x==f.x && y== f.y)
    {
        ghost->x=f.x;
        ghost->y=f.y;
        ghost->followupPath--;
        //pac->Blinky.px=pac->Blinky.x*4-2;
        //pac->Blinky.py=pac->Blinky.y*4-2;
    }
    else
    {
        if(x>f.x)
        {
            if( ghost->py==(f.y*4-2))
            {
                ghost->px--;
            }
            else{
                if(ghost->py>(f.y*4-2))
                {
                    ghost->py--;
                }
                if(ghost->py<(f.y*4-2))
                {
                    ghost->py++;
                }
            }
        }
        if(x<f.x)
        {
            
            if( ghost->py==(f.y*4-2))
            {
                ghost->px++;
            }
            else{
                if(ghost->py>(f.y*4-2))
                {
                    ghost->py--;
                }
                if(ghost->py<(f.y*4-2))
                {
                    ghost->py++;
                }
            }
        }
        if(y>f.y)
        {
            if( ghost->px==(f.x*4-2))
            {
                ghost->py--;
            }
            else{
                if(ghost->px>(f.x*4-2))
                {
                    ghost->px--;
                }
                if(ghost->px<(f.x*4-2))
                {
                    ghost->px++;
                }
            }
        }
        if(y<f.y)
        {
            if( ghost->px==(f.x*4-2))
            {
                ghost->py++;
            }
            else{
                if(ghost->px>(f.x*4-2))
                {
                    ghost->px--;
                }
                if(ghost->px<(f.x*4-2))
                {
                    ghost->px++;
                }
            }
        }
        ghost->x=(ghost->px+2)/4;
        ghost->y=(ghost->py+2)/4;
        
    }
    
}




void Pacman::_PACTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
      Pacman  *pac=(Pacman *)pvParameters;
        pac->notdisplay=true;

        //Serial.println(pac->Blinky.followupPath);
        
        
        //we move pacman
        switch(pac->tempdirectionpac)
        {
            case UP:
                if(pac->canMoveup())
                    pac->directionpac=UP;
                break;
            case DOWN:
                if(pac->canMovedown())
                    pac->directionpac=DOWN;
                break;
            case LEFT:
                if(pac->canMoveleft())
                    pac->directionpac=LEFT;
                break;
            case RIGHT:
                if(pac->canMoveright())
                    pac->directionpac=RIGHT;
                break;
        }
        pac->tempdirectionpac=0;
        
        switch(pac->directionpac)
        {
            case UP:
                pac->moveup();
            break;
            case LEFT:
                pac->moveleft();
                break;
            case DOWN:
                pac->movedown();
                break;
            case RIGHT:
                pac->moveright();
                break;
        }
        //node scatterghost;

        pac->deplaceGhost(&(pac->Blinky),pac->pacmanPosition);
        float t1=(float)(millis()-pac->startime)/1000;
        if(t1>0)
        {
            if(pac->Pinky.inbox)
            {
                pac->Pinky.x=14;
                pac->Pinky.y=19;
                pac->Pinky.direction=UP;
                pac->Pinky.px=(pac->Pinky.x*4);
                pac->Pinky.py=(pac->Pinky.y*4-2);
                pac->Pinky.inbox=false;
            }
            nodedir nd;
            nd.d=pac->pacmanPosition;
            nd.direction=pac->directionpac;
            nodedir d=pac->nextFree(nd);

            

            pac->deplaceGhost(&(pac->Pinky),d.d);//d.d);
        }
        if(t1>10)
        {
            if(pac->Inky.inbox)
            {
                pac->Inky.x=14;
                pac->Inky.y=19;
                pac->Inky.direction=UP;
                pac->Inky.px=(pac->Pinky.x*4);
                pac->Inky.py=(pac->Pinky.y*4-2);
                pac->Inky.inbox=false;
            }
            node target;
            target.x=2*(pac->pacmanPosition.x-pac->Blinky.x)+pac->Blinky.x;
            target.y=2*(pac->pacmanPosition.y-pac->Blinky.y)+pac->Blinky.y;
            pac->deplaceGhost(&(pac->Inky),target);

           
        }
        if(t1>0)
        {
            if(pac->Clyde.inbox)
            {
                pac->Clyde.x=14;
                pac->Clyde.y=19;
                pac->Clyde.direction=UP;
                pac->Clyde.px=(pac->Pinky.x*4);
                pac->Clyde.py=(pac->Pinky.y*4-2);
                pac->Clyde.inbox=false;
            }
            nodedir nd;
            nd.d=pac->pacmanPosition;
            nd.direction=pac->directionpac;
            nodedir d=pac->nextFree(nd);
           if(pac->calculCostTwoPointsabs(pac->Clyde,pac->pacmanPosition)<7)
            pac->deplaceGhost(&(pac->Clyde),*pac->Clyde.scatter );//d.d);
            else
                 pac->deplaceGhost(&(pac->Clyde),pac->pacmanPosition );
        }
        /*
        float t1=(float)(millis()-pac->startime)/1000;
        pac->deplaceGhost(&(pac->Blinky),pac->pacmanPosition);
         if(t1>15)
         {
             if(pac->Pinky.inbox)
             {
                 pac->Pinky.x=14;
                 pac->Pinky.y=19;
                 pac->Pinky.direction=UP;
                 pac->Pinky.px=(pac->Pinky.x*4);
                 pac->Pinky.py=(pac->Pinky.y*4-2);
                 pac->Pinky.inbox=false;
             }
            nodedir nd;
            nd.d=pac->pacmanPosition;
            nd.direction=pac->directionpac;
            nodedir d=pac->nextFree(nd);
            d=pac->nextFree(d);
            d=pac->nextFree(d);
            d=pac->nextFree(d);
            pac->deplaceGhost(&(pac->Pinky),d.d);
         }*/
        /*
        
        
        if( (pac->Blinky.x==pac->pacmanPosition.x && pac->Blinky.y==pac->pacmanPosition.y) || (pac->Clyde.x==pac->pacmanPosition.x && pac->Clyde.y==pac->pacmanPosition.y)|| (pac->Pinky.x==pac->pacmanPosition.x && pac->Pinky.y==pac->pacmanPosition.y) || (pac->Inky.x==pac->pacmanPosition.x && pac->Inky.y==pac->pacmanPosition.y) )
        {
           // Serial.println("TROUVE");
        }
        else{
    
            while(!pac->resetpossible){}
            
            float t1=(float)(millis()-pac->startime)/1000;
            //Serial.printf("time %f\n",t1 );
                pac->drawGhost(&(pac->Blinky));
            if(t1>15)
                pac->drawGhost(&(pac->Clyde));
            if(t1>10)
                pac->drawGhost(&(pac->Pinky));
             if(t1>20)
                 pac->drawGhost(&(pac->Inky));
            pac->countdisplay=(pac->countdisplay+1)%5;
         
         
         */
           // pac->Blinky.px+=pac->finvx;
           // pac->Blinky.py+=pac->finvy;
            //pac->Blinky.x=(pac->Blinky.px+2)/4;
            //pac->Blinky.y=(pac->Blinky.py+2)/4;
            // Blinky.x+=finvx;
            
            
 
            /*
            f=Clyde.path[Clyde.followupPath];
            Clyde.x=f.x;
            Clyde.y=f.y;
            Clyde.followupPath--;
            
            f=Pinky.path[Pinky.followupPath];
            Pinky.x=f.x;
            Pinky.y=f.y;
            Pinky.followupPath--;
            
            */
            // Blinky.y+=finvy;
            
            //displayarray();`
            //pac->displayPacman();
            //FastLED.show();
        //}
        pac->notdisplay=false;
        // delay(10);
        //vTaskNotifyGiveFromISR(_userGhostTaskTaskHandle);
        _userGhostTaskTaskHandle=0;
        //nespad.used=false;
        
        
    }
}
