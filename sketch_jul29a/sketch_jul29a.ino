void setup() {
  // put your setup code here, to run once:
uint32_t d[2]={
  0x01234567,0x89ABCDEF
};
Serial.begin(115200);

for (int i=0;i<8;i++)
  Serial.printf("%d\n",(char)*((char *)&d[0]+i));
  
}



void loop() {
  // put your main code here, to run repeatedly:

}
