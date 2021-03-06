int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;
//int latchPinv = 7;
//Pin connected to SH_CP of 74HC595
//int clockPinv = 5;
////Pin connected to DS of 74HC595
//int dataPinv = 6;
int i;
int delai=20;
#define __TIMER1_MAX 0xFFFF // 16 bit CTR
#define __TIMER1_CNT 0x00004F// 32 levels --> 0x0130; 38 --> 0x0157 (flicker)
#include <io.h>
#include <interrupt.h>

byte lettre[][5]={
  {
    B0000,
    B0000,
    B0000,
    B0000,
    B0000
    },
  {
    B0100, //a
    B1010,
    B1110,
    B1010,
    B1010
  },
  {
    B1100, //b
    B1010,
    B1100,
    B1010,
    B1100
   
  },
  {
    B0110, //c
    B1000,
    B1000,
    B1000,
    B0110
  },
   {
    B1100, //D
    B1010,
    B1010,
    B1010,
    B1100
  },
    {
    B1110, //e
    B1000,
    B1100,
    B1000,
    B1110
    },    
    {
    B1110, //f
    B1000,
    B1100,
    B1000,
    B1000
    },  
     {
    B01100, //g
    B10000,
    B10110,
    B10010,
    B01100,
    },    
    {
    B1010, //H
    B1010,
    B1110,
    B1010,
    B1010,
    },    
    {
    B10, //I
    B10,
    B10,
    B10,
    B10,
    },   
    {
    B0110, //J
    B0010,
    B0010,
    B0010,
    B1100,
    },  
      {
    B1010, //K
    B1010,
    B1100,
    B1010,
    B1010,
    },       
    {
    B1000, //L
    B1000,
    B1000,
    B1000,
    B0110,
    }, 
    {
    B1010, //M
    B1110,
    B1010,
    B1010,
    B1010,
    },    
    {
    B1100, //N
    B1010,
    B1010,
    B1010,
    B1010,
    },    
    { //o
   B0100,
   B1010,
   B1010,
   B1010,
   B0100,
    },    
    {  //p
    B1100,
    B1010,
    B1100,
    B1000,
    B1000,
    },    
   {
    B0110, //Q
    B1010,
    B0110,
    B0010,
    B0010
    },    
    { //R
    B1100,
    B1010,
    B1100,
    B1010,
    B1010
    },   
    {
     B0110,
     B1000,
     B0110,
     B0010,
     B1100
    },  
   {//T
    B1110,
    B0100,
    B0100,
    B0100,
    B0100
    },  
    { //U
    B1010,
    B1010,
    B1010,
    B1010,
    B0110
    },  
   {//v
   B1010,
   B1010,
   B1010,
   B1100,
   B1000,
    },  
    {//w
    B1010,
    B1010,
    B1010,
    B1110,
    B1010
    },  
     { //x
    B1010,
    B1010,
    B0100,
    B1010,
    B1010,
    },         
     { //y
   B1010,
   B1010,
   B0100,
   B0100,
   B0100
   },   
    {//z
   B1110,
   B0010,
   B0100,
   B1000,
   B1110,
    }
};

int taille[27]={2,4,4,4,4,4,4,5,4,2,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

byte tab[8]={B11111111,B00000000,B11110000,B00001111,B11100011,B1110000,B00000011,B11011100};
byte tab2[8]={B11111111,B00000000,B11110000,B00001111,B11100011,B1110000,B00000011,B11011100};
void cls()
{
  for(int j=0;j<8;j++)
  {
    tab[j]=0;
    tab2[j]=0;
  }
}

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  /* pinMode(latchPinv, OUTPUT);
  pinMode(clockPinv, OUTPUT);
  pinMode(dataPinv, OUTPUT);*/
  setup_timer1_ovf();
  sei();
 // Serial.begin(9600);
}

void scrolldg()
{
   for(int j=0;j<8;j++)
  {
    tab[j]=tab[j]<<1;
    tab2[j]=tab2[j]<<1;
  }
}


void scrolllettre(int let,byte coul)
{
  for (int g=0;g<taille[let];g++)
  {
    for (int i=0;i<=4;i++)
    {
       tab[i+1]=(tab[i+1]<<1) | (!!(lettre[let][i] & (1<<(taille[let]-1-g))) & coul );
       tab2[i+1]=(tab2[i+1]<<1)| (!!(lettre[let][i] & (1<<(taille[let]-1-g)))& !!(coul & 2) );
    }
   delay(delai); 
  }
}


void loop() {
  cls();
  //count up routine
 /* for(int i=0;i<8;i++)
  {
   
      digitalWrite(latchPinv, LOW);
       digitalWrite(latchPin, LOW);
       
      shiftOut(dataPinv, clockPinv, MSBFIRST, ~(1<<i));   
      
  //  for (int j = 0; j < 8; j++) {
    //ground latchPin and hold low for as long as you are transmitting
   
    shiftOut(dataPin, clockPin, MSBFIRST, tab[i]);   
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, HIGH);
     digitalWrite(latchPinv, HIGH);
    
  
  
  }*/
  //delay(200);
 // Serial.println('demarre');
  //pixon(1,1);
  int k=0;
  
  char mess[]="YVES BAZIN S AMUSE UN PEU LOL  ";
  byte coul[]={1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3,1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3};
// scrolllettre(0);
int lens=strlen(mess);
  while(1){
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
  scrolllettre(ascd-offset,coul[k%lens]);
   k++;
  // delay(500);
   //scrolldg();
   
   /*pixoff(k%8,0);
    pixoff((k+1)%8,2);
    pixoff((k+2)%8,3);
  k++;
  //Serial.println("ee");
  pixon(k%8,1,1,1);

  //k++;
  //Serial.println("ee");
  pixon((k+1)%8,2,0,1);
  pixon((k+2)%8,3,1,0);
  delay(30);*/
  }
  
} 
 
void pixon(int x,int y,byte red,byte green)
 {
   tab[y]=(tab[y] | (1 <<x))& (green<<x);
   tab2[y]=(tab2[y] | (1 <<x))&(red<<x);
 }
 
 void pixoff(int x,int y)
 {
   tab[y]=tab[y] & ~(1 <<x);
   tab2[y]=tab2[y] & ~(1 <<x);
 }
 
 
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
   

for( i=0;i<8;i++)
  {
   
      digitalWrite(latchPin, LOW);
       //digitalWrite(latchPin, LOW);
       
       
      
  //  for (int j = 0; j < 8; j++) {
    //ground latchPin and hold low for as long as you are transmitting
   shiftOut(dataPin, clockPin, LSBFIRST, tab2[i]);
    shiftOut(dataPin, clockPin, LSBFIRST, tab[i]);   
    shiftOut(dataPin, clockPin, MSBFIRST, ~(1<<i));  
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
   // digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, HIGH);
    
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
     __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
     __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
 /*  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
     __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
     __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
   __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t"); __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  */
  // digitalWrite(latchPin, LOW);
     digitalWrite(latchPin, LOW);
  //shiftOut(dataPin, clockPin, MSBFIRST, B11111111); 
   shiftOut(dataPin, clockPin, MSBFIRST, B11111111); 
   // digitalWrite(latchvinv, HIGH);
    digitalWrite(latchPin, HIGH);
  
  }
 
  
  /*for(int l=0;l<100000;l++)
  {
  }*/
  //delay(1);
 
  


}
