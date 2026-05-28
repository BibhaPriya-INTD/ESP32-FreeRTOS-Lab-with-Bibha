#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

TaskHandle_t countingTaskHandle;
TaskHandle_t waitTaskHandle;

void CountingTask(void* pv){
    int wakeCount = 0;
    for(;;){
        uint32_t val = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        wakeCount++;
        printf("[Counting #%d] consumed 1 token (val = %lu)\n", wakeCount, val);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void WaitTask(void* pv){
    uint32_t val;
    int wakeCount = 0;
    for(;;){
        xTaskNotifyWait(0x00000000, 0xFFFFFFFF, &val, portMAX_DELAY);
        wakeCount++;
        printf("[Wait #%d] got value = %lu\n", wakeCount, val);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void Sender(void* pv){
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("Sender will fire 5 Notification to the two tasks\n");
    uint32_t value[5] = {10, 20, 30, 40, 50};
    for(int i =0; i<5; i++){
        xTaskNotify(countingTaskHandle, 0, eIncrement);//XTaskNotifyGive();
        xTaskNotify(waitTaskHandle, value[i], eSetValueWithOverwrite);
    }
    vTaskDelete(NULL);
}

void app_main(void){
    xTaskCreatePinnedToCore(CountingTask, "CountingTask", 2048, NULL, 2, &countingTaskHandle, 1);
    xTaskCreatePinnedToCore(WaitTask, "WaitTask", 2048, NULL, 2, &waitTaskHandle, 1);
    xTaskCreatePinnedToCore(Sender, "Sender", 2048, NULL, 1, NULL, 1);

}








