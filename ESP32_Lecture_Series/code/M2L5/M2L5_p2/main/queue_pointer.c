#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

typedef struct{
    int id;
    char message[32];
} data_t;

QueueHandle_t xQueue;

void sender_task(void* pvParameter){
    data_t* ptr;
    for(;;){
        ptr = (data_t*)malloc(sizeof(data_t));
        if(ptr == NULL){
            printf("Memory allocation failed");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        ptr->id = 1;
        snprintf(ptr->message, sizeof(ptr->message), "Hello from Sender");
        xQueueSend(xQueue, &ptr, 0);
    }
}

void receiver_task(void* pvParameter){
    data_t *receivePtr;
    BaseType_t xstatus;
    for(;;){
        xstatus = xQueueReceive(xQueue, &receivePtr, pdMS_TO_TICKS(100));
        if(xstatus==pdPASS){
            printf("ID: %d, Message : %s\n", receivePtr->id, receivePtr->message);
            //Very important
            free(receivePtr);
        }

    }
}

void app_main(){
    xQueue = xQueueCreate(5, sizeof(data_t*));
    if(xQueue == NULL){
        printf("Queue Creation failed\n");
    }else{
        xTaskCreatePinnedToCore(sender_task, "Sender", 4096, NULL, 1, NULL, 1);
        xTaskCreatePinnedToCore(receiver_task, "Receiver", 4096, NULL, 2, NULL, 1);
    }


}