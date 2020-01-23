#include <stdio.h>


#include "timer.h"
#include "rom.h"
#include "mem.h"
#include "cpu.h"
#include "lcd.h"
#include "sdl.h"
#include "gbrom.h"

/*
static TaskHandle_t lcdtask = 0;
static TaskHandle_t userlcdtaskHandle = 0;

void lcd_cycle_task()
{
    if (userlcdtaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
        // noInterrupts();
        userlcdtaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(lcdtask);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}


void lcdcycleTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
        
         if (anim==0)
         //  int i=0;
         // displayPicNew(artnet.getframe(),0,0,48,123); //for the artnet
         memcpy(leds,artnet.getframe(),123*48*sizeof(CRGB));
         else
         memcpy(leds,Tpic,123*48*sizeof(CRGB));
         
         replaceled();
         //delay(1),
         //controller.showPixels();
      lcd_cycle();
        
        userlcdtaskHandle=0; //so we can't have two display tasks at the same time
        
    }
}

*/
void setup() {
 // xTaskCreatePinnedToCore(lcdcycleTask, "lcdcycleTask", 5000, NULL,0, &lcdtask, 0);
  // put your setup code here, to run once:
  int r = rom_init(gb_rom);

  sdl_init();

  printf("ROM OK!\n");

  mem_init2();
  printf("Mem OK!\n");

  cpu_init();
  printf("CPU OK!\n");
  lcd_init();
}

void loop() {
  // put your main code here, to run repeatedly:
    cpu_cycle();

//lcd_cycle_task();
    lcd_cycle();

    timer_cycle();
}
