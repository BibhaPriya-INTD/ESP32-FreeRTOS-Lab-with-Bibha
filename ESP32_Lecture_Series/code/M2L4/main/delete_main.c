#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

void TaskDel1(void* pvParameters);
void TaskDel2(void* pvParameters);

void app_main(){
    xTaskCreatePinnedToCore(TaskDel1, "Task1", 4096, "NULL", 1, NULL, 1);
}

void TaskDel1(void* pvParameters){
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("From Task 1 creating Task 2..........\n");
    xTaskCreatePinnedToCore(TaskDel2, "Task2", 4096, NULL, 1, NULL, 1);
    printf("From Task 1 deleting Task 1..........\n");
    printf(".....................................\n");
    vTaskDelete(NULL);
}
void TaskDel2(void* pvParameters){
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("From Task 2 creating Task 1..........\n");
    xTaskCreatePinnedToCore(TaskDel1, "Task1", 4096, NULL, 1, NULL, 1);
    printf("From Task 2 deleting Task 2..........\n");
    printf(".....................................\n");
    vTaskDelete(NULL);
}