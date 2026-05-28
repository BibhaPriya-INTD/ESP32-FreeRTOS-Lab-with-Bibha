#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Define bits
#define TASK_A_BIT (1 << 0)
#define TASK_B_BIT (1 << 1)
#define TASK_C_BIT (1 << 2)

#define ALL_SYNC_BITS (TASK_A_BIT | TASK_B_BIT | TASK_C_BIT)

EventGroupHandle_t sync_event_group;

// Task A
void taskA(void *pvParameters)
{
    for(;;)
    {
        printf("Task A: Doing work...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));

        printf("Task A: Waiting at sync point\n");

        xEventGroupSync(
            sync_event_group,
            TASK_A_BIT,      // Set my bit
            ALL_SYNC_BITS,   // Wait for all bits
            portMAX_DELAY    // Wait forever
        );

        printf("Task A: All tasks synchronized! Continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Task B
void taskB(void *pvParameters)
{
    for(;;)
    {
        printf("Task B: Doing work...\n");
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("Task B: Waiting at sync point\n");

        xEventGroupSync(
            sync_event_group,
            TASK_B_BIT,
            ALL_SYNC_BITS,
            portMAX_DELAY
        );

        printf("Task B: All tasks synchronized! Continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Task C
void taskC(void *pvParameters)
{
    for(;;)
    {
        printf("Task C: Doing work...\n");
        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("Task C: Waiting at sync point\n");

        xEventGroupSync(
            sync_event_group,
            TASK_C_BIT,
            ALL_SYNC_BITS,
            portMAX_DELAY
        );

        printf("Task C: All tasks synchronized! Continuing...\n\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Main function
void app_main(void)
{
    sync_event_group = xEventGroupCreate();

    xTaskCreate(taskA, "Task A", 2048, NULL, 1, NULL);
    xTaskCreate(taskB, "Task B", 2048, NULL, 1, NULL);
    xTaskCreate(taskC, "Task C", 2048, NULL, 1, NULL);
}