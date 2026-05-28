#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/queue.h>

typedef struct{
    int x;
    int y;
}Data_t;

QueueHandle_t xQueueInt = NULL;
QueueHandle_t xQueueStruct = NULL;
QueueSetHandle_t xQueueSet = NULL;

void vIntSender(void* pvParameters){
    int value = 0;
    for(;;){
        value++;
        xQueueSend(xQueueInt, &value, 0);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void vStrusctSender(void* pvParameters){
    Data_t packet;
    int base = 10;
    for(;;){
        packet.x = base;
        packet.y = packet.x + 1;
        xQueueSend(xQueueStruct, &packet, 0);
        base += 10;
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void vReceiver(void* pvParameter){
    QueueHandle_t xReadyMember;
    int receivedInt;
    Data_t receivedPacket;

    for(;;){
        xReadyMember = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);
        if(xReadyMember == xQueueInt){
            xQueueReceive(xReadyMember, &receivedInt, 0);
            printf("The received value from Int Queue is %d\n", receivedInt);
        }else if(xReadyMember == xQueueStruct){
            xQueueReceive(xReadyMember, &receivedPacket, 0);
            printf("The first value of packet is %d and 2nd value is %d\n", receivedPacket.x, receivedPacket.y);
        }
    }
}

void app_main(void){
    xQueueInt = xQueueCreate(5, sizeof(int));
    xQueueStruct = xQueueCreate(5, sizeof(Data_t));
    xQueueSet = xQueueCreateSet(10);
    xQueueAddToSet(xQueueInt, xQueueSet);
    xQueueAddToSet(xQueueStruct, xQueueSet);
    xTaskCreatePinnedToCore(vIntSender, "IntSender", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vStrusctSender, "PacketSender", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(vReceiver, "Receiver", 4096, NULL, 2, NULL, 1);
}