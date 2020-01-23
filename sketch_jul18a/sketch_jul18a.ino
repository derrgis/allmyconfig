


#define __TIMER1_MAX 0xFFFF // 16 bit CTR
#define __TIMER1_CNT 0x006// 32 levels --> 0x0130; 38 --> 0x0157 (flicker)
#include <io.h>
#include <interrupt.h>

int delais=100;
int l[27][6][5]={
   {
    {0,0,0,0,0}, //espace
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
    },
  {
    {0,1,0,0,0}, //a
    {1,0,1,0,0},
    {1,1,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },
    {
    {1,1,0,0,0}, //b
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,1,0,0,0},
    {0,0,0,0,0}
    },
    {
    {0,1,1,0,0}, //c
    {1,0,0,0,0},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {0,1,1,0,0},
    {0,0,0,0,0}
    },
    {
    {1,1,0,0,0}, //d
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,1,0,0,0},
    {0,0,0,0,0}
    },
   {
    {1,1,1,0,0}, //e
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,0,0,0,0},
    {1,1,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {1,1,1,0,0}, //f
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0}
    },  
     {
    {0,1,1,0,0}, //g
    {1,0,0,0,0},
    {1,0,1,1,0},
    {1,0,0,1,0},
    {0,1,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {1,0,1,0,0}, //H
    {1,0,1,0,0},
    {1,1,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {1,1,1,0,0}, //I
    {0,1,0,0,0},
    {0,1,0,0,0},
    {0,1,0,0,0},
    {1,1,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {0,1,1,0,0}, //J
    {0,0,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {1,1,0,0,0},
    {0,0,0,0,0}
    },  
      {
    {1,0,1,0,0}, //K
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },       
    {
    {1,0,0,0,0}, //L
    {1,0,0,0,0},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {0,1,1,0,0},
    {0,0,0,0,0}
    }, 
    {
    {1,0,1,0,0}, //M
    {1,1,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {1,1,0,0,0}, //N
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {0,1,0,0,0}, //O
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,1,0,0,0},
    {0,0,0,0,0}
    },    
    {
    {1,1,0,0,0}, //P
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0}
    },    
   {
    {0,1,1,1,0}, //Q
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,0,0,1},
    {0,1,1,1,0},
    {0,0,0,0,1}
    },    
    {
    {1,1,0,0,0}, //R
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,0,0,0,0}
    },    
    {
    {0,1,1,0,0}, //S
    {1,0,0,0,0},
    {0,1,0,0,0},
    {0,0,1,0,0},
    {1,1,0,0,0},
    {0,0,0,0,0}
    },  
   {
    {1,1,1,0,0}, //T
    {0,1,0,0,0},
    {0,1,0,0,0},
    {0,1,0,0,0},
    {0,1,0,0,0},
    {0,0,0,0,0}
    },  
    {
    {1,0,1,0,0}, //U
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,0,1,0,0},
    {0,1,1,0,0},
    {0,0,0,0,0}
    },  
   {
    {1,0,1,0,0}, //V
    {1,0,1,0,0},
    {1,0,1,0,0},
    {1,1,0,0,0},
    {1,0,0,0,0},
    {0,0,0,0,0}
    },  
    {
    {1,0,0,0,1}, //W
    {1,0,0,0,1},
    {1,0,0,0,1},
    {1,0,1,0,1},
    {0,1,0,1,0},
    {0,0,0,0,0}
    },  
     {
    {1,0,0,0,1}, //X
    {0,1,0,1,0},
    {0,0,1,0,0},
    {0,1,0,1,0},
    {1,0,0,0,1},
    {0,0,0,0,0}
    },         
     {
    {1,0,1,0,0}, //Y
    {1,0,1,0,0},
    {0,1,1,0,0},
    {0,0,1,0,0},
    {0,0,1,0,0},
    {0,0,0,0,0}
    },  
    {
    {1,1,1,0,0}, //Z
    {0,0,1,0,0},
    {0,1,0,0,0},
    {1,0,0,0,0},
    {1,1,1,0,0},
    {0,0,0,0,0}
    },      
   };
    
int taille[27]={2,4,4,4,4,4,4,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
int screen[8][8];

void setup() {
  // put your setup code here, to run once:
pinMode (8,OUTPUT);
pinMode (9,OUTPUT);
pinMode (10,OUTPUT);
pinMode (11,OUTPUT);
pinMode (12,OUTPUT);
pinMode (13,OUTPUT);
for (int i=0;i<=7;i++)
{
  pinMode (i,OUTPUT);
}
setup_timer1_ovf() ;
}

void picon(int i,int j)
{
  screen[i][j]=1;
}

void picoff(int i,int j)
{
  screen[i][j]=0;
}


void slidescreengdcycle()
{
  //cli();
  for (int i=0;i<=7;i++)
  {
    int save=screen[i][7];
    for (int j=6;j>=0;j--)
    {
      screen[i][j+1]=screen[i][j];
    }
    screen[i][0]=save;
  }
    //sei();
}


void slidescreendgcycle(int lettre)
{
  //cli();
  for (int g=0;g<taille[lettre];g++)
  {
    for (int i=0;i<=7;i++)
    {
      //int save=screen[i][0];
      for (int j=0;j<7;j++)
      {
        screen[i][j]=screen[i][j+1];
      }
      if (g>4)
      {
        screen[i][7]=0;
      }else{
      screen[i][7]=l[lettre][i][g];
      }
    }
    delay(delais);
  }
    //sei();
}


void slidescreendgcycle()
{
  //cli();
  for (int i=0;i<=7;i++)
  {
    int save=screen[i][0];
    for (int j=0;j<7;j++)
    {
      screen[i][j]=screen[i][j+1];
    }
    screen[i][7]=save;
  }
    //sei();
}

void loop() {
  
  char mess[]="BONJOUR ON EST VENDREDI ";
  int line[8]={0,0,0,0,0,0,0,0};
  int line0[8]={0,0,0,0,0,0,0,0};
  int line2[8]={0,0,0,0,0,0,0,0};
  int line3[8]={0,0,0,0,0,0,0,0};
  int line4[8]={0,0,0,0,0,0,0,0};
  int line5[8]={0,0,0,0,0,0,0,0};
 for (int i=0;i<8;i++)
 {
   screen[1][i]=line[i];
    screen[2][i]=line2[i];
    screen[0][i]=line0[i];
    screen[3][i]=line3[i];
    screen[4][i]=line4[i];
    screen[5][i]=line5[i];
 }
  // put your main code here, to run repeatedly: 
  int i;
  for(i=8 ;i<=11;i++)
{
  digitalWrite(i,HIGH);
  
}
  for(i=0 ;i<=7;i++)
{
  digitalWrite(i,HIGH);
  
}
 //DDRD = B11111110; 
//PORTB = B10101000; // sets digital pins 7,5,3 HIGH
int k=0;
//delay(2000);
//slidescreen();
int lens=strlen(mess);
while (1)
{
 //picoff(4,k%8);
// picoff(1,k%8);

 //picon(4,k%8);
  //picon(1,k%8);
  int offset=0;
  int ascd=(int)mess[k%lens];
  if (ascd>=65)
  {
   offset=65-1;
  }
  else
  {
   offset=32;
  }
  slidescreendgcycle(ascd-offset);
   k++;
//delay(100);
}}

void setup_timer1_ovf(void) {
  // Arduino runs at 16 Mhz...
  // Timer1 (16bit) Settings:
  // prescaler (frequency divider) values:   CS12    CS11   CS10
  //                                           0       0      0    stopped
  //                                           0       0      1      /1  
  //                                           0       1      0      /8  
  //                                           0       1      1      /64
  //                                           1       0      0      /256 
  //                                           1       0      1      /1024
  //                                           1       1      0      external clock on T1 pin, falling edge
  //                                           1       1      1      external clock on T1 pin, rising edge
  //
  TCCR1B &= ~ ( (1<<CS11) );
  TCCR1B |= ( (1<<CS12) | (1<<CS10) );      
  //normal mode
  TCCR1B &= ~ ( (1<<WGM13) | (1<<WGM12) );
  TCCR1A &= ~ ( (1<<WGM11) | (1<<WGM10) );
  //Timer1 Overflow Interrupt Enable  
  TIMSK1 |= (1<<TOIE1);
  TCNT1 = __TIMER1_MAX - __TIMER1_CNT;
  // enable all interrupts
  sei(); 
}

ISR(TIMER1_OVF_vect) { /* Framebuffer interrupt routine */
  TCNT1 =  __TIMER1_MAX - __TIMER1_CNT;
  byte cycle;
   


  for (int  i=0;i<=5;i++)
  {
   
    
  
    

  
  for (int j=0;j<=7;j++)
  {
    digitalWrite(j,screen[i][7-j]);
  }
    digitalWrite(i+8,LOW);
  
  /*for(int l=0;l<100000;l++)
  {
  }*/
  //delay(1);
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  
  digitalWrite(i+8,HIGH);
  }

}
