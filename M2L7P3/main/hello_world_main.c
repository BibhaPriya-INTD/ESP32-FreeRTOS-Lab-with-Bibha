#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t dataReadySemaphore;

int sharedData = 0;

/* Consumer Task */
void processingTask(void *pvParameters)
{
    while (1)
    {
        printf("Processing Task waiting for data...\n");

        xSemaphoreTake(dataReadySemaphore, portMAX_DELAY);

        printf("Processing Task received signal!\n");
        printf("Processing data = %d\n\n", sharedData);
    }
}

/* Producer Task */
void sensorTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        value++;
        sharedData = value;

        printf("Sensor Task produced data = %d\n", sharedData);

        xSemaphoreGive(dataReadySemaphore);
    }
}

void app_main()
{
    dataReadySemaphore = xSemaphoreCreateBinary();
    xTaskCreate(sensorTask, "Sensor Task", 2048, NULL, 5, NULL);
    xTaskCreate(processingTask, "Processing Task", 2048, NULL, 5, NULL);
}