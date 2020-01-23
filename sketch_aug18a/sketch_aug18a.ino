

uint32_t  m2(uint32_t values,uint32_t mask,uint32_t lanes)
{
    uint32_t nword3=0;
            uint32_t maskK=mask;
            //uint32_t ert=0;
            //    if(PORT_MASK>0)
            for (register byte  j=0;j< lanes;j++)
            {
              uint32_t h=maskK&(maskK-1);
                if(!(values & 1))    ///if(ert & 1) //on a une 1 a ecrire
                {
                    nword3+=maskK&(~(maskK-1));  //((maskK&(maskK-1) ) ^  maskK) ;
                   // nword= nword | b[j];
                }
                
                //ert=ert>>1;
                maskK=maskK&(maskK-1);
                values=values>>1;
            }
     return nword3;
}
//a^b= a_.b+a.b_
//(m&(m-1))^m= m(m(m-1))_+m_.(m(m-1))=m(m_+(m-1)_)

/*
void m(uint32_t value,uint32_t mask,uint32_t lanes,uint32_t & nword)
{
 // uint32_t nword=0;
  __asm__ __volatile__(
// "xor a6,a6,a6 \n"
"mov a6,%3 \n"
//"xor a7,a7,a7 \n"
"movi a7,0x1 \n"
// "xor a10,a10,a10 \n"
 "xor a10,a10,a10 \n"
 "mov a8,%2 \n" //value
 "mov a11,%1 \n" //le mask
 "loop: \n"
"and a9,a8,a7 \n" //if
 "BNEZ a9,suite\n" //"BNEI a9,0x0,suite \n"
//"sub a9,a11,a7 \n"
"addi a9,a11,-1 \n"
"and a9,a11,a9 \n"
 "xor a9,a11,a9 \n"
"or a10,a10,a9 \n"


"suite:\n"
"SRAI a8,a8,1 \n"
//"sub a9,a11,a7 \n" //mask =mask-1
"addi a9,a11,-1 \n"
"and a11,a11,a9 \n" //mask =mask&mask
 "addi a6,a6,-1 \n" //"sub a6,a6,a7 \n"
 "BNEZ a6,loop \n"//"BNEI a6,0x0,loop \n"
"mov %0,a10 \n"

 :  "=a" (nword): "a" (mask),"a" (value), "a" (lanes)
  );
//  return nword;
}*/



void m(uint32_t value,uint32_t mask,uint32_t lanes,uint32_t & nword)
{
 // uint32_t nword=0;
  __asm__ __volatile__(
// "xor a6,a6,a6 \n"
"mov a6,%3 \n"
//"xor a7,a7,a7 \n"
"movi a7,0x1 \n"
// "xor a10,a10,a10 \n"
 "xor a10,a10,a10 \n"
 "mov a8,%2 \n" //value
 "mov a11,%1 \n" //le mask
 "loop: \n"
"xor a12,a8,a7 \n" //if
 //"BNEZ a9,suite\n" //"BNEI a9,0x0,suite \n"
//"sub a9,a11,a7 \n"
"addi a9,a11,-1 \n"
"and a11,a11,a9 \n"
"addi a9,a9,1 \n"
 "xor a9,a11,a9 \n"
 "and a9,a9,a12 \n"
"or a10,a10,a9 \n"



"suite:\n"
"SLLI a7,a7,1 \n"
//"sub a9,a11,a7 \n" //mask =mask-1
//"addi a9,a11,-1 \n"
//"and a11,a11,a9 \n" //mask =mask&mask
 "addi a6,a6,-1 \n" //"sub a6,a6,a7 \n"
 "BNEZ a6,loop \n"//"BNEI a6,0x0,loop \n"
"mov %0,a10 \n"

 :  "=a" (nword): "a" (mask),"a" (value), "a" (lanes)
  );
//  return nword;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
uint32_t a;
uint32_t b;
uint32_t c;
c=0;
a=2;
b=156;
 uint32_t nword=0;
 uint32_t nword2=0;


 Serial.printf("\na:%d b:%d c:%d\n",a,b,c);
  uint8_t theta=0;
uint8_t offset = theta;

uint32_t ff[3];
ff[0]=23;
ff[1]=45;
ff[2]=194;
nword++;
nword2++;
 //Serial.printf("nword:%ld\n",nword);
 //Serial.printf("nword2:%u\n",nword2);
 b=0b0100010;
 /*
__asm__ __volatile__(
  "mov a7,%1 \n"
  //"mov a6,%0 \n"
  "L32I a6,a7,0 \n"
  //"mov %0,a6 \n"
    "movi a7,0x4 \n"
   "add a7,%1,a7 \n"
   //"mov a6,%0 \n"
  "L32I a6,a7,0 \n"
"mov %0,a6 \n"

 // "mov a6,%1 \n"

 :  "=a" (nword): "a" (ff)
  );*/
uint32_t PORT_MASK =0xffffffff;
uint32_t value=0x102273;
uint32_t LANES=21;
uint32_t NB=500000;
static long time3=ESP.getCycleCount();
    time3=ESP.getCycleCount();
    time3++;
  time3=ESP.getCycleCount();
for(int i=0;i<NB;i++)
{
          m(value,PORT_MASK,LANES,nword);
}
  time3=ESP.getCycleCount()-time3;

Serial.printf("value:%ld nword asm:%ld time:%f\n",value,nword,(float)time3/240/NB);

  time3=ESP.getCycleCount();
for(int i=0;i<NB;i++)
{
 nword= m2(value,PORT_MASK,LANES);
}         
  time3=ESP.getCycleCount()-time3;

  Serial.printf("value:%ld nword:%ld time:%f\n", value,nword,(float)time3/240/NB);
//Serial.println(nword);
   /* asm volatile(
                 "mul %[m16],%[secoffset]   \n\t"
                 "mov %[mx],r0              \n\t"
                 "mov %[xr1],r1             \n\t"
                 "eor  r1, r1               \n\t"
                 "swap %[mx]                \n\t"
                 "andi %[mx],0x0F           \n\t"
                 "swap %[xr1]               \n\t"
                 "andi %[xr1], 0xF0         \n\t"
                 "or   %[mx], %[xr1]        \n\t"
                 : [mx] "=d" (mx), [xr1] "=d" (xr1)
                 : [m16] "d" (m16), [secoffset] "d" (secoffset)
                 );*/
   Serial.println("ee");
                 
}

void loop() {
  // put your main code here, to run repeatedly:

}
