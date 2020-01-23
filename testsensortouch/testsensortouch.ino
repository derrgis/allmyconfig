void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(27,OUTPUT);
pinMode(12,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(27,HIGH);
digitalWrite(12,HIGH);
delay(10000);
digitalWrite(27,LOW);
delay(2000);
//delay(500);
}
