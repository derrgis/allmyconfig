char tt[]="ABCDEFRS";
char ff[]="EE3R";
int val=0;
int vari=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.println((int)&tt);
Serial.println((int)&ff);
Serial.println(tt);
__asm__( "LDS %0,0x107\n\t"
"ldi %0,0x44\n\t"
"STS 0x101,%0\n\t"

:"=r" (val):"r" (vari));
//Serial.print("value:");
Serial.println(val);
Serial.println(tt);
Serial.println((int)&tt);
}

void loop() {
  // put your main code here, to run repeatedly: 
 // Serial.println(tt);
 
  
}
