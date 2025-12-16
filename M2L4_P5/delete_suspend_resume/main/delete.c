
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

void TaskDel1(void* pvParameters);
void TaskDel2(void* vpParameters);

void TaskSusp1(void* pvParameters);
void TaskSusp2(void* vpParameters);

TaskHandle_t TaskSuspHandle1, TaskSuspHandle2;

void app_main(void)
{
    //xTaskCreatePinnedToCore(TaskDel1, "Task2", 4096, "NULL", 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskSusp1, "Task2", 4096, "NULL", 1, &TaskSuspHandle1, 1);
    xTaskCreatePinnedToCore(TaskSusp2, "Task2", 4096, "NULL", 1, &TaskSuspHandle2, 1);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void TaskDel1(void* pvParameters){
   vTaskDelay(pdMS_TO_TICKS(500));
   printf("From Task 1 Creating  Task Delete 2\n");
   xTaskCreatePinnedToCore(TaskDel2, "Task1", 4096, "NULL", 1, NULL, 1);
   printf("From Task 1 Deleteing  Task Delete 1\n");
   printf("------------------------\n");
   vTaskDelete(NULL);
}
void TaskDel2(void* vpParameters){
   vTaskDelay(pdMS_TO_TICKS(500));
   printf("From Task 2 Creating  Task Delete 1\n");
   xTaskCreatePinnedToCore(TaskDel1, "Task2", 4096, "NULL", 1, NULL, 1);
   printf("From Task 2 Deleteing  Task Delete 2\n");
   printf("------------------------\n");
   vTaskDelete(NULL);
   
}

void TaskSusp1(void* pvParameters){
   
   for(;;){
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("From Task 1 Resuminging  Task Suspend 2\n");
    vTaskResume(TaskSuspHandle2);
    printf("From Task 1 Suspending  Task Suspend 1\n");
    printf("------------------------\n");
    vTaskSuspend(NULL);

   }
}
void TaskSusp2(void* vpParameters){
   
   for(;;){
    vTaskDelay(pdMS_TO_TICKS(500));
    printf("From Task 2 Resuminging  Task Suspend 1\n");
    vTaskResume(TaskSuspHandle1);
    printf("From Task 2 Suspending  Task Suspend 2\n");
    printf("------------------------\n");
    vTaskSuspend(TaskSuspHandle2);
   }
   
}