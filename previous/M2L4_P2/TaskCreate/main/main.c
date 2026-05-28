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

static const int delay_count = 100000;

typedef struct {//5
    int id;
    const char* name;
} TaskParam_t;


void vTask1_entry_function(void* pvParameters);// 1
void vTask2_entry_function(void* pvParameters);// 1
void vTask1_creator_function(void* pvParameters);// 2
void vTask_entry_function(void* pvParameters);//3 Use this to create multiple tasks
void vTask_entry_with_param(void* pvParameters);//4 Pass a parameter such that the tasks are distinguishable
void vTask_entry_structure_param(void* pvParameters);//5 Pass a parameter such that the tasks are distinguishable


void app_main(){
    //xTaskCreate(vTask1_entry_function, "Task 1", 4096, NULL, 1, NULL); //1
    //xTaskCreate(vTask2_entry_function, "Task 2", 4096, NULL, 1, NULL); //1
    //xTaskCreate(vTask1_creator_function, "Task 3", 4096, NULL, 1, NULL); //2
    //xTaskCreate(vTask_entry_function, "Task 1", 4096, NULL, 1, NULL);// 3
    //xTaskCreate(vTask_entry_function, "Task 2", 4096, NULL, 1, NULL);// 3
    //static int param1 = 1;// 4
    //static int param2 = 2;// 4
    //xTaskCreate(vTask_entry_with_param, "Task 1", 4096, &param1, 1, NULL);// 4
    //xTaskCreate(vTask_entry_with_param, "Task 2", 4096, &param2, 1, NULL);// 4
    static TaskParam_t task1_param = { .id = 1, .name = "First Task" };// 5
    static TaskParam_t task2_param = { .id = 2, .name = "Second Task" };// 5
    xTaskCreate(vTask_entry_structure_param, "Task 2", 4096, &task1_param, 1, NULL);// 5
    xTaskCreate(vTask_entry_structure_param, "Task 2", 4096, &task2_param, 1, NULL);// 5
    
}

void vTask1_entry_function(void* pvParameters){
    volatile uint64_t count;
    for(;;){
        printf("Task 1 Running\n");
        for(count = 0; count<delay_count; count++){
            //The loop is just a crude implementation of the delay
            //latter we will replace with a delay function that will not keep the processor busy
        }

    }
}

void vTask2_entry_function(void* pvParameters){
    volatile uint64_t count;
    for(;;){
        printf("Task 2 Running\n");
        for(count = 0; count<delay_count; count++){
            //The loop is just a crude implementation of the delay
            //later we will replace with a delay function that will not keep the processor busy
        }

    }
}

void vTask_entry_function(void* pvParameters){
    volatile uint64_t count;
    for(;;){
        printf("Task Running\n");
        for(count = 0; count<delay_count; count++){
            //The loop is just a crude implementation of the delay
            //later we will replace with a delay function that will not keep the processor busy
        }

   
    }
}

void vTask1_creator_function(void* pvParameters){
    volatile uint64_t count;
    //Creating Task 1 from task 2 //2
    printf("Creating task 1 from task 2\n");
    xTaskCreate(vTask1_entry_function, "Task 1", 4096, NULL, 1, NULL);
    for(;;){
        printf("Task 2 Running\n");
        for(count = 0; count<delay_count; count++){
            //The loop is just a crude implementation of the delay
            //later we will replace with a delay function that will not keep the processor busy
        }

    }
}

void vTask_entry_function_with_param(void* pvParameters){
    int* task_num = (int*)pvParameters;
    volatile uint64_t count;
    for(;;){
        printf("Task %d Running\n", *task_num);
        for(count = 0; count<delay_count; count++){
            //The loop is just a crude implementation of the delay
            //later we will replace with a delay function that will not keep the processor busy
        }

    }

}

void vTask_entry_structure_param(void* pvParameters)
{
    TaskParam_t* param = (TaskParam_t*) pvParameters;   // Cast void* to struct pointer
    volatile uint64_t count;

    for(;;)
    {
        printf("Task %d Running | Name: %s\n", param->id, param->name);

        for(count = 0; count < delay_count; count++)
        {
            // crude delay
        }
    }
}