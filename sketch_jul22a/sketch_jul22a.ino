#include <driver/dac.h>
#include "esp32-hal-dac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "esp_attr.h"
#include "esp_intr.h"
#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "SoundData.h"
unsigned char cosi[500];
uint32_t ech=150;
float v=128;


uint32_t c = 261;
uint32_t d = 294;
uint32_t e = 329;
uint32_t f = 349;
uint32_t g = 391;
uint32_t gS = 415;
uint32_t a = 440;
uint32_t aS = 455;
uint32_t b = 466;
uint32_t cH = 523;
uint32_t cSH = 554;
uint32_t dH = 587;
uint32_t dSH = 622;
uint32_t eH = 659;
uint32_t fH = 698;
uint32_t fSH = 740;
uint32_t gH = 784;
uint32_t gSH = 830;
uint32_t aH = 880;
void beep(uint32_t freq,uint32_t duration)
{
 //Serial.printf("Duration:%d calcul:",duration);
//duration=(duration*ech*freq)/1000;
//Serial.println((1000000/ech/freq)*duration);
uint32_t tempo=240000000/ech/freq;
//Serial.println(tempo);
long t2=ESP.getCycleCount();
long t1;
long c=0;
  while((ESP.getCycleCount()-t2)/240000<duration)
  {
    t1=ESP.getCycleCount();
    //dac_out_voltage(DAC_CHANNEL_1,cosi[c]);
   dacWrite(25,cosi[c]);
   c=(c+1)%ech;
   while(ESP.getCycleCount()-t1<tempo)
    {
      }

  //delayMicroseconds(tempo);
  }
  Serial.println((ESP.getCycleCount()-t2)/240000);
}

float func(float i)
{
  i=i*255/ech;
 /*   if(i<255)
    {
      return (int)(v*(exp(-(127-i)*(127-i)/100000))*(cos(i*3*PI/255)+1));
    }
        if(i<127)
    {
      return v*(cos(i*0.5*PI/255)+1);
    }
        if(i<192)
    {
      return (int)(v*(192-i)/128+v/2)*(cos(i*8*PI/255)+1);
    }
    else
     return (int)((v/2))*(cos(i*0.5*PI/255)+1);
     */
     if(i<64)
     {
      return i/127;
     }
     if(i<128)
      return 0;
      if(i<192)
       return 0;
     else
      return 1-(i-192)/64;
}

void setup() {
 // delay(10000);
  
  Serial.begin(115200);
  
  for (int i=0;i<ech;i++)
  {
   //cosi[i]=func(i);
    //cosi[i]=random(v);
   // Serial.println(sin(1));
    //cosi[i]=(int)v*(sin((long)i*2*PI/ech)+1);
   // Serial.printf("i:%d Value:",i);
    float t=(float)i*2*PI/(float)ech;
   // Serial.println(sin( t)+sin( 2*t)+sin( 3*t)+sin(4*t)+sin(5*t));
   // Serial.println(t);
t=v/7*(sin( t)+sin( 2*t)+sin( 3*t)+sin(4*t)+4);
//t=v/2*(sin(t)+1);
//t=v*(func(i));
   cosi[i]=(int)t;
    Serial.println(cosi[i]);
    
  }
  // put your setup code here, to run once:
beep(440,1000);

Serial.println("done");
/*
for (long i=44;i<33850;i++)
{
 long t2=ESP.getCycleCount();
long t1;


    t1=ESP.getCycleCount();
    //dac_out_voltage(DAC_CHANNEL_1,cosi[c]);
   dacWrite(25,pacman[i]);
  // c=(c+1)%ech;
   while(ESP.getCycleCount()-t1<125*240)
    {
      }

  //delayMicroseconds(tempo);
  
}*/
/*
 firstSection();
 
  //Play second section
  secondSection();
 
  //Variant 1
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay(500);
 
  //Repeat second section
  secondSection();
 
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay(650);
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
   CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
  digitalWrite(25,LOW);
   pinMode(25,INPUT_PULLUP);

   digitalRead(25);
   dac_i2s_disable();
   dac_output_disable(DAC_CHANNEL_1);*/
   pinMode(12,INPUT);
    pinMode(33,INPUT);
     pinMode(32,INPUT);
     pinMode(14,INPUT);
}
int loopv=0;
long delayloop=0;

void loop() {
  if(digitalRead(12)==HIGH)
  {
   beep(880,500);
   Serial.println(digitalRead(12));
  }
  if(digitalRead(33)==HIGH)
  {
   beep(440,500);
   Serial.println(digitalRead(12));
  }
  if(digitalRead(32)==HIGH)
  {
   beep(700,500);
   Serial.println(digitalRead(12));
  }
   if(digitalRead(14)==HIGH)
  {
   beep(700,500);
   Serial.println(digitalRead(12));
  }
//beep(1880,1000);
  //Play first section
 
/*delayloop++;
  // put your main code here, to run repeatedly:
 // dac_out_voltage(DAC_CHANNEL_1,cosi[loopv]);
 dacWrite(25,cosi[loopv]);
loopv=(loopv+1)%e;
delayMicroseconds(1000000/(e*f));*/

}



void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay(350);
}
