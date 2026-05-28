#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/event_groups.h"

#define TASK_A_BIT (1<<0)
#define TASK_B_BIT (1<<1)
#define TASK_C_BIT (1<<2)

#define ALL_SYNC_BITS (TASK_A_BIT | TASK_B_BIT | TASK_C_BIT)

EventGroupHandle_t sync_evt_grp;

//Task A
void taskA(void* pv){
    for(;;){
        printf("TASK A: Doing Work......\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("TASK A: Waiting at Sync Point....\n");

        xEventGroupSync(
            sync_evt_grp,
            TASK_A_BIT,
            ALL_SYNC_BITS,
            portMAX_DELAY);
        printf("TASK A: Synchronized and continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//Task B
void taskB(void* pv){
    for(;;){
        printf("TASK B: Doing Work......\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("TASK B: Waiting at Sync Point....\n");

        xEventGroupSync(
            sync_evt_grp,
            TASK_B_BIT,
            ALL_SYNC_BITS,
            portMAX_DELAY);
        printf("TASK B: Synchronized and continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//Task C
void taskC(void* pv){
    for(;;){
        printf("TASK C: Doing Work......\n");
        vTaskDelay(pdMS_TO_TICKS(3000));
        printf("TASK C: Waiting at Sync Point....\n");

        xEventGroupSync(
            sync_evt_grp,
            TASK_C_BIT,
            ALL_SYNC_BITS,
            portMAX_DELAY);
        printf("TASK C: Synchronized and continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(){
    sync_evt_grp = xEventGroupCreate();

    xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskB, "Task B", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskC, "Task C", 2048, NULL, 1, NULL, 1);
}