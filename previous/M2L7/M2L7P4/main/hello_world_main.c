#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t parkingLot;

void carTask(void *pvParameters)
{
    int carID = (int) pvParameters;

    while(1)
    {
        printf("Car %d arriving at parking lot\n", carID);

        // Try to take a parking slot
        xSemaphoreTake(parkingLot, portMAX_DELAY);

        printf("Car %d parked\n", carID);

        // Car stays parked for some time
        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("Car %d leaving parking lot\n", carID);

        // Release parking slot
        xSemaphoreGive(parkingLot);

        // Wait before coming again
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main()
{
    // Create counting semaphore
    // Max spaces = 3
    // Initial available spaces = 3
    parkingLot = xSemaphoreCreateCounting(3, 3);

    // Create 5 car tasks
    xTaskCreate(carTask, "Car1", 2048, (void*)1, 5, NULL);
    xTaskCreate(carTask, "Car2", 2048, (void*)2, 5, NULL);
    xTaskCreate(carTask, "Car3", 2048, (void*)3, 5, NULL);
    xTaskCreate(carTask, "Car4", 2048, (void*)4, 5, NULL);
    xTaskCreate(carTask, "Car5", 2048, (void*)5, 5, NULL);
}