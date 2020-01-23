void setup() {
  // put your setup code here, to run once:
pinMode(18,INPUT);
pinMode(17,INPUT);
pinMode(05,INPUT);
Serial.begin(115200);
}

void loop() {
  Serial.println(digitalRead(18));
  if(digitalRead(18)==HIGH)
  {
    Serial.println("on a appuyer sur 18");
  }
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(05));
  if(digitalRead(05)==HIGH)
  {
    Serial.println("on a appuyer sur 05");
  }
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(17));
  if(digitalRead(17)==HIGH)
  {
    Serial.println("on a appuyer sur 17");
  }
  // put your main code here, to run repeatedly:

}
