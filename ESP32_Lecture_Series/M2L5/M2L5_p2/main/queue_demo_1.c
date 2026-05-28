#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

QueueHandle_t xQueue;

void vSenderTask(void* pvParameter){
    int32_t* valueToSend;
    valueToSend = (int32_t*)pvParameter;
    BaseType_t status;

    for(;;){
        status = xQueueSend(xQueue, valueToSend, 0);
        if(status != pdPASS){
            printf("can not send data\n");
        }
    }
}

void vReceiverTask(void* pvParameter){
    int32_t receivedValue;
    BaseType_t status;
    for(;;){
        if(uxQueueMessagesWaiting(xQueue) == 0){
            printf("The queue is empty\n");
        }
        status = xQueueReceive(xQueue, &receivedValue, pdMS_TO_TICKS(100));
        if(status == pdPASS){
            printf("The received value is %ld\n", receivedValue);
        }
    }
}

void app_main(void){
    xQueue = xQueueCreate(5, sizeof(int32_t));
    static uint32_t value1 = 100;
    static uint32_t value2 = 200;
    if(xQueue != NULL){
        xTaskCreatePinnedToCore(vSenderTask, "Sender 1", 4096, &value1, 1, NULL, 1);
        xTaskCreatePinnedToCore(vSenderTask, "Sender 2", 4096, &value2, 1, NULL, 1);
        xTaskCreatePinnedToCore(vReceiverTask, "Receiver", 4096, NULL, 2, NULL, 1);
    }
}