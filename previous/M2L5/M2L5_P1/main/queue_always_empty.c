#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

QueueHandle_t xQueue;


static void vSenderTask(void* pvParameters){
    int32_t* lvalueToSend;
    lvalueToSend = (int32_t*) pvParameters;
    BaseType_t xStatus;

    for(;;){
        xStatus = xQueueSend(xQueue, lvalueToSend, 0);
        if(xStatus != pdPASS){
        printf("Could not send to the queue\n");
    }
    }
    
}

static void vReceiveTask(void* pvParameters){
    int32_t lReceiveValue;
    BaseType_t xstatus;
    for(;;){
        if(uxQueueMessagesWaiting(xQueue) == 0){
            printf("The Queue is  empty\n");
        }
        xstatus = xQueueReceive(xQueue, &lReceiveValue, pdMS_TO_TICKS(200));
        if(xstatus == pdPASS){
            printf("The received Value is %ld\n", lReceiveValue);
        }
    }
}

void app_main(void){
    xQueue = xQueueCreate(5, sizeof(int32_t));
    static uint32_t value1 = 100;
    static uint32_t value2 = 200;
    if(xQueue != NULL){
        xTaskCreatePinnedToCore(vSenderTask, "Sender 1", 4096, &value1, 1, NULL, 1 );
        xTaskCreatePinnedToCore(vSenderTask, "Sender 2", 4096, &value2, 1, NULL, 1 );
        xTaskCreatePinnedToCore(vReceiveTask, "Receiver", 4096, NULL, 2, NULL, 1 );

    }

}