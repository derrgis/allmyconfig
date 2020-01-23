
#include "NesTimer.h"

void setup() {
  // put your setup code here, to run once:
NesTimer::init();

}

void loop() {
  Serial.begin(115200);
  // put your main code here, to run repeatedly:
    //Serial.println("jjkjk");
  delay(100);
if(nespad.updated==true)
{
  Serial.println("inside");
  //nespad.used=true;
  char str[30];
  //sprintf(str,"%s%s%s%s%s%s%s%s", nespad.a ? "A":"_",nespad.b ? "B":"_",nespad.up ? "U":"_",nespad.down ? "D":"_",nespad.left ? "L":"_",nespad.right ? "R":"_",nespad.select ? "SEL":"___",nespad.start ? "STR":"___");
  /*sprintf(str,"%s%s", str,nespad.b ? "B":" ");
  sprintf(str,"%s%s", str,nespad.up ? "U":" ");
  sprintf(str,"%s%s", str,nespad.down ? "D":" ");
  sprintf(str,"%s%s", str,nespad.left ? "L":" ");
  sprintf(str,"%s%s", str,nespad.right ? "R":" ");
  sprintf(str,"%s%s", str,nespad.select ? "SEL":"   ");
  sprintf(str,"%s%s", str,nespad.start ? "STR":" ");*/
  //Serial.printf("on a :%s\n",str);
nespad.updated=false;
  // nespad.used=false;
} 

}
