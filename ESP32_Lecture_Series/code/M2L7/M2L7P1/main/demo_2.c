#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

volatile int counter =0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void task1(void* pvParameter){
    int temp = 0;
    for(;;){
        printf("Task1 Start Update, Counter = %d\n", counter);
        vTaskSuspendAll();

        temp = counter;

        temp++;

        for(long long i = 0; i<20000000; i++){
            temp = temp;
        }

        counter = temp;
        xTaskResumeAll();

        printf("Task1 End Update, Counter = %d\n", counter);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void task2(void* pvParameter){
    for(;;){
        printf("Task2 sees counter = %d\n",counter);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void app_main(){
    xTaskCreatePinnedToCore(task1, "task1", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 1, NULL, 1);

}