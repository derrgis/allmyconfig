int latchPin = 10;
//Pin connected to SH_CP of 74HC595
int clockPin = 13;
////Pin connected to DS of 74HC595
int dataPin = 11;
 
//int latchPinv = 7;
//Pin connected to SH_CP of 74HC595
//int clockPinv = 5;
////Pin connected to DS of 74HC595
//int dataPinv = 6;
int i;
int delai=50;
#define __TIMER1_MAX 0xFFFF // 16 bit CTR
#define __TIMER1_CNT 0x0150// 32 levels --> 0x0130; 38 --> 0x0157 (flicker)
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
char tt[]="ABCDEFGHIJKL";
char tt2[]="ABCDEFGHIJKL";
void cls()
{
  for(int j=0;j<8;j++)
  {
    tab[j]=0;
    tab2[j]=0;
  }
}


byte spi_transfer(byte data) {
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte. (we don't need that here)
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
  setup_hardware_spi();
  sei();
  Serial.begin(9600);
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
    for (int i=1;i<=5;i++)
    {
       tab[i+1]=(tab[i+1]<<1) | (!!(lettre[let][i] & (1<<(taille[let]-1-g))) & coul );
       tab2[i+1]=(tab2[i+1]<<1)| (!!(lettre[let][i] & (1<<(taille[let]-1-g)))& !!(coul & 2) );
    }
   delay(delai); 
  }
}


void loop() {
  cls();
 
  int k=0;
  
  char mess[]="YVES BAZIN S AMUSE UN PEU LOL AVEC UN ARDUINO ET JE SUIS SUPER CONTENT  ";
  byte coul[]={1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3,1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3,1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3,1,2,2,3,3,2,3,1,2,3,1,1,2,3,3,3,2,3,1,2,3,1,1,2,3,3,3};
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
  scrolllettre(ascd-offset,2);
   k++;
  
  }
  /*while(1)
  {
   Serial.print("g:");
 Serial.println((int)&tt2);
 Serial.print("r:");
 Serial.println((int)&tt2);
  }*/
} 
 
void pixon(int x,int y,byte red,byte green)
 {
   tab[y]=tab[y] | ((1 <<x) & (green<<x));
   tab2[y]=tab2[y] | ((1 <<x)&(red<<x));
 }
 
 void pixoff(int x,int y)
 {
   tab[y]=tab[y] & ~(1 <<x);
   tab2[y]=tab2[y] & ~(1 <<x);
 }
 
 void setup_hardware_spi(void) {
  byte clr;
  // spi prescaler: 
  // SPI2X SPR1 SPR0
  //   0     0     0    fosc/4
  //   0     0     1    fosc/16
  //   0     1     0    fosc/64
  //   0     1     1    fosc/128
  //   1     0     0    fosc/2
  //   1     0     1    fosc/8
  //   1     1     0    fosc/32
  //   1     1     1    fosc/64
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  //SPCR |= ( (1<<SPR1) ); // set prescaler bits
  SPCR &= ~ ( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  clr=SPSR; // clear SPI status reg
  clr=SPDR; // clear SPI data reg
  SPSR |= (1<<SPI2X); // set prescaler bits
  //SPSR &= ~(1<<SPI2X); // clear prescaler bits
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
  int redaddr=(int)&tab2;
 int greenaddr=(int)&tab; 

int port;

   

    __asm__(
    //sauvegarde des registres
     "PUSH R0\n\t"
     "PUSH R1\n\t"
     "PUSH R16\n\t"
     "PUSH R17\n\t"
     "PUSH R18\n\t"
     "PUSH R28\n\t"
     "PUSH R29\n\t"
     "PUSH R30\n\t"  
     "PUSH R31\n\t"
 
 //code begin
  "mov r30,%A0\n\t" //load green dans z
  "mov r31,%B0\n\t" 
  //"ld z,%0\n\t"
 "mov r28,%A1\n\t" //load red dans y
  "mov r29,%B1\n\t" 
  //eteint le lactch
   
    
  "ldi r16,0x1\n\t" //row
  "mov r1,r16\n\t"
  "ldi r16,8\n\t" //loop value
   "mov r0,r16\n\t"
   
  "1:\n\t"  //loop1
  "in r18,0x05\n\t" 
   "cbr r18,0x4\n\t"
   "out 0x05,r18\n\t"
  "nop\n\t"
  "ld r16,Z+\n\t" //on lite green
  "ld r17,Y+\n\t" //on lit red
  
  "COM r16\n\t"
  "COM r17\n\t"
  
  "out	0x2E, r17\n\t"
 "nop\n\t"
 "2: \n\t"
 "in r17,0x2D\n\t"
 "sbrs r17,7\n\t"
 "rjmp 2b\n\t"

 "out	0x2E, r16\n\t"
 "nop\n\t"
 "3: \n\t"
 "in r17,0x2D\n\t"
 "sbrs r17,7\n\t"
 "rjmp 3b\n\t"

 //"COM r1\n\t"
  "out	0x2E, r1\n\t"
 "nop\n\t"
 "4: \n\t"
 "in r17,0x2D\n\t"
 "sbrs r17,7\n\t"
 "rjmp 4b\n\t"
 //"COM r1\n\t"
 "LSL r1\n\t"

 
 "in r18,0x05\n\t" 
 "sbr r18,0x4\n\t"
 "out 0x05,r18\n\t"
   "nop\n\t"
 "in r18,0x05\n\t" 
   "cbr r18,0x4\n\t"
   "out 0x05,r18\n\t"
 
 "ldi r17,0xF\n\t"
 "6: \n\t"
 
 "ldi r16,0xFF\n\t"
 "5: nop\n\t"
  "dec r16\n\t"
 "brne 5b\n\t"
  "dec r17\n\t"
 "brne 6b\n\t"
 
 
 "dec r0\n\t"
 "brne 7f\n\t"
 "jmp 8f\n\t"
 "7: jmp 1b\n\t"
 "8:\n\t"
 
 
 
   //recuperation des registres
    "POP R31\n\t"
    "POP R30\n\t"
    "POP R29\n\t"
    "POP R28\n\t"
    "POP R18\n\t"
    "POP R17\n\t"
    "POP R16\n\t"
    "POP R1\n\t"
    "POP R0\n\t"    
    
    ::"e" (redaddr) , "e" (greenaddr)); // on envoie les adresses des tableaux
 
  


}
