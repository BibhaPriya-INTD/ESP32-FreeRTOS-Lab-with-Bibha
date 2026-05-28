#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int globalCounter = 0;   // ⚠ Shared by all tasks

void incrementCounter(void)
{
    int temp;

    temp = globalCounter;     // Step 1: Read
    temp = temp + 1;          // Step 2: Modify
    //vTaskDelay(1);            // Force context switch here!
    globalCounter = temp;     // Step 3: Write back
}

void Task1(void *pvParameters)
{
    while(1)
    {
        incrementCounter();
        printf("Task1: %d\n", globalCounter);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void Task2(void *pvParameters)
{
    while(1)
    {
        incrementCounter();
        printf("Task2: %d\n", globalCounter);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main()
{
    xTaskCreate(Task1, "Task1", 2048, NULL, 1, NULL);
    xTaskCreate(Task2, "Task2", 2048, NULL, 1, NULL);
}