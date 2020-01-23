void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
char datestring[20];
sprintf(datestring,"%02u:%02u:%02u %02u/%02u/%04u",          //without this line there is no failure but yes of corse also no displaying
            1,
            2,
            3,
            32, 
            12,
            2018);
Serial.printf(datestring);
}

void loop() {
  // put your main code here, to run repeatedly:

}
