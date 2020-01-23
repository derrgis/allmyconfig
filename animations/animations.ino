#define LED_WIDTH 100
#define LED_HEIGHT 64
#define NUM_LEDS LED_WIDTH * LED_HEIGHT


//copy your code for your panel set up

#include "FastLED.h"

const float pi = 3.14159265359;
CRGB  leds[NUM_LEDS];

void PixelOn(int x,int y,CRGB Color)
{
    
    if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return ;
    
        leds[x+y*LED_WIDTH]=Color;
    
    
}

CRGB line[LED_WIDTH];

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


void straightLine(int x1,int y1,int x2,CRGB Color)
{
    if (y1<0  or y1>=LED_HEIGHT)
        return;
    if(x1>=LED_WIDTH and x2>=LED_WIDTH)
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
    
    if(Color != line[0])
    {
        line[0]=Color;
        memfill(line, sizeof(line), sizeof(line[0]));
    }
    int fx=0;
    int longueur=0;
    
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

void    glbDrawLine(int firstx,int firsty,int lastx,int lasty,CRGB Color)
{
    int         x1;        /* debut de la ligne => milieu de la ligne */
    int         y1;        /* debut de la ligne => milieu de la ligne */
    int         x2;        /* fin de la ligne => milieu de la ligne */
    int         y2;        /* fin de la ligne => milieu de la ligne */
    int         dx;        /* longueur X de la droite */
    int         dy;        /* longueur Y de la droite */
    int         sub;        /* utilise pour des soustractions */
    int         remain;        /* nombre de pixel restant */
    int         error;        /* erreur du trace, doit etre corrige si positif */
    int         inc1;        /* pour des incrementations/decrementations */
    int         inc2;        /* pour des incrementations/decrementations */
    
    /*
     x1 = premier point X
     x2 = dernier point X
     y1 = premier point Y
     y2 = dernier point Y
     
     si firstx > lastx les premiers et les derniers sont inverses
     
     
     */
    
    if(firsty==lasty)
    {
        straightLine(firstx,firsty,lastx,Color);
        return;
    }
    
     if (firstx>lastx)
     {
     x1=lastx;
     x2=firstx;
     y1=lasty;
     y2=firsty;
     }
     else
     {
     x1=firstx;
     x2=lastx;
     y1=firsty;
     y2=lasty;
     }
     dx=x2-x1;    /* longueur X de la droite */
    dy=y2-y1;    /* longueur Y de la droite */
    if ((!dx)&&(!dy)) return; /* quitte si aucun trace a effectuer */
    
    
     if(y2<y1)

     {
     dy=y1-y2;
     inc1=-1;
     inc2=1;
     }
     else
     {
         dy=y2-y1;
     inc1=1;
     inc2=1;
     }
     
     
    if (dx>dy)
    {
        sub=dx-dy;
        error=dy-(dx>>1);
        remain=(dx+1)>>1;    
                              do
                              {
                             // func(x1,y1);
                                 PixelOn(x1,y1,Color);
                                  /* appelle la fonction de dessin avec en parametre */
        //func(x2,y2);        /* les positions des points a dessiner */
        PixelOn(x2,y2,Color);
        x1+=inc2;    /* la position x1 augmente */
        x2-=inc2;   /* et la position x2 diminue (on va vers le centre) */
        if (error>=0)    /* si l'erreur est assez grande pour etre corrigee */
        {
            y1+=inc1;    /* la position y1 augmente */
            y2-=inc1;    /* et la position y2 diminue (on va vers le centre) */
            error-=sub;    /* diminue l'erreur */
        }
        else error+=dy;    /* sinon augmente l'erreur */
    } while (--remain>0);    /* diminue remain et recommence s'il reste des points a tracer */
    if (!(dx&1)) PixelOn(x1,y1,Color);    /*
                                  si dx etait pair l'algo n'a pas pu tracer le point du milieu
                                  car il a fait une division entiere par deux sur dx pour
                                  connaitre le nombre de points a tracer et a ainsi perdu le 1
                                  
                                  
                                  */
                                  
                                  return;        /* fin de la fonction */
}
else
{
    sub=dy-dx;
    error=dx-(dy>>1);
    remain=(dy+1)>>1;    
                          
                          do
                          {
                          PixelOn(x1,y1,Color);   /* appelle la fonction de dessin avec en parametre */
    PixelOn(x2,y2,Color);        /* les positions des points a dessiner */
    y1+=inc1;    /* la position y1 augmente */
    y2-=inc1;   /* et la position y2 diminue (on va vers le centre) */
    if (error>=0)    /* si l'erreur est assez grande pour etre corrigee */
    {
        x1+=inc2;    /* la position x1 augmente */
        x2-=inc2;    /* et la position x2 diminue (on va vers le centre) */
        error-=sub;    /* diminue l'erreur */
    }
    else error+=dx;    /* sinon augmente l'erreur */
        } while (--remain>0);    /* diminue remain et recommence s'il reste des points a tracer */
if (!(dy&1)) PixelOn(x1,y1,Color);   
                              
                              return;        /* fin de la fonction */
}
}




void dessinePoly(int x0,int y0,float r,float start,int faceNumber,CRGB Color)
{
    if (faceNumber<2)
        return;
    if (r<0)
        return;
    
    int xs,ys,xn,yn,xi,yi;
    
    xs=(int)round((float)r*cos((float)start)+x0);
    ys=(int)round((float)r*sin((float)start)+y0);
    xi=xs;
    yi=ys;
    for(int i=1;i<faceNumber;i++)
    {
        xn=(int)round((float)r*cos((float)start+2*pi*i/faceNumber)+x0);
        yn=(int)round((float)r*sin((float)start+2*pi*i/faceNumber)+y0);
        glbDrawLine(xn,yn,xs,ys,Color);
        xs=xn;
        ys=yn;
    }
    glbDrawLine(xn,yn,xi,yi,Color);
    
}


void circleFilled (int xc,int yc,int R, CRGB Color)
{
    int x=0;
    int y=R;
    int d=(-R)/2;
    if(R<0)
        return;
    while(x<=y)
    {

        glbDrawLine(xc+x,yc+y,xc-x,yc+y, Color);
        glbDrawLine(xc-x,yc-y,xc+x,yc-y, Color);
        glbDrawLine(xc+y,yc-x,xc-y,yc-x, Color);
        glbDrawLine(xc-y,yc+x,xc+y,yc+x, Color);

        
        if(d<=0)
        {
            x++;
            d+=x;
        }
        else
        {
            x++;
            y--;
            d=d+x-y;
            // d=d-y;
        }
        
    }
    
}



void setup() {
  // put your setup code here, to run once:
  //put your setup code

}






int r=0;
int dire=0;
uint16_t k=0;
int anim=0;
CRGB bgColor;
void loop() {
  // put your main code here, to run repeatedly:
anim=(k/1000)%3;

switch (anim)
{
  case 0:
        {
        float ab=(float)(r/10)*2*PI/720;
       int offx=-LED_WIDTH*(sin(3*ab)+1)/2;
       int offy=-LED_HEIGHT*(sin(10*ab)+1)/2;
       for(int x1=1;x1<LED_WIDTH-1;x1++)
          for(int y1=0;y1<LED_HEIGHT;y1++)
          {
            int x=x1+offx;
            int y=y1+offy;
            
           int coloor= 
          (
              sin8(x * 9.0)
           + sin8(y *5.0)
            + sin8((x + y) *10.0)
            + sin8(sqrt(double(x * x + y * y)) * 6.0)
          ) / 4;
           PixelOn(x1,y1,CHSV((coloor+r)%256,255,255));
      
          }
          r=r+5;
        }
        break;

   case 1: 
        {
           
              bgColor=CRGB(10,10,10);
              fill_solid(leds,bgColor,NUM_LEDS);
              
               if(dire>=0)
                           {
                              r=r+1;
                              if(r>100)
                                dire=-1;
                           }
                            else
                            {
                              r=r-1;
                              if(r<1)
                                dire=0;
                            }
                            //r=(r+(int)k/10)%20;
                           //fill(CRGB(5, 5, 5));
                            dessinePoly(61,24, r,k*pi/360,3,CRGB(255,0,0));
                           int r1=r-10;
                           dessinePoly(61,24, r1,k*1.5*pi/360,6,CRGB(0,255,0));
                
                            r1=r-20;
               
                           dessinePoly(61,24, r1,-k*pi/360,5,CRGB(0,255,255));
                            r1=r-30;
                         dessinePoly(61,24, r1,+k*2*pi/360,4,CRGB(0,0,255));
                                   
                            r1=r-40;      
                           dessinePoly(61,24, r1,-k*1.5*pi/360,3,CRGB(255,0,255));
                           r1=r-50;
                           dessinePoly(61,24, r1,k*1.5*pi/360,6,CRGB(255,255,255));
                
                            r1=r-60;
               
                           dessinePoly(61,24, r1,-k*pi/360,5,CRGB::Yellow);
                            r1=r-70;
                           dessinePoly(61,24, r1,+k*2*pi/360,4,CRGB::Red);
                                   
                            r1=r-80;      
                           dessinePoly(61,24, r1,-k*1.5*pi/360,3,CRGB::Blue);
                          // fill_solid(leds, NUM_LEDS, CRGB(255,0,0));
                      k++;     
              
              }
        break;
        case 2:
        {
            int rayon=3;

          
          fadeToBlackBy(leds, (LED_WIDTH * LED_HEIGHT), 20);
          float ab=(float)r*2*PI/720;
          circleFilled(LED_WIDTH*(sin(5*ab)+1)/2,LED_HEIGHT*(sin(3*ab)+1)/2,rayon,CHSV(r%255,255,255));
          circleFilled(LED_WIDTH*(sin(5*ab)+1)/2,LED_HEIGHT*(sin(7*ab)+1)/2,rayon,CHSV((r+180)%255,255,255));
          circleFilled(LED_WIDTH*(sin(4*ab)+1)/2,LED_HEIGHT*(sin(9*ab)+1)/2,rayon,CHSV((r+60)%255,255,255));
          circleFilled(LED_WIDTH*(sin(7*ab)+1)/2,LED_HEIGHT*(sin(3*ab)+1)/2,rayon,CHSV((r+120)%255,255,255));
          circleFilled(LED_WIDTH*(sin(2*ab)+1)/2,LED_HEIGHT*(sin(7*ab)+1)/2,rayon,CHSV((r+30)%255,255,255));
          circleFilled(LED_WIDTH*(sin(11*ab)+1)/2,LED_HEIGHT*(sin(4*ab)+1)/2,rayon,CHSV((r+80)%255,255,255));
          circleFilled(LED_WIDTH*(sin(1*ab)+1)/2,LED_HEIGHT*(sin(7*ab)+1)/2,rayon,CHSV((r+30)%255,255,255));
          circleFilled(LED_WIDTH*(sin(10*ab)+1)/2,LED_HEIGHT*(sin(11*ab)+1)/2,rayon,CHSV((r+70)%255,255,255));
          //float dc=;
          //Serial.printf("gg:%f  %f\n",ab,dc);
            
          r=(r+1)%10000000;
        }
        break;
     
}

FastLED.show();
    

}
