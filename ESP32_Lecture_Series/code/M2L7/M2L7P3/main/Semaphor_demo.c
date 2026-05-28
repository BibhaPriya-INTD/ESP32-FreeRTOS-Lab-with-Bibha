#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"

SemaphoreHandle_t dataSem;

int sharedNumber = 0;

void producerTask(void* pvParameter){
    int num = 1;
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(1000));

        sharedNumber = num;

        printf("Producer generated a number %d\n", sharedNumber);

        xSemaphoreGive(dataSem);

        num++;
    }
}

void oddTask(void* pv){
    for(;;){
        xSemaphoreTake(dataSem, portMAX_DELAY);
        if(sharedNumber %2 == 1){
            printf("Odd Task processing %d\n", sharedNumber);
        } else {
            xSemaphoreGive(dataSem);
        }

    }
}

void evenTask(void* pv){
    for(;;){
        xSemaphoreTake(dataSem, portMAX_DELAY);
        if(sharedNumber %2 == 0){
            printf("Even Task processing %d\n", sharedNumber);
        }  else {
            xSemaphoreGive(dataSem);
        }
    }
}

void app_main(){
    dataSem = xSemaphoreCreateBinary();

    xTaskCreatePinnedToCore(producerTask, "Producer", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(evenTask, "Even Task", 2048, NULL, 1, NULL, 1);
    vTaskDelay(1);
    xTaskCreatePinnedToCore(oddTask, "Odd Task", 2048, NULL, 1, NULL, 1);
    
    
}