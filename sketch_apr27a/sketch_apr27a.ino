void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(33, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.printf("value:%d\n",analogRead(33));
}
