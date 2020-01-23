#include "NesController.h"

NesController nes;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
nes.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println("jjkjk");
  delay(100);
  
if(nes.nespad.updated==true)
{
  //nes.nespad.used=true;
  char str[30];
  sprintf(str,"%s%s%s%s%s%s%s%s", nes.nespad.a ? "A":"_",nes.nespad.b ? "B":"_",nes.nespad.up ? "U":"_",nes.nespad.down ? "D":"_",nes.nespad.left ? "L":"_",nes.nespad.right ? "R":"_",nes.nespad.select ? "SEL":"___",nes.nespad.start ? "STR":"___");
  Serial.println(str);
  nes.nespad.updated=false;

}
}
