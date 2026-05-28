#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mutex1;
SemaphoreHandle_t mutex2;

// Task A: Takes mutex1 then mutex2
void taskA(void *pvParameters)
{
    while (1)
    {
        printf("Task A: Trying to take Mutex1\n");
        xSemaphoreTake(mutex1, portMAX_DELAY);
        printf("Task A: Took Mutex1\n");

        vTaskDelay(pdMS_TO_TICKS(100)); // Force overlap

        printf("Task A: Trying to take Mutex2\n");
        xSemaphoreTake(mutex2, portMAX_DELAY);
        printf("Task A: Took Mutex2\n");

        // Critical section
        printf("Task A: In critical section\n");

        xSemaphoreGive(mutex2);
        xSemaphoreGive(mutex1);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task B: Takes mutex2 then mutex1
void taskB(void *pvParameters)
{
    while (1)
    {
        printf("Task B: Trying to take Mutex2\n");
        xSemaphoreTake(mutex2, portMAX_DELAY);
        printf("Task B: Took Mutex2\n");

        vTaskDelay(pdMS_TO_TICKS(100)); // Force overlap

        printf("Task B: Trying to take Mutex1\n");
        xSemaphoreTake(mutex1, portMAX_DELAY);
        printf("Task B: Took Mutex1\n");

        // Critical section
        printf("Task B: In critical section\n");

        xSemaphoreGive(mutex1);
        xSemaphoreGive(mutex2);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    mutex1 = xSemaphoreCreateMutex();
    mutex2 = xSemaphoreCreateMutex();

    xTaskCreate(taskA, "Task A", 2048, NULL, 1, NULL);
    xTaskCreate(taskB, "Task B", 2048, NULL, 1, NULL);
}