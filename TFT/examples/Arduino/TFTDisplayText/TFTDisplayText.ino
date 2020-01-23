/*
  Arduino TFT text example
  
  This example demonstrates how to draw text on the 
  TFT with an Arduino. The Arduino reads the value 
  of an analog sensor attached to pin A0, and writes 
  the value to the LCD screen, updating every
  quarter second.
  
  This example code is in the public domain

  Created 15 April 2013 by Scott Fitzgerald
 
  http://arduino.cc/en/Tutorial/TFTDisplayText
 
 */

#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 4
#define RST_PIN 7
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  
#define bl 3

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1 

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
char sensorPrintout[16];

void setup() {
  pinMode(bl, OUTPUT);
  analogWrite(bl,1);
   Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(0,0,255);
  // set the font size
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Sensor Value :\n ",0,0);
  // ste the font size very large for the loop
  TFTscreen.setTextSize(1);
}
String strold;
String strnew;
void loop() {

 if (  mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("jj");
  

  // Select one of the cards
  if ( mfrc522.PICC_ReadCardSerial()) {
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
 strold.toCharArray(sensorPrintout, 16);
  TFTscreen.stroke(0,0,0);
 TFTscreen.text(sensorPrintout, 0, 20);

 String strnew = String(mfrc522.uid.uidByte[0])+":"+String(mfrc522.uid.uidByte[1])+":"+String(mfrc522.uid.uidByte[2])+String(mfrc522.uid.uidByte[3]);
 strold=strnew;
 strnew.toCharArray(sensorPrintout, 16);
  
  // convert the reading to a char array
  //.toCharArray(ssensorValensorPrintout, 4);
 Serial.println("ee");
 
//  TFTscreen.text(strold, 0, 20);
  // set the font color
  TFTscreen.stroke(255,255,255);
   TFTscreen.text(sensorPrintout, 0, 20);
  //TFTscreen.text(strnew, 0, 20);
  }
 }
  // Dump debug info about the card. PICC_HaltA() is automatically called.

  // Read the value of the sensor on A0
 /*String sensorVal = String(analogRead(A0));
 
  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 4);
 
  TFTscreen.stroke(0,0,0);
//  TFTscreen.text(strold, 0, 20);
  // set the font color
  TFTscreen.stroke(255,255,255);
  // print the sensor value
  TFTscreen.text(sensorPrintout, 0, 20);
  // wait for a moment
  delay(250);*/
  // erase the text you just wrote

}

