   // This code is meant to take at most 10 inputs  (eventually 70) and display them in WS2812B individually addressed LEDs
   // 9/3/2018 : Now keeps track of what LEDs are on and how long each have been on (almost)
   // Changes needed: Find why the leds never turn off. currently the time on is not changing past 30miliseconds.
   // 
   

    #include <FastLED.h>
    #define LED_PIN     7
    #define NUM_LEDS    10
    #define BRIGHTNESS 30       // 0-64

    long previousMillis_1 = 0;
    long previousMillis_2 = 0;
    long onTime_1 = 10000; // 10 seconds
    long onTime_2 = 1000;   // 1 seconds   
    int ledStatus[] = {0,0,0,0,0,0,0,0,0,0,0};  // Each LED is assigned to the their according element. {LED1,LED2,LED3,...}
                                  // 0 will indicate that the LED is off. 1 will indicate that the LED is on
                                  // the intention for this is to let each LED have their own timing.
   
    CRGB leds[NUM_LEDS];
    long ledstart[NUM_LEDS];
    long duration[NUM_LEDS]={200,1000,500,3000,2500,10000,600,90,7000,100};
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
    void setup() {
      Serial.begin(9600);                         // Serial communication at 9600buad.
      Serial.flush();                                         
      FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS); 
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.show();                              //Initializing all LEDs to off
      }     
     


void checkleds()
{
  long now =millis();
  for(int i=0;i<NUM_LEDS;i++)
  {
    if(ledStatus[i]==1 && (now-ledstart[i])>duration[i] )
    {
      ledStatus[i]=0;
      leds[i]=CRGB::Black;
    }
  }
}
   
    void loop() {
   while (Serial.available() > 0)
   {                                                         
     
      int input = Serial.read()-48;    // Only using integers for inputs.
      Serial.println(input);




                                               
      ledStatus[input] = 1;
      leds[input] = CRGB::Red;// setting each number entered to its assigned LED
      ledstart[input]=millis();
  

      checkleds();
     
      FastLED.show();              // turn on the led that has been entered             
                                     

   

   
   }
   checkleds();
   FastLED.show();
                                 
 }



    


    

    

