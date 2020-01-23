
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <string.h>
#include "queued_i2s_parallel.h"

//Queue for dummy data used to block main loop when all buffers are full
QueueHandle_t main_data_queue;

#define bufferFrameSize 2000 //Number of 16-bit samples per buffer
#define bufferMemorySize (bufferFrameSize*2) //Becuase 2 bytes to store each 16-bit sample

//#define showDebugPulse

uint16_t *bufferToFill; //Pointer to buffer that is next to be filled

//This gets called from the I2S interrupt. (This only removes dummy data from queue to unblock main loop)
void IRAM_ATTR buffer_filler_fn(void *buf, int len, void *arg) {
  Serial.println("fuck");
  QueueHandle_t queueHanle=(QueueHandle_t)arg;
  portBASE_TYPE high_priority_task_awoken = 0;
//Serial.println(len);
  
  uint8_t dummy[1];

  //Check if theres a dummy byte in the queue, indecating the main loop has updated the buffer, if not set to 0
  if (xQueueReceiveFromISR(queueHanle, (void*)&dummy, &high_priority_task_awoken)==pdFALSE) {
    //memset(buf, 0, bufferFrameSize*2); //Nothing in queue. Zero out data
  }

  //Wake thread blocking the queue
  if (high_priority_task_awoken == pdTRUE) { //Check if a context switch needs to be requested
    portYIELD_FROM_ISR();
  }
}

void maininit() {
  bufferToFill=(uint16_t*)malloc(bufferFrameSize*sizeof(uint16_t)) ;
  memset(bufferToFill, 0, bufferFrameSize*2);
  //Create data queue
  main_data_queue=xQueueCreate(1, 1);//bufferFrameSize*2
  //Initialize I2S parallel device.
  i2s_parallel_config_t i2scfg={
    {12,2,4,5,0,13,14,15,16,18,19,21,22,23,25,26

    },
    3333333, //3.33 MHz
    I2S_PARALLEL_BITS_16,
    bufferFrameSize*2,
    buffer_filler_fn, //Function Called by I2S interrupt
    main_data_queue //Queue pointer
  };
  i2s_parallel_setup(&I2S0, &i2scfg);
    Serial.println("hhkke");
  i2s_parallel_start(&I2S0);
  Serial.println("hhkke");
  
}

void writeSample(uint16_t *buf, uint16_t data, uint16_t pos){
  buf[pos]=data;
}

void empty(uint16_t *buff)
{
  memset(buff,0,2000);
  for (int i=0;i<11;i++)
  {
     writeColor(buff, i, 0,0,0 );
  }
}



void writeColor3bid(uint16_t *buf, int x, uint16_t r, uint16_t g, uint16_t b)
{
  uint16_t pos=(x+1)*72;//96 bit per color 4*24
 // Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint8_t l=b&1;
    b=b>>1;
    //writeSample(buf,1,pos-3);
   // buf[pos-6]=1*3;
    buf[pos]=1;
   
      buf[pos-2]=0;
       buf[pos-1]=l*3;
  
    pos=pos-3;
  }
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint8_t l=r&1;
    r=r>>1;
    buf[pos]=1;
   buf[pos-2]=0;
       buf[pos-1]=l*3;
  
    pos=pos-3;
  }
  int8_t dummy[1];
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint8_t l=g&1;
    g=g>>1;
     
    buf[pos]=1;
   
     buf[pos-2]=0;
       buf[pos-1]=l*3;
  
    pos=pos-3;
  }
 // Serial.println(pos);
}



void writeColor(uint16_t *buf, int x, uint16_t r, uint16_t g, uint16_t b)
{
  //writeColor3bid(buf,x,r,g,b);
  //return;
  uint16_t pos=(x+1)*96-1;//96 bit per color 4*24
  //Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    int l=b&1;
    b=b>>1;
    //writeSample(buf,1,pos-3);
    buf[pos-3]=1;
    buf[pos]=0;
    
      buf[pos-2]=l;
       buf[pos-1]=l;
    
    pos=pos-4;
  }
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint16_t l=r&1;
    r=r>>1;
    buf[pos-3]=1;
    buf[pos]=0;
    
      buf[pos-2]=l;
       buf[pos-1]=l;
   
    pos=pos-4;
  }
  int8_t dummy[1];
//Serial.println(pos);
  for(int i=0;i<8;i++)
  {
    uint16_t l=g&1;
    g=g>>1;
     
    buf[pos-3]=1;
    buf[pos]=0;
    
      buf[pos-2]=l;
       buf[pos-1]=l;
    
    pos=pos-4;
  }
  //Serial.println(pos)
}

void mainlp(void *arg) {
  while(1) {
    uint8_t dummy[1];
    //for (int i=0;i<54;i++)
    //{
   // if (i%bufferFrameSize == 0)
 /*   xQueueSend(main_data_queue, dummy, portMAX_DELAY);
    if (bufferToFill!=NULL){
   //Blocked when queue is full
    //}
   // writeColor(bufferToFill, 0, 0, 0,255);
     //writeColor(bufferToFill, 1, 0, 0,255);
 // Serial.println(bufferFrameSize);
   /* for (uint16_t i=0;i<=4*24;i++){
      //Send the data to the queue becuase we've filled the buffer
      if (i%bufferFrameSize == 0){
      

        
       
        //Serial.println(i);
       // xQueueSend(main_data_queue, dummy, portMAX_DELAY); //Blocked when queue is full
      }
      //Fill the buffer here
    if (bufferToFill!=NULL){
        if(i%4==0)
           writeSample(bufferToFill, 1, i%bufferFrameSize);
           else
           {
        if((i+1)%4==0)
          writeSample(bufferToFill, 0, i%bufferFrameSize);
          else
          if(i>=64 and i<96)
            writeSample(bufferToFill, 1, i%bufferFrameSize);
            else
            
             writeSample(bufferToFill, 0, i%bufferFrameSize);
           }
            
      }
    }
    empty(bufferToFill);
   writeColor(bufferToFill, 0, 255, 0,0);
    writeColor(bufferToFill, 1, 0, 0,0);
    writeColor(bufferToFill, 2, 0,255,0);
     writeColor(bufferToFill, 3, 32,0,0);
      writeColor(bufferToFill, 4, 255,0,255);
      //writeColor(bufferToFill, 5, 0,255,255);
      
    }*/

for (uint16_t i=2*96;i<=3*96;i++){
      //Send the data to the queue becuase we've filled the buffer
      if (i%bufferFrameSize == 0){
      

        
       

        xQueueSend(main_data_queue, dummy, portMAX_DELAY); //Blocked when queue is full
      }
      //Fill the buffer here
      if (bufferToFill!=NULL){
        if(i%4==0)
           writeSample(bufferToFill, 1, i%bufferFrameSize);
           else
           {
        if((i+1)%4==0)
          writeSample(bufferToFill, 0, i%bufferFrameSize);
          else
          if(i>=64+2*96 and i<96+2*96)
            writeSample(bufferToFill, 1, i%bufferFrameSize);
            else
            
             writeSample(bufferToFill, 0, i%bufferFrameSize);
           }
            
      }
    }


    
  }
}

void setup() {
  

   Serial.begin(115200);
 Serial.println("hhe");
 //mainlp();
  maininit();

  xTaskCreatePinnedToCore(mainlp, "mainlp", 1024*16, NULL, 7, NULL, 1); //Use core 1 for main loop, I2S interrupt on core 0

}

void loop()
{

}

