#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t countingTaskHandle;
TaskHandle_t waitTaskHandle;

/* ─────────────────────────────────────────
   COUNTING TASK — slow on purpose (500ms per token)
   Notifications pile up while it works.
   Each token is still processed — none lost.
   ──────────────────────────────────────── */
void CountingTask(void *pvParameters)
{
    int wakeCount = 0;
    while (1)
    {
        uint32_t val = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        wakeCount++;
        printf("[COUNTING #%d] consumed 1 token (val=%lu)\n", wakeCount, val);

        /* Simulate slow processing — more notifications arrive here */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* ─────────────────────────────────────────
   WAIT TASK — also slow on purpose (500ms per wake)
   Notifications pile up while it works.
   Only the LAST value survives — rest are lost.
   ──────────────────────────────────────── */
void WaitTask(void *pvParameters)
{
    uint32_t value;
    int wakeCount = 0;
    while (1)
    {
        xTaskNotifyWait(0x00000000, 0xFFFFFFFF, &value, portMAX_DELAY);
        wakeCount++;
        printf("[WAIT     #%d] got value = %lu\n", wakeCount, value);

        /* Simulate slow processing — more notifications arrive here */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/* ─────────────────────────────────────────
   SENDER — fires 5 notifications instantly
   to BOTH tasks with NO gap between them.
   Both tasks are sleeping when this happens.
   ──────────────────────────────────────── */
void SenderTask(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(2000)); /* let tasks block first */

    printf("\n[SENDER] Firing 5 notifications to BOTH tasks instantly...\n");
    printf("[SENDER] CountingTask gets eIncrement x5\n");
    printf("[SENDER] WaitTask     gets values 10,20,30,40,50 (eSetValueWithOverwrite)\n\n");

    uint32_t values[5] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++)
    {
        xTaskNotify(countingTaskHandle, 0,         eIncrement);
        //xTaskNotify(waitTaskHandle,     values[i], eSetValueWithOverwrite);
        //xTaskNotify(waitTaskHandle,     values[i], eSetValueWithoutOverwrite);
        xTaskNotify(waitTaskHandle,     values[i], eIncrement);
    }

    /* Sender does nothing after — we just watch the tasks drain */
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(CountingTask, "CountingTask", 2048, NULL, 2, &countingTaskHandle);
    xTaskCreate(WaitTask,     "WaitTask",     2048, NULL, 2, &waitTaskHandle);
    xTaskCreate(SenderTask,   "SenderTask",   2048, NULL, 1, NULL);
}