#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"


void TaskSusp1(void* pvParameters);
void TaskSusp2(void* pvParameters);

TaskHandle_t taskhandle1 = NULL;
TaskHandle_t taskhandle2 = NULL;

void app_main(){
    xTaskCreatePinnedToCore(TaskSusp1, "Task1", 4096, "NULL", 1, &taskhandle1, 1);
    xTaskCreatePinnedToCore(TaskSusp2, "Task2", 4096, "NULL", 1, &taskhandle2, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void TaskSusp1(void* pvParameters){
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("From Task 1 Resume Task 2\n");
        vTaskResume(taskhandle2);
        printf("From Task 1 Suspend Task 1\n");
        printf("..........................\n");
        vTaskSuspend(taskhandle1);
    }
}
void TaskSusp2(void* pvParameters){
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("From Task 2 Resume Task 1\n");
        vTaskResume(taskhandle1);
        printf("From Task 2 Suspend Task 2\n");
        printf("..........................\n");
        vTaskSuspend(NULL);
    }
}


