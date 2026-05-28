/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

static const uint64_t delay_count = 100000;

TaskHandle_t xTask1Handler = NULL;
TaskHandle_t xTask2Handler = NULL;

void vTask1(void* pvParameters);//1
void vTask_Alternating1(void* pvParameters);
void vTask_Alternating2(void* pvParameters);

void app_main(void)
{
    //xTaskCreatePinnedToCore(vTask1, "Task1", 4096, "From Task 1", 1, NULL, 1);//1
    //xTaskCreatePinnedToCore(vTask1, "Task2", 4096, "From Task 2", 2, NULL, 1);//1
    xTaskCreatePinnedToCore(vTask_Alternating1, "Task1", 4096, "From Task 1", 2, &xTask1Handler, 1);//2
    xTaskCreatePinnedToCore(vTask_Alternating2, "Task2", 4096, "From Task 2", 1, &xTask2Handler, 1);//2
   
}

void vTask1(void* pvParameters){
    volatile uint64_t count; 
    char* task_name = (char*)pvParameters;
    for(;;){
        printf("%s\n", task_name);
        for (count = 0; count < delay_count; count++){

        }
    }
}

void vTask_Alternating1(void* pvParameters){
    UBaseType_t uxPriority;
    volatile uint64_t count;
    uxPriority = uxTaskPriorityGet(NULL);

    for(;;){
        printf("Task 1 is running\n");
        vTaskPrioritySet(xTask2Handler, (uxPriority+1));
        for (count = 0; count < delay_count; count++){

        }
    }
}

void vTask_Alternating2(void* pvParameters){
    UBaseType_t uxPriority;
    volatile uint64_t count;
    uxPriority = uxTaskPriorityGet(NULL);

    for(;;){
        printf("Task 2 is running\n");
        vTaskPrioritySet(NULL, (uxPriority-2));
        for (count = 0; count < delay_count; count++){

        }
    }

}