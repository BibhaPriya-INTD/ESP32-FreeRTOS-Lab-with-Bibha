#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"

SemaphoreHandle_t parkinglot;

void carTask(void* pvParameter){
    int carID = (int)pvParameter;

    for(;;){
        printf("Car %d arriving at parkinglot\n", carID);

        //Try to take the resource
        xSemaphoreTake(parkinglot, portMAX_DELAY);

        printf("Car %d Parked\n", carID);

        //Car stays in the patking lot for 5 sec
        vTaskDelay(pdMS_TO_TICKS(5000));

        printf("Car %d Leaving the Parking lot\n", carID);

        //Release teh parking lot
        xSemaphoreGive(parkinglot);

        //Wait for 3 secs before coming back
        vTaskDelay(pdMS_TO_TICKS(3000));

    }
}

void app_main(){
    parkinglot = xSemaphoreCreateCounting(3, 3);

    //Car Task
    xTaskCreatePinnedToCore(carTask, "Car1", 2048, (void*)1, 1, NULL, 1);
    xTaskCreatePinnedToCore(carTask, "Car2", 2048, (void*)2, 1, NULL, 1);
    xTaskCreatePinnedToCore(carTask, "Car3", 2048, (void*)3, 1, NULL, 1);
    xTaskCreatePinnedToCore(carTask, "Car4", 2048, (void*)4, 1, NULL, 1);
    xTaskCreatePinnedToCore(carTask, "Car5", 2048, (void*)5, 1, NULL, 1);
}