void setup() {
  Serial.begin (115200);
  uint32_t PORT_MASK=0xffffffff;
  byte LANES=21;
  uint32_t b[21]={
    1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,8192*2,8192*4,8192*8,8192*16,8192*32,8192*64,8192*128
    };
  // put your setup code here, to run once:
  static long time3=ESP.getCycleCount();
            uint32_t nword=0;
            uint32_t maskK=PORT_MASK;
  for (uint32_t i=0;i<5000000;i++)
  {
            nword=0;
             maskK=PORT_MASK;
            //uint32_t ert=0;
            //    if(PORT_MASK>0)
          /*  for (register byte  j=0;j< LANES;j++)
            {
                if((uint32_t)(i) & (1<<j))    ///if(ert & 1) //on a une 1 a ecrire
                {
                    nword= nword |  ((maskK&(maskK-1) ) ^  maskK) ;
                   // nword= nword | b[j];
                }
                
                //ert=ert>>1;
                maskK=maskK&(maskK-1);
            }*/
            nword=( i & 0x1) + ( (i& 0b11110)<<1) + ((i & 0b1111111100000)<<3);
  }
  time3=ESP.getCycleCount()-time3;
  uint32_t n=nword;
  Serial.println(n);
  Serial.printf("time:%f\n",(float)time3/240/5000000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
