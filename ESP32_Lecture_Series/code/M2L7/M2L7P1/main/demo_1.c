#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>

int globalCounter = 0; //shared resource

void incrementCounter(void){
    int temp;
    //Step 1 Read
    temp = globalCounter;
    //MOdify
    temp++;
    vTaskDelay(1); //Forced Context Switch Here
    //Write back
    globalCounter = temp;
}

void TaskA(void* pvParameter){
    for(;;){
        incrementCounter();
        printf("Task A: %d\n", globalCounter);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void TaskB(void* pvParameter){
    for(;;){
        incrementCounter();
        printf("Task B: %d\n", globalCounter);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(){
    xTaskCreatePinnedToCore(TaskA, "TaskA", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskB, "TaskB", 2048, NULL, 1, NULL, 1);
}