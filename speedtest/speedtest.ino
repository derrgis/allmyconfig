#define LANES 5
//#include "p1.h"
//#include "p2.h"
#define PORT_MASK 0b10010000000
#define PH2        0b10000000000000000000
#define PH (  (LANES)>16 ? (PH2) :(0) )
//#define PH (PORT_MASK & 0xFFFF0000)
#define M1 (( ( PORT_MASK&(PORT_MASK-1) ) ^  PORT_MASK))
#define T1 (  PORT_MASK&(PORT_MASK-1) )
#define T2 (T1&(T1-1))
#define T3 (T2&(T2-1))
#define T4 (T3&(T3-1))
#define T5 (T4&(T4-1))
#define T6 (T5&(T5-1))
#define T7 (T6&(T6-1))
#define T8 (T7&(T7-1))
#define T9 (T8&(T8-1))
#define T10 (T9&(T9-1))
#define T11 (T10&(T10-1))
#define T12 (T11&(T11-1))
#define T13 (T12&(T12-1))
#define T14 (T13&(T13-1))
#define T15 (T14&(T14-1))
#define T16 (T15&(T15-1))
/*#define T17 (PH&(PH-1))
#define T18 (T17&(T17-1))
#define T19 (T18&(T18-1))
#define T20 (T19&(T19-1))
#define T21 (T20&(T20-1))
#define T22 (T21&(T21-1))
#define T23 (T22&(T22-1))
#define T24 (T23&(T23-1))
#define T25 (T24&(T24-1))
#define T26 (T25&(T25-1))
#define T27 (T26&(T26-1))
#define T28 (T27&(T27-1))
#define T29 (T28&(T28-1))
#define T30 (T29&(T29-1))
#define T31 (T30&(T30-1))
#define T32 (T31&(T31-1))*/
#define M2 (T1&(-T1))
#define M3 (T2&(-T2))
#define M4 (T3&(-T3))
#define M5 (T4&(-T4))
#define M6 (T5&(-T5))
#define M7 (T6&(-T6))
#define M8 (T7&(-T7))
#define M9 (T8&(-T8))
#define M10 (T9&(-T9))
#define M11 (T10&(-T10))
#define M12 (T11&(-T11))
#define M13 (T12&(-T12))
#define M14 (T13&(-T13))
#define M15 (T14&(-T14))
#define M16 (T15&(-T15))
/*#define M17 (PH&(-PH))
#define M18 (T17&(-T17))
#define M19 (T18&(-T18))
#define M20 (T19&(-T19))
#define M21 (T20&(-T20))
#define M22 (T21&(-T21))
#define M23 (T22&(-T22))
#define M24 (T23&(-T23))
#define M25 (T24&(-T24))
#define M26 (T25&(-T25))
#define M27 (T26&(-T26))
#define M28 (T27&(-T27))
#define M29 (T28&(-T28))
#define M30 (T29&(-T29))
#define M31 (T30&(-T30))
#define M32 (T31&(-T31))*/

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.printf("%ld",M3);

}
__attribute__ ((always_inline)) inline static uint32_t __clock_cycles() {
    uint32_t cyc;
    __asm__ __volatile__ ("rsr %0,ccount":"=a" (cyc));
    return cyc;
}
uint32_t t1,t2,t3,val;
void loop() {
  uint32_t nword,n2,n3,maskK;

val++;
  nword=0;
  n2=0;
  n3=0;
  maskK=PORT_MASK;
  
  t1= __clock_cycles();
  // uint32_tput your main code here, to run repeatedly:
for ( short j=0;j< LANES;j++)
        {
            if( (uint32_t) (val & (1<<j)) )   ///if(ert & 1) //on a une 1 a ecrire
            {
                nword= nword |  ((maskK&(maskK-1) ) ^  maskK) ;
                
            }
            
            //ert=ert>>1;
            maskK=maskK&(maskK-1);
            //Serial.print("23");
        }
     t2=__clock_cycles()-t1;
     t1=__clock_cycles();
     // n2= (~( ((val>>0 )&1) -1))&M1 |  (~( ((val>>1 )&1) -1))&M2 |  (~( ((val>>2 )&1) -1))&M3  |  (~( ((val>>3 )&1) -1))&M4 |  (~( ((val>>4 )&1) -1))&M5 |  (~( ((val>>5 )&1) -1))&M6 |  (~( ((val>>6 )&1) -1))&M7;
  n2= (~( ((val>>0 )&1) -1))&M1 | (~( ((val>>1 )&1) -1))&M2 | (~( ((val>>2 )&1) -1))&M3 | (~( ((val>>3 )&1) -1))&M4 | (~( ((val>>4 )&1) -1))&M5 | (~( ((val>>5 )&1) -1))&M6 | (~( ((val>>6 )&1) -1))&M7 | (~( ((val>>7 )&1) -1))&M8  | (~( ((val>>8 )&1) -1))&M9 | (~( ((val>>9 )&1) -1))&M10 | (~( ((val>>10 )&1) -1))&M11 | (~( ((val>>11 )&1) -1))&M12 | (~( ((val>>12 )&1) -1))&M13 | (~( ((val>>13 )&1) -1))&M14 | (~( ((val>>14 )&1) -1))&M15 | (~( ((val>>15 )&1) -1))&M16 ;
//n2= n2  | (~( ((val>>16 )&1) -1))&M17 | (~( ((val>>17 )&1) -1))&M18 | (~( ((val>>18 )&1) -1))&M19 | (~( ((val>>19 )&1) -1))&M20;// | (~( ((val>>20 )&1) -1))&M21 | (~( ((val>>21 )&1) -1))&M22 | (~( ((val>>22 )&1) -1))&M23 | (~( ((val>>23 )&1) -1))&M24 ;
//n2= n2  | (~( ((val>>24 )&1) -1))&M25 | (~( ((val>>25 )&1) -1))&M26 | (~( ((val>>26 )&1) -1))&M27 | (~( ((val>>27 )&1) -1))&M28 | (~( ((val>>28 )&1) -1))&M29 | (~( ((val>>29 )&1) -1))&M30 | (~( ((val>>30 )&1) -1))&M31 | (~( ((val>>31 )&1) -1))&M32 ;     
      //n2=n2|n3;
      t3=__clock_cycles()-t1;
      //Serial.printf("23:%ld",M23);
      //Serial.printf("24:%ld",M24);
        Serial.printf("\ntemps:%ld .vs  %ld\n", t3,t2);
        Serial.printf( "resultat:%ld vs %ld vs %ld\n",n2,nword,~(((val>>1 )&1)-1));
        
        //Serial.print(nword);
           //t2=__clock_cycles();
       // 250*
        delay(200);
}
