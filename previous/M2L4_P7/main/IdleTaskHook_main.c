#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

volatile uint32_t ulCounter = 0;

void vApplicationIdleHook(void){
    ulCounter++;
    if(ulCounter%100 == 0){
        printf("%ld\n", ulCounter);
    }
}

void vTaskFunction(void* pvParameters){
    char* pcTaskName;
    const TickType_t delay_tick = pdMS_TO_TICKS(250);
    pcTaskName = (char*)pvParameters;
    for(;;){
        printf("%s --> %ld\n", pcTaskName, ulCounter);
        vTaskDelay(delay_tick);
    }

}

void app_main(void)
{
   xTaskCreatePinnedToCore(vTaskFunction, "Task 1", 4096, "From Task 1", 1, NULL, 1);
}
