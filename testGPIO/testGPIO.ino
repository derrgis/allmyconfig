#include "driver/gpio.h"
#include "driver/rtc_io.h"

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  // put your main code here, to run repeatedly:
gpio_config_t io_conf;
    //disable interrupt
//    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode        
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1<<12;
    //disable pull-down mode
    io_conf.pull_down_en = (gpio_pulldown_t)0;
    //disable pull-up mode
    io_conf.pull_up_en = (gpio_pullup_t)0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);
   //GPIO.enable=GPIO.enable | (1<<12);
}

void loop() {


    
    GPIO.out=GPIO.out  ^ (1<<12);
    delay(500);

}
