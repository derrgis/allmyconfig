//
//  PacmanClass.h
//  
//
//  Created by Yves BAZIN on 29/12/2019.
//

#ifndef PacmanClass_h
#define PacmanClass_h

#include <stdio.h>
#include "FastLED.h"
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4


struct node{
    int x,y;
    long cost;
    long pathlength;
    int px,py;
    int pax,pay;
    node *path;
    int followupPath;
    int shortpathlength;
    int direction;
    bool inbox;
    node *scatter;
};

struct nodedir{
    node d;
    int direction;
};
struct directionst{
    int vx,vy;
};
static  TaskHandle_t _GhostTaskHandle ;
static  TaskHandle_t _userGhostTaskTaskHandle;
class Pacman {
    
    
public:
     Pacman(CRGB *leds);
     Pacman(int width,int height,CRGB *leds );
    node up,down,left,right;
    node  pacmanPosition,Blinky,Pinky,Inky,Clyde,targetPacmanPosition,targetBlinky;
    node scatterPinky,scatterInky,scatterClyde,scatterBlinky;
    static void _PACTask(void *pvParameters);
    volatile int value=0;
    void displayPacman();
       directionst pacmanDirection;
    node scatterghost;
    int decalDisplay;
    int virtualPacmanY;
//protected:
    int width;
    int height;
    CRGB *leds;
    bool  resetpossible=true;
    void drawGhost(node *ghost);
    void putOrientation2(CRGB *pic,int h,int w,int x0,int y0);
    void displayBitmapFromProgmem(const unsigned char *pic,int x0,int y0,int h,int w );
    void displayPicNew(CRGB *pica, int x0, int y0, int h, int w);
    void displayPicNewInv(CRGB *pica, int x0, int y0, int h, int w);
    void displaypic( CRGB *pica, int x0, int y0, int h, int w);
    void displaygif( CRGB *pica, long int x0, long int y0, long int h, long int w,long pic);
    void displaygifdir( CRGB *pica, long int x0, long int y0, long int h, long int w,long  pic,int direc);
    void calculpic(int *pic, CRGB *palette, CRGB color, int h, int w,CRGB *result);
    void putOrientation(CRGB *pic,int h,int w,int x0,int y0);
    void moveleft();
    void moveup();
    void movedown();
    void moveright();
    node newMinimumNode();
    void calculpath(node *start, node target);
    bool isInShortPath(node target,node n);
    int getNode(node n);
    node getParent(node n);
    long calculCost(struct node  n);
     void ghostDirection(struct node n);
    bool isFreeTile(struct node n);
    void addInOpenList(node n);
    void addInClosedList(node n);
    bool isInOpenList(node n);
    bool isInClosedList(node n);
    void emptyClosedList();
    void emptyOpenList();
    void static IRAM_ATTR moveGhosts();
    int followupPath=0;
    int gohstInterval;
    int pacmanScore;
    int directionpac=0;
    int shortpathlength=0;
    int openListSize=0;
    int closedListSize=0;
    int pacmanFace=0;
    
    bool incalcul=false;
    bool updatepossible=true;
    bool isPacmanMoving=true;
    bool inPacman=true;
    volatile static int finvx,finvy;
    uint32_t pacmanInterval=3000;
    byte facestuff=0;
    directionst blinkyDirection;
    struct node openList[50];
    struct node closedList[50];
    //node shortpath[300];
    node shortpathBlinky[10];
    node shortpathClyde[10];
    node shortpathPinky[10];
     node shortpathInky[10];
    struct directionst finalDirection={0,0};
    void start();
    bool notdisplay=false;
    nodedir nextFree(nodedir nd);
    int countdisplay=0;
    void checkpastille();
    void ghostMovement();
    long timewhite=0;
    bool isafraid=false;
    long startime=0;
    bool  canMoveup();
    bool  canMovedown();
    bool  canMoveleft();
    bool  canMoveright();
    int tempdirectionpac;
    long calculCostTwoPoints(node n,node n1);
    void  deplaceGhost(node *ghost,node target);
    long  calculCostTwoPointsabs(node n,node n1);
    int getTile(int x,int y);
};
#endif /* PacmanClass_h */
