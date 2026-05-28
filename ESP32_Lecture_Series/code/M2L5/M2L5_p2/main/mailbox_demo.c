#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

typedef struct{
    int x;
    int y;
} Data_t;

QueueHandle_t xMailBox;

void vSenderTask(void* pvParameter){
    Data_t data;
    data.x = 1;
    data.y = 2;
    for(;;){
        xQueueOverwrite(xMailBox, &data);
        data.x += 1;
        data.y = data.x+1;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vReceiverTask(void* pvParameter){
    Data_t receivedData;
    for(;;){
        xQueuePeek(xMailBox, &receivedData, portMAX_DELAY);
        printf("The Data Received is %d and %d\n", receivedData.x, receivedData.y);
    }
}

void app_main(void){
    xMailBox = xQueueCreate(1,sizeof(Data_t));
    xTaskCreatePinnedToCore(vSenderTask, "SenderTask", 4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(vReceiverTask, "ReceiverTask", 4096, NULL, 1, NULL, 1);

}