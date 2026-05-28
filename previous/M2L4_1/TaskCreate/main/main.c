#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char* TAG ="TASK CREATE";

//----------------------------------
//UTILITY TO PRINT
//----------------------------------
void print_task(const char* name){
    printf("%s Running on core %d\n", name, xPortGetCoreID());
}

void task_entry_function(void* parameters){
    while(1){
        print_task(pcTaskGetName(NULL));
        vTaskDelay(pdMS_TO_TICKS(5000));
        for(volatile int i =0; i<20; i++){
            printf("%d", i);
        }
    }
    vTaskDelete(NULL);
}


void app_main(void){
    xTaskCreate(task_entry_function,
        "Task 1",
        4096,
        NULL,
        5,
        NULL);
    xTaskCreatePinnedToCore(task_entry_function,
        "Task 2",
        4096,
        NULL,
        5,
        NULL,
        tskNO_AFFINITY);
    xTaskCreatePinnedToCore(task_entry_function,
        "Task 3",
        4096,
        NULL,
        5,
        NULL,
        tskNO_AFFINITY);
    for(int i=0; i<30; i++){
        char name[32];
        snprintf(name, sizeof(name), "float_%d",i);
        xTaskCreate(task_entry_function,
            name,
            4096,
            NULL,
            5,
            NULL);
            vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    
}
