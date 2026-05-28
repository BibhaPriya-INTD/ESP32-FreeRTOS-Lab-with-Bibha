#include<inttypes.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

QueueHandle_t xQueue;

typedef struct{
    uint8_t TaskId;
    uint32_t value;
} Data_t;

static Data_t xData_to_send[2] = {
    {1, 100},
    {2, 200}
};

void vSender(void* pvParameter){
    BaseType_t xstatus;
    for(;;){
        xstatus = xQueueSend(xQueue, pvParameter, pdMS_TO_TICKS(100));
        if(xstatus != pdPASS){
            printf("Can not send to the queue\n");
        }
    }
}

void vReceiver(void* pvParameter){
    Data_t xReceivedData;
    BaseType_t xstatus;
    for(;;){
        if(uxQueueMessagesWaiting(xQueue) != 5){
            printf("The Que is not full\n");
        }
        xstatus = xQueueReceive(xQueue, &xReceivedData, 0);
        if(xstatus == pdPASS){
            printf("Data from sender %d is %ld\n", xReceivedData.TaskId, xReceivedData.value);
        } else {
            printf("Could not receive from the queue\n");
        }
    }
}

void app_main(){
    xQueue = xQueueCreate(5, sizeof(Data_t));
    if(xQueue != NULL){
        xTaskCreatePinnedToCore(vSender, "Sender 1", 4096, &xData_to_send[0], 2, NULL, 1);
        xTaskCreatePinnedToCore(vSender, "Sender 2", 4096, &xData_to_send[1], 2, NULL, 1);
        xTaskCreatePinnedToCore(vReceiver, "Receiver", 4096, NULL, 1, NULL, 1);
    } else {
        printf("Queue not created\n ");
    }
}
