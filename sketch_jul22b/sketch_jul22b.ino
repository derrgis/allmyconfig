#include <SPI.h>
#include <TFT.h>
#define cs 10
#define dc 9
#define rst 8
TFT screen=TFT(cs,dc,rst);

void setup() {
  // put your setup code here, to run once:
screen.begin();
screen.background(0,0,0);
screen.stroke(255,255,255);
 screen.setTextSize(3);
  // write the text to the top left corner of the screen
  //screen.text("Yves BAZIN \n38 ans",20,20);
}
void loop() {
  // put your main code here, to run repeatedly: 
  screen.point(1,1);
  screen.point(20,20);
  screen.point(30,30);
}
