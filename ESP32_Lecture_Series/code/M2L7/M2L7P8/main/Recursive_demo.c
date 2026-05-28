#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"

SemaphoreHandle_t mutex1;

void taskA(void* pv){
    for(;;){
        printf("Task A: Trying to take Mutex1\n");
        xSemaphoreTakeRecursive(mutex1, portMAX_DELAY);
        printf("Task A: Takes First Mutex1\n");
        vTaskDelay(pdMS_TO_TICKS(100));

        printf("Task A: Trying to take Mutex1\n");
        xSemaphoreTakeRecursive(mutex1, portMAX_DELAY);
        printf("Task A: Takes Second Mutex1\n");

        xSemaphoreGiveRecursive(mutex1);
        printf("Task A: Gives mutex back First time\n");
        xSemaphoreGiveRecursive(mutex1);
        printf("Task A: Gives mutex back Second time\n");
    }
    
}

void app_main(){
    mutex1 = xSemaphoreCreateRecursiveMutex();

    xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, NULL, 1);
}