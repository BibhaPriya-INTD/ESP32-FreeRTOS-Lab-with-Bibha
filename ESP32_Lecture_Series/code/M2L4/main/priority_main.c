#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

static const uint32_t delay_count = 100000;

void vTask1(void* pvParameters);
void vTask_ALter1(void* pvParameters);
void vTask_ALter2(void* pvParameters);

TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

void app_main(void)
{
    //xTaskCreatePinnedToCore(vTask1,"Task1", 4096, "From Task 1", 1, NULL, 1);
    //xTaskCreatePinnedToCore(vTask1,"Task2", 4096, "From Task 2", 2, NULL, 1);
    xTaskCreatePinnedToCore(vTask_ALter1,"Task1", 4096, "From Task 1", 2, &xTask1Handle, 1);
    xTaskCreatePinnedToCore(vTask_ALter2,"Task2", 4096, "From Task 2", 3, &xTask2Handle, 1);

}

void vTask1(void* pvParameters){
    char* task_name = (char*)pvParameters;
    volatile uint32_t count;
    for(;;){
        printf("%s\n", task_name);
        for(count = 0; count<delay_count; count++){

        }

    }
}

void vTask_ALter1(void* pvParameters){
    UBaseType_t uxPriority;
    volatile uint32_t count;
    uxPriority = uxTaskPriorityGet(NULL);
    for(;;){
        printf("Task 1 is running\n");
        vTaskPrioritySet(xTask2Handle,(uxPriority + 1));
        for(count=0; count<delay_count; count++){

        }

    }
}
void vTask_ALter2(void* pvParameters){
    UBaseType_t uxPriority;
    volatile uint32_t count;
    uxPriority = uxTaskPriorityGet(NULL);
    for(;;){
        printf("Task 2 is Running\n");
        vTaskPrioritySet(NULL, (uxPriority - 2));
        for(count=0; count<delay_count; count++){

        }
    }
}