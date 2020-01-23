
 void fu(uint32_t *f,uint32_t g,uint32_t *h)
{
   __asm__ __volatile__ (
    "l32i.n a11,a2,0;"
    "add a11,a11,a3;"
     "add a3,a11,a3;"
    "l32i.n a11,a4,0;"
     "add a11,a11,a3;"
    "s32i.n a11, a2, 0;"
    );
}

void fvddd(uint32_t *f,uint32_t g,uint32_t *h)
{
  *f=*f+2*g+*h;
}
void transpose8rS32(uint8_t * A, int m, int n, uint8_t * B)
    {
        uint32_t x, y, t;
        
        // Load the array and pack it into x and y.
        
       // x = (A[7]<<24)   | (A[6]<<16)   | (A[5]<<8) | A[4];//(A[0]<<24)   | (A[m]<<16)   | (A[2*m]<<8) | A[3*m];
        //y = *(uint16_t*)(A);//(A[3]<<12) | (A[2]<<8) | (A[1]<<4) | A[0];//(A[4*m]<<24) | (A[5*m]<<16) | (A[6*m]<<8) | A[7*m];
        
        
        x = (A[0]<<24)   | (A[m]<<16)   | (A[2*m]<<8) | A[3*m];
        y = (A[4*m]<<24) | (A[5*m]<<16) | (A[6*m]<<8) | A[7*m];
        
        t = (x ^ (x >> 7)) & 0x00AA00AA;  x = x ^ t ^ (t << 7);
       t = (y ^ (y >> 7)) & 0x00AA00AA;  y = y ^ t ^ (t << 7);
        
        t = (x ^ (x >>14)) & 0x0000CCCC;  x = x ^ t ^ (t <<14);
        t = (y ^ (y >>14)) & 0x0000CCCC;  y = y ^ t ^ (t <<14);
        
        t = (x & 0xF0F0F0F0) | ((y >> 4) & 0x0F0F0F0F);
        y = ((x << 4) & 0xF0F0F0F0) | (y & 0x0F0F0F0F);
        x = t;
        
        B[0]=x>>24;    B[n]=x>>16;    B[2*n]=x>>8;  B[3*n]=x;
        B[4*n]=y>>24;  B[5*n]=y>>16;  B[6*n]=y>>8;  B[7*n]=y;
    }
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  uint32_t AB[4];
  uint32_t ABC[18];
  uint32_t d;
  uint8_t A[20];
  uint8_t B[20];
  uint32_t f;
  AB[0]=10;
  A[0]=255;
  A[1]=255;
  A[2]=255;
  d=10;
  f=100;
  Serial.printf("%d   %d\n",AB[0],d);
  uint32_t num=1;
 long time1=ESP.getCycleCount();
  for(uint32_t i=0;i<num;i++)
  {
   fu(AB,i,&f);
 // fv(&d,34);
  //Serial.printf("%d   %d\n",AB[0],d);
 //Serial.println(AB[0]);
  }
long time2=ESP.getCycleCount();


    for(uint32_t i=0;i<num;i++)
  {
   // fu(AB,34);
  fvddd(&d,i,&f);
  //Serial.printf("%d   %d\n",AB[0],d);
 //Serial.println(AB[0]);
  }
  //transpose8rS32(AB,1,4,ABC);
long time3=ESP.getCycleCount();  
Serial.printf("fv:%f fu:%f\n",(float)(time2-time1)/num,(float)(time3-time2)/num);



Serial.printf("%d   %d\n",AB[0],d);

long time4=ESP.getCycleCount();
    for(uint32_t i=0;i<num;i++)
  {
transpose8rS32(A,  1,  1, B);

  }
  long time5=ESP.getCycleCount();
 Serial.printf("fv:%f \n",(float)(time5-time4)/num); 

}

void loop() {
  // put your main code here, to run repeatedly:

}
