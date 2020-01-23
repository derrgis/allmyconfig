char * yves;
char * m;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(ESP.getFreeHeap());
  yves=(char *) malloc(112000);
  Serial.println(ESP.getFreeHeap());
  m=(char *) malloc(88000);
  Serial.println(ESP.getFreeHeap());
  free(yves);
   Serial.println(ESP.getFreeHeap()); 
  free(m);
   Serial.println(ESP.getFreeHeap());   
}
 
void loop() {}
