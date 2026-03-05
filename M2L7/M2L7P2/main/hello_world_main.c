#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

volatile int counter = 0;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void task1(void *arg)
{
    int temp = 0;
    while(1)
    {
        
        printf("Task1 START update, counter=%d\n", temp);

        //vTaskSuspendAll();
        taskENTER_CRITICAL(&mux);

        temp = counter;

        temp++;
        // Very long loop to simulate long operation
        /*for(long long i = 0; i < 20000000; i++)
        {
            temp = temp; // waste time
        }*/
        vTaskDelay(1);    

        counter = temp;
        //xTaskResumeAll();
        taskEXIT_CRITICAL(&mux);
        printf("Task1 END update, counter=%d\n\n", counter);
        

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void task2(void *arg)
{
    while(1)
    {
        printf("Task2 sees counter = %d\n\n", counter);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void app_main()
{
    xTaskCreatePinnedToCore(task1, "task1", 4096, NULL, 1, NULL,1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 1, NULL,1);
}