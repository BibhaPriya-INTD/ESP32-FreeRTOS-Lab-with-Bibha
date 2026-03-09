#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t dataSem;

int sharedNumber = 0;

void producerTask(void *pv)
{
    int num = 1;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));

        sharedNumber = num;

        printf("Producer generated %d\n", sharedNumber);

        xSemaphoreGive(dataSem);

        num++;
    }
}

void oddTask(void *pv)
{
    while(1)
    {
        xSemaphoreTake(dataSem, portMAX_DELAY);

        if(sharedNumber % 2 == 1)
            printf("Odd Task processed %d\n", sharedNumber);
    }
}

void evenTask(void *pv)
{
    while(1)
    {
        xSemaphoreTake(dataSem, portMAX_DELAY);

        if(sharedNumber % 2 == 0)
            printf("Even Task processed %d\n", sharedNumber);
    }
}

void app_main()
{
    dataSem = xSemaphoreCreateBinary();

    xTaskCreate(producerTask, "Producer", 2048, NULL, 5, NULL);
    xTaskCreate(evenTask, "Even Task", 2048, NULL, 5, NULL);
    xTaskCreate(oddTask, "Odd Task", 2048, NULL, 5, NULL);
}