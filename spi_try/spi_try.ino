/* SPI Master example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "SPI.h"
//#include "compet.h"

/*
 This code displays some fancy graphics on the 320x240 LCD on an ESP-WROVER_KIT board.
 It is not very fast, even when the SPI transfer itself happens at 8MHz and with DMA, because
 the rest of the code is not very optimized. Especially calculating the image line-by-line
 is inefficient; it would be quicker to send an entire screenful at once. This example does, however,
 demonstrate the use of both spi_device_transmit as well as spi_device_queue_trans/spi_device_get_trans_result
 as well as pre-transmit callbacks.
 Some info about the ILI9341/ST7789V: It has an C/D line, which is connected to a GPIO here. It expects this
 line to be low for a command and high for data. We use a pre-transmit callback here to control that
 line: every transaction has as the user-definable argument the needed state of the D/C line and just
 before the transaction is sent, the callback will set this line to the correct state.
*/

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   22

#define PIN_LATCH 2

#define SPI_DEVICE_NO_DUMMY
esp_err_t ret;
    spi_device_handle_t spi;
   spi_bus_config_t buscfg;
   
        
spi_transaction_t trans_desc;
        
 spi_device_interface_config_t devcfg;
   typedef struct {        
    spi_transaction_t   *trans; 
    uint32_t *buffer_to_send;   //equals to tx_data, if SPI_TRANS_USE_RXDATA is applied; otherwise if original buffer wasn't in DMA-capable memory, this gets the address of a temporary buffer that is;
                                //otherwise sets to the original buffer or NULL if no buffer is assigned.
    uint32_t *buffer_to_rcv;    // similar to buffer_to_send
} spi_trans_priv;


int k=0;
void lcd_cmd(spi_device_handle_t spi, const uint8_t cmd) 
{
  
   // esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=7;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    //ret=
  // spi_device_transmit(spi, &t);
   // spi_device_queue_trans(spi,&t, portMAX_DELAY);//Transmit!

   // spi_trans_priv trans_buf;
   //memset( &trans_buf, 0, sizeof(spi_trans_priv) );
//    trans_buf.trans = &t;

    //trans_buf.buffer_to_send = heap_caps_malloc((t.length+7)/8, MALLOC_CAP_DMA);
 // trans_buf.buffer_to_send=(uint32_t*)heap_caps_malloc((t.length+7)/8, MALLOC_CAP_DMA); //malloc((t.length+7)/8);
   // memcpy( trans_buf.buffer_to_send,   t.tx_buffer, ((t.length+7))/8 );
   QueueHandle_t xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
    xQueueSend(xQueue1, (void*)&cmd, portMAX_DELAY);
    
    //assert(ret==ESP_OK);            //Should have had no issues.
}
char data[4];
void setup() {
  //SPICOMMON_BUSFLAG_MASTER;
   Serial.begin(115200);
   delay(1000);
 pinMode(PIN_LATCH,OUTPUT);
Serial.printf("new max %f\n");
  // put your setup code here, to run once:
                 //We want to be able to queue 7 transactions at a time
       // .pre_cb=lcd_spi_pre_transfer_callback,  //Specify pre-transfer callback to handle D/C line
   
    //Initialize the SPI bus
//ret= spicommon_periph_claim(HSPI_HOST);
  // assert(ret==ESP_OK);
   buscfg.miso_io_num=-1;//GPIO_NUM_19;
      buscfg.mosi_io_num=GPIO_NUM_23;
       buscfg.sclk_io_num=GPIO_NUM_18;
        buscfg.quadwp_io_num=-1;
        buscfg.quadhd_io_num=-1;
        

        
        devcfg.command_bits=0;
        devcfg.address_bits=0;
        devcfg.duty_cycle_pos=0;
        devcfg.dummy_bits=0;
       // devcfg.pre_cb=lcd_spi_pre_transfer_callback;
        devcfg.clock_speed_hz=80000000;           //Clock out at 10 MHz
        devcfg.mode=0   ;                           //SPI mode 0
        devcfg.spics_io_num=5;            //CS pin
        devcfg.queue_size=1 ;
        devcfg.cs_ena_posttrans = 0;
        devcfg.cs_ena_pretrans  = 0;
        devcfg.flags=1<<6;   
        devcfg.pre_cb=NULL;
       devcfg.post_cb=NULL;



//trans_desc.address= 0;
//trans_desc.command= 0;
trans_desc.flags= SPI_TRANS_USE_TXDATA;
trans_desc.length=  1;
trans_desc.rxlength  = 0;
   trans_desc.tx_buffer = data;
   //trans_desc.rx_buffer = data;



  ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
 bool *isna;
  // ret=spicommon_bus_initialize_io(VSPI_HOST, &buscfg, 0,0,isna);
    
    assert(ret==ESP_OK);
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
assert(ret==ESP_OK);
   // gpio_set_level(GPIO_NUM_2, 0);
    //lcd_cmd(spi,0);
    //gpio_set_level(GPIO_NUM_2, 1);
 // Serial.println(spi_cal_clock(APB_CLK_FREQ, 20000000, 255, NULL));
// SPI.setFrequency(80000000000);
 //SPI.setClockDivider(-1);
// SPI.begin();
 //SPI.begin();
//Serial.printf("sck %d\n",SPI.getsck());

    
}
//#define SPI_TRANS_MODE_QIO;

void loop() {
     /*  spi_transaction_t t;
    memset(&t, 0, sizeof(t)); 
    t.length=32; */
     //gpio_set_level(GPIO_NUM_2, 0);
       //SPI. _spi->dev->mosi_dlen.usr_mosi_dbitlen =15 ;
//spi_dev_t *t=SPI._spi->dev;
//volatile uint32_t *z=t->data_buf;

       long time2=ESP.getCycleCount();
    //lcd_cmd(spi,k);
    for(int i=0;i<400;i++)
    {
   // gpio_set_level(GPIO_NUM_2, 1);
   // gpio_set_level(GPIO_NUM_2, 0);
       // SPI.transfer((uint8_t)k);
      
       /*  memset(&t, 0, sizeof(t)); 
         t.length=32;
       t.tx_buffer=&k;               //The data is the cmd itself
    //t.user=(void*)0;                //D/C needs to be set to 0
    //ret=
    spi_device_transmit(spi, &t);*/
   //SPI.transfer((uint32_t)k);
    //SPI.write((uint16_t)k);
  // SPI._spi->dev->cmd.usr = 1;
  // t->cmd.usr=1;
 // *t=1;
    //SPI._spi->dev->data_buf[0] = (uint32_t)k;
      //  *z = (volatile uint32_t)k;
    //spi->dev->miso_dlen.usr_miso_dbitlen = 0;
   //;
    spi_device_queue_trans(spi, &trans_desc,pdMS_TO_TICKS( 0));
//    xQueueSend(spi->trans_queue, (void*)&data, pdMS_TO_TICKS( 0));
    //while(t->cmd.usr); //SPI._spi->dev->cmd.usr);
      }
     long time3=ESP.getCycleCount();
    //gpio_set_level(GPIO_NUM_2, 1);
delay(200);
     printf("new max %f\n",(float)1E9*((float)(-time2+time3)/240000000/400));
    k=(k+1)%32;
    //assert(ret==ESP_OK);
  // put your main code here, to run repeatedly:

}

