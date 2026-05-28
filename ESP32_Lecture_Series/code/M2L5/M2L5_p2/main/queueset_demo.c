#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/queue.h>
#include<freertos/task.h>

QueueHandle_t xQueue1 = NULL, xQueue2 = NULL;
QueueSetHandle_t xQueueSet = NULL;

void vSender1(void* pvParameters){
    int value = 100;
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(100));
        xQueueSend(xQueue1, &value, 0);
    }
}

void vSender2(void* pvParameters){
    int value = 200;
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(200));
        xQueueSend(xQueue1, &value, 0);
    }
}

void vReceiver(void* pvParameter){
    int value;
    QueueHandle_t xQueueWithData;
    for(;;){
        xQueueWithData = (QueueHandle_t)xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
        xQueueReceive(xQueueWithData, &value, 0);
        printf("The value is %d\n", value);
    }
}

void app_main(){
    xQueue1 = xQueueCreate(5, sizeof(int));
    xQueue2 = xQueueCreate(5, sizeof(int));
    xQueueSet = xQueueCreateSet(10);
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xTaskCreatePinnedToCore(vSender1, "Sender1", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vSender2, "Sender2", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vReceiver, "Receiver", 4096, NULL, 2, NULL, 1);
}