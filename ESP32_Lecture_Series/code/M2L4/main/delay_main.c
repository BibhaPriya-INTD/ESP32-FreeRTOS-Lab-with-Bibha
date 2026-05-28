#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void vTaskENtryFunction(void* pvParameters);
void vTaskFunctiondelayuntil(void* pvParameters);
void vContinuousTask(void* pvParameters);
void vPeriodicTask(void* pvParameters);


void app_main(){
    //xTaskCreatePinnedToCore(vTaskENtryFunction, "Task1", 4096, "From Task 1", 1, NULL, 1);
    //xTaskCreatePinnedToCore(vTaskENtryFunction, "Task2", 4096, "From Task 2", 2, NULL, 1);
    //xTaskCreatePinnedToCore(vTaskFunctiondelayuntil, "Task1", 4096, "From Task 1", 1, NULL, 1);
    //xTaskCreatePinnedToCore(vTaskFunctiondelayuntil, "Task2", 4096, "From Task 2", 2, NULL, 1);
    xTaskCreatePinnedToCore(vContinuousTask, "Task1", 4096, "From Task 1", 1, NULL, 1);
    xTaskCreatePinnedToCore(vContinuousTask, "Task2", 4096, "From Task 2", 1, NULL, 1);
    xTaskCreatePinnedToCore(vPeriodicTask, "Task3", 4096, "From Task 3", 2, NULL, 1);

}

void vTaskENtryFunction(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xdelay1000ms = pdMS_TO_TICKS(1000);
    for(;;){
        vTaskDelay(xdelay1000ms);
        printf("%s\n", pcParams);
    }
}

void vTaskFunctiondelayuntil(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xdelay1000ms = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTick = xTaskGetTickCount();
    for(;;){
        vTaskDelayUntil(&xLastWakeTick ,xdelay1000ms);
        printf("%s\n", pcParams);
    }
}

void vContinuousTask(void* pvParameters){
    char* pcTaskName;
    pcTaskName = (char*)pvParameters;
    for(;;){
        for(int i =0; i<10000; i++){

        }
        printf("Continuous task %s is running\n", pcTaskName);
    }
}
void vPeriodicTask(void* pvParameters){
    char* pcParams = (char*)pvParameters;
    const TickType_t xdelay1000ms = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTick = xTaskGetTickCount();
    for(;;){
        printf("%s Periodic Task is running\n", pcParams);
        vTaskDelayUntil(&xLastWakeTick ,xdelay1000ms);
    }
}