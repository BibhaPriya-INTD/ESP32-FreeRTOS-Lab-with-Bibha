#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"

SemaphoreHandle_t mutex1;
SemaphoreHandle_t mutex2;

void taskA(void* pv){
    for(;;){
        printf("Task A: Trying to take mutex 1\n");
        xSemaphoreTake(mutex1, portMAX_DELAY);
        printf("Task A Takes mutex1\n");

        vTaskDelay(pdMS_TO_TICKS(100));

        printf("Task A: Trying to take mutex 2\n");
        xSemaphoreTake(mutex2, portMAX_DELAY);
        printf("Task A Takes mutex2\n");

        xSemaphoreGive(mutex2);
        xSemaphoreGive(mutex1);

        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

void taskB(void* pv){
    for(;;){
        printf("Task B: Trying to take mutex 2\n");
        xSemaphoreTake(mutex2, portMAX_DELAY);
        printf("Task B Takes mutex2\n");

        vTaskDelay(pdMS_TO_TICKS(100));//forced task switch

        printf("Task B: Trying to take mutex 1\n");
        xSemaphoreTake(mutex1, portMAX_DELAY);
        printf("Task B Takes mutex1\n");

        xSemaphoreGive(mutex1);
        xSemaphoreGive(mutex2);

        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

void app_main(){
    mutex1 = xSemaphoreCreateMutex();
    mutex2 = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskB, "Task B", 2048, NULL, 1, NULL, 1);

}