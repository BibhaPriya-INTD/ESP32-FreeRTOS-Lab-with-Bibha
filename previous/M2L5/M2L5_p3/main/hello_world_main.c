#include<inttypes.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

QueueHandle_t xQueue;

typedef enum{
    sender1,
    sender2
} Sender_t;

typedef struct{
    uint8_t ucValue;
    Sender_t source;
} Data_t;

static Data_t xData_to_send[2]={
    {100, sender1},
    {200, sender2}
};

static void vSenderTask(void* pvParameters){
    BaseType_t xstatus;
    for(;;){
        xstatus = xQueueSend(xQueue, pvParameters, pdMS_TO_TICKS(100));
        if(xstatus != pdPASS){
            printf("Can not send to queue\n");
        }

    }
}

static void vReceiverTask(void* pvParameters){
    Data_t xReceivedStruct;
    BaseType_t xstatus;
    for(;;){
        if(uxQueueMessagesWaiting(xQueue) != 3){
            printf("Queue is not full\n");
        }
        xstatus = xQueueReceive(xQueue, &xReceivedStruct, 0);
        if(xstatus == pdPASS){
            if(xReceivedStruct.source == sender1){
                printf("From Sender 1 = %d\n", xReceivedStruct.ucValue);
            } else {
                printf("From Sender 2 = %d\n", xReceivedStruct.ucValue);
            }
        } else{
            printf("Could not receive from the queue\n");
        }

    }
}

void app_main(){
    xQueue = xQueueCreate(3, sizeof(Data_t));
    if(xQueue != NULL){
        xTaskCreatePinnedToCore(vSenderTask, "Sender 1", 4096, &(xData_to_send[0]), 2, NULL, 1);
        xTaskCreatePinnedToCore(vSenderTask, "Sender 2", 4096, &(xData_to_send[1]), 2, NULL, 1);
        xTaskCreatePinnedToCore(vReceiverTask, "Receivber", 4096, NULL, 1, NULL, 1);
    }else{
        printf("THe task could not be created\n");
    }
}