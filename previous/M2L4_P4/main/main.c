#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

void vTaskFunction(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xDelay1s = pdMS_TO_TICKS(100);
    for(;;){
        vTaskDelay(xDelay1s);
        printf("%s, Delayed for %ld ticks\n", pcParams, xDelay1s);
    }
}

void vTaskFunctionDelayuntil(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xDelay1s = pdMS_TO_TICKS(10000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;){
        vTaskDelayUntil(&xLastWakeTime, xDelay1s);
        printf("%s, Delayed for %ld ticks\n", pcParams, xDelay1s);
    }
}

void vContinuousProcessingTask(void* pvParameters){
    char* pcTaskName;
    pcTaskName = (char* )pvParameters;
    for(;;){
        for(int i = 0; i<10000; i++){

        }
        printf("Continuous task %s is running\n", pcTaskName);
    }
}

void vPeriodicTask(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xDelay1s = pdMS_TO_TICKS(10);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for(;;){
        printf("%s, Periodic task is running\n", pcParams);
        vTaskDelayUntil(&xLastWakeTime, xDelay1s);
    }
}

void app_main(void)
{
    //xTaskCreatePinnedToCore(vTaskFunctionDelayuntil, "Task1", 4096, "From Task 1", 1, NULL, 1);
    //xTaskCreatePinnedToCore(vTaskFunctionDelayuntil, "Task2", 4096, "From Task 2", 1, NULL, 1);
    xTaskCreatePinnedToCore(vContinuousProcessingTask, "Task1", 4096, "From Task 1", 1, NULL, 1);
    xTaskCreatePinnedToCore(vContinuousProcessingTask, "Task2", 4096, "From Task 2", 1, NULL, 1);
    xTaskCreatePinnedToCore(vPeriodicTask, "Task3", 4096, "From Task 3", 2, NULL, 1);
    
}
