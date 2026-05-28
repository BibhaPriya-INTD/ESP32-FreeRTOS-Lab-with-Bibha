#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "QUEUE_SET_DEMO";

/* Structure containing two integers */
typedef struct
{
    int x;
    int y;
} DataPacket;

/* Queue Handles */
QueueHandle_t xQueueInt = NULL;
QueueHandle_t xQueueStruct = NULL;
QueueSetHandle_t xQueueSet = NULL;

/* ---------------- Sender Task 1 (Integer Sender) ---------------- */
void vIntSenderTask(void *pvParameters)
{
    int value = 0;

    while (1)
    {
        value++;
        xQueueSend(xQueueInt, &value, portMAX_DELAY);
        ESP_LOGI(TAG, "Sent INT: %d", value);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* ---------------- Sender Task 2 (Struct Sender) ---------------- */
void vStructSenderTask(void *pvParameters)
{
    DataPacket packet;
    int base = 100;

    while (1)
    {
        packet.x = base;
        packet.y = base + 1;

        xQueueSend(xQueueStruct, &packet, portMAX_DELAY);
        ESP_LOGI(TAG, "Sent STRUCT: x=%d y=%d", packet.x, packet.y);

        base += 10;
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

/* ---------------- Receiver Task ---------------- */
void vReceiverTask(void *pvParameters)
{
    QueueHandle_t xActivatedMember;

    int receivedInt;
    DataPacket receivedPacket;

    while (1)
    {
        /* Wait for any queue in the set */
        xActivatedMember = xQueueSelectFromSet(xQueueSet, portMAX_DELAY);

        if (xActivatedMember == xQueueInt)
        {
            if (xQueueReceive(xQueueInt, &receivedInt, 0) == pdPASS)
            {
                ESP_LOGI(TAG, "Received INT: %d", receivedInt);
            }
        }
        else if (xActivatedMember == xQueueStruct)
        {
            if (xQueueReceive(xQueueStruct, &receivedPacket, 0) == pdPASS)
            {
                ESP_LOGI(TAG, "Received STRUCT: x=%d y=%d",
                         receivedPacket.x,
                         receivedPacket.y);
            }
        }
    }
}

/* ---------------- app_main ---------------- */
void app_main(void)
{
    /* Create individual queues */
    xQueueInt = xQueueCreate(5, sizeof(int));
    xQueueStruct = xQueueCreate(5, sizeof(DataPacket));

    /* IMPORTANT:
       Queue set length must be >= sum of member queue lengths
       5 + 5 = 10
    */
    xQueueSet = xQueueCreateSet(10);

    /* Add queues to set */
    xQueueAddToSet(xQueueInt, xQueueSet);
    xQueueAddToSet(xQueueStruct, xQueueSet);

    /* Create tasks */
    xTaskCreate(vIntSenderTask, "IntSender", 2048, NULL, 1, NULL);
    xTaskCreate(vStructSenderTask, "StructSender", 2048, NULL, 1, NULL);
    xTaskCreate(vReceiverTask, "Receiver", 2048, NULL, 2, NULL);
}
