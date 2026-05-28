#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t receiverHandle;

/* ================= RECEIVER TASK ================= */
void ReceiverTask(void *pvParameters)
{
    uint32_t value;

    while (1)
    {
        /* -------- CASE 1 TAKE -------- 
        printf("\n--- Receiver Waiting (ulTaskNotifyTake) ---\n");

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Receiver woke using ulTaskNotifyTake!\n");

        vTaskDelay(pdMS_TO_TICKS(2000));*/

        /* -------- CASE 2 TAKE -------- 
        printf("\n--- Receiver Waiting (ulTaskNotifyTake) ---\n");

        uint32_t val = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // NON-blocking check
        printf("Receiver woke using ulTaskNotifyTake!\n");
        printf("Initial value check: %lu\n", val);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));*/

        /* -------- CASE 3 TAKE -------- 
        printf("\n--- Receiver Waiting (ulTaskNotifyTake) ---\n");

        uint32_t val = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // NON-blocking check
        printf("Receiver woke using ulTaskNotifyTake!\n");
        printf("Initial value check: %lu\n", val);

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));*/

        /* -------- CASE 4 TAKE -------- 
        printf("\n--- Receiver Waiting (xTaskNotifyWait) ---\n");

        xTaskNotifyWait(0, 0xFFFFFFFF, &value, portMAX_DELAY);
        printf("Receiver woke using xTaskNotifyWait! Value = %lu\n", value);

        vTaskDelay(pdMS_TO_TICKS(2000));*/



        /* -------- CASE 5 TAKE -------- */
        printf("\n--- Receiver Waiting (xTaskNotifyWait) ---\n");

        xTaskNotifyWait(0, 0xFFFFFFFF, &value, portMAX_DELAY);
        printf("Receiver woke using xTaskNotifyWait! Value = %lu\n", value);

        vTaskDelay(pdMS_TO_TICKS(2000));

        
    }
}

/* ================= SENDER TASK ================= */
void SenderTask(void *pvParameters)
{
    while (1)
    {
        /* -------- CASE 1 -------- 
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n[CASE 1] xTaskNotifyGive() → SHOULD WAKE (Take)\n");
        xTaskNotifyGive(receiverHandle);
        */


        /* -------- CASE 2 -------- 
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n[CASE 1] xTaskNotify() → SHOULD WAKE (Take)\n");
         xTaskNotify(receiverHandle, 0, eNoAction);
         */



         /* -------- CASE 3 -------- 
        vTaskDelay(pdMS_TO_TICKS(2000));

        printf("\n[CASE 1] xTaskNotify() → SHOULD WAKE (Take)\n");
         xTaskNotify(receiverHandle, 0, eIncrement);*/


         /* -------- CASE 4 -------- 
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("\n[CASE 1] xTaskNotify() → SHOULD WAKE (Wait)\n");

        xTaskNotify(receiverHandle, 42, eIncrement);*/

        
        /* -------- CASE 5 -------- */
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("\n[CASE 1] xTaskNotify() → SHOULD WAKE (Wait)\n");

        xTaskNotify(receiverHandle, 42, eSetValueWithOverwrite);
        

    }
}

/* ================= MAIN ================= */
void app_main(void)
{
    xTaskCreate(ReceiverTask, "ReceiverTask", 2048, NULL, 2, &receiverHandle);
    xTaskCreate(SenderTask, "SenderTask", 2048, NULL, 1, NULL);
}
