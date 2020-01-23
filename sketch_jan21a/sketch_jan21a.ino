
#include "esp32-hal-gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include "esp_attr.h"
#include "esp_intr.h"
#include "rom/gpio.h"
#include "soc/gpio_reg.h"
#include "soc/io_mux_reg.h"
#include "soc/gpio_struct.h"
#include "soc/rtc_io_reg.h"


   void gpio_task(void* pvParameter)
   {   
   gpio_config_t io_conf;
   //io_conf.intr_type = GPIO_INTR_POSEDGE;                   //enable positive edge interrupt
   io_conf.intr_type = GPIO_INTR_NEGEDGE;                   //enable positive edge interrupt
   io_conf.mode = GPIO_MODE_INPUT;                          //set as output mode
   io_conf.pin_bit_mask = GPIO_SEL_21;                  //bit mask of the pins that you want to set,e.g.GPIO18/19
   io_conf.pull_down_en = 0;                                 //disable pull-down mode
   io_conf.pull_up_en = 1;                                   //disable pull-up mode
   gpio_config(&io_conf);

   ESP_LOGI(TAG, "zb_tlink_esp_task called..\n");

   ESP_LOGI(TAG,"INPUT_GPIO into zb_tlink_esp_task :: %d\n",gpio_get_level(INPUT_BUTTON_GPIO));
   
   // Intterrupt number see below
   //gpio_isr_register(gpioCallback, INPUT_BUTTON_GPIO, gpioCallback, NULL); // 17
   gpio_isr_register(gpioCallback, NULL, 0, NULL); // 17

   ESP_LOGI(TAG, "zb_tlink_esp_task before while(1) loop..\n");
   
   while(1) {
      vTaskDelay(500 / portTICK_PERIOD_MS);
   }
   }
   
   void IRAM_ATTR gpioCallback(void* arg)
{
   gpio_config_t io_conf;
   
   //GPIO intr process
   ESP_LOGI(TAG,"in gpio_intr callback...\n");
   uint32_t gpio_num = INPUT_BUTTON_GPIO;
   uint32_t gpio_intr_status = READ_PERI_REG(GPIO_STATUS_REG);   //read status to get interrupt status for GPIO0-31
   uint32_t gpio_intr_status_h = READ_PERI_REG(GPIO_STATUS1_REG);//read status1 to get interrupt status for GPIO32-39
   SET_PERI_REG_MASK(GPIO_STATUS_W1TC_REG, gpio_intr_status);    //Clear intr for gpio0-gpio31
   SET_PERI_REG_MASK(GPIO_STATUS1_W1TC_REG, gpio_intr_status_h); //Clear intr for gpio32-39
   do {
      if(gpio_num < 32) {
         if(gpio_intr_status & BIT(gpio_num)) { //gpio0-gpio31
         
            ESP_LOGI(TAG,"creating esp_reset_ap_mode_task...\n");
            
            xTaskCreate(&esp_reset_ap_mode_task, "esp_reset_ap_mode_task", 2048, NULL, 5, &esp_reset_ap_mode_task_handler);
         }
      } else {
         if(gpio_intr_status_h & BIT(gpio_num - 32)) {
            ets_printf("Intr GPIO%d, val : %d\n",gpio_num,gpio_get_level(gpio_num));
            //This is an isr handler, you should post an event to process it in RTOS queue.
         }
      }
   } while(++gpio_num < GPIO_PIN_COUNT);
}

/******************************************************************************
 * FunctionName : esp_reset_ap_mode_task
 * Description  : 
 * Parameters   : void* pvParameter
 * Returns      : void
*******************************************************************************/
void esp_reset_ap_mode_task(void)
{
   int count = 1;
   gpio_config_t io_conf;
   
   ESP_LOGI(TAG,"Intr GPIO%d ,val: %d\n",INPUT_BUTTON_GPIO,gpio_get_level(INPUT_BUTTON_GPIO));
   
   while ((count <= 6) && (gpio_get_level(INPUT_BUTTON_GPIO) == 0))
   {
      vTaskDelay(500 / portTICK_PERIOD_MS);
      count++;
   }
   
   if ( count == 6)
   {
      ESP_LOGI(TAG, "setting AP MODE on system restart...\n");      
   }
   else
   {
      ESP_LOGI(TAG, "setting STA MODE on system restart...\n");      
   }
}
   

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
