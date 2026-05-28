#include <stdio.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


volatile uint32_t ulCounter = 0;

void vApplicationIdleHook(void){
    ulCounter++;
    if(ulCounter%100 == 0){
        printf("%ld -->\n", ulCounter);
    }
}

void vTaskFunction(void* pvParameters){
    char* pcTaskName;
    pcTaskName = (char*)pvParameters;
    for(;;){
        printf("%s -- > %ld\n", pcTaskName, ulCounter);
            vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void app_main(void){
    xTaskCreatePinnedToCore(vTaskFunction,"Task 1", 4096, "From Task 1", 1, NULL, 1);
}