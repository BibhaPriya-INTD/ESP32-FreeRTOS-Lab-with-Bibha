#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define STACK_SIZE 2048

static StackType_t task_stack[STACK_SIZE];
static StaticTask_t task_tcb;

void static_task(void* pvParameters){
    for(;;){
        printf("Static task running\n");;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreateStaticPinnedToCore(
        static_task,
        "Static_Task",
        STACK_SIZE,
        NULL,
        1,
        task_stack,
        &task_tcb,
        1
    );
}
