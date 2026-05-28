#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"

SemaphoreHandle_t mutex1;


void taskA(void* pv){
    for(;;){
        printf("Task A: Trying to take mutex 1\n");
        //xSemaphoreTake(mutex1, portMAX_DELAY);
        xSemaphoreTakeRecursive(mutex1, portMAX_DELAY);
        printf("Task A: Takes mutex1 First time\n");

        vTaskDelay(pdMS_TO_TICKS(100));

        printf("Task A: Trying to take mutex 1\n");
        //xSemaphoreTake(mutex1, portMAX_DELAY);
        xSemaphoreTakeRecursive(mutex1, portMAX_DELAY);
        printf("Task A: Takes mutex1 Second time\n");

        //xSemaphoreGive(mutex1);
        //printf("Task A Gives back mutex1 Second time\n");
        //xSemaphoreGive(mutex1);
        // printf("Task A Gives back mutex1 First time\n");
        xSemaphoreGiveRecursive(mutex1);
        printf("Task A: Gives back mutex1 Second time\n");
        xSemaphoreGiveRecursive(mutex1);
        printf("Task A: Gives back mutex1 First time\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}


void app_main(){
    //mutex1 = xSemaphoreCreateMutex();
    mutex1 = xSemaphoreCreateRecursiveMutex();

    xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, NULL, 1);

}