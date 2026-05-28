#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct
{
    int id;
    char message[32];
} data_t;


QueueHandle_t xQueue;

void sender_task(void *pvParameters)
{
    data_t *ptr;

    while (1)
    {
        // Allocate memory
        ptr = (data_t *)malloc(sizeof(data_t));
        if (ptr == NULL)
        {
            printf("Memory allocation failed\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        ptr->id = 1;
        snprintf(ptr->message, sizeof(ptr->message), "Hello from Sender");

        // Send pointer to queue
        xQueueSend(xQueue, &ptr, portMAX_DELAY);

        printf("Sender: Sent pointer %p\n", ptr);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}


void receiver_task(void *pvParameters)
{
    data_t *receivedPtr;

    while (1)
    {
        if (xQueueReceive(xQueue, &receivedPtr, portMAX_DELAY) == pdPASS)
        {
            printf("Receiver: Got pointer %p\n", receivedPtr);
            printf("ID: %d, Message: %s\n",
                   receivedPtr->id,
                   receivedPtr->message);

            // IMPORTANT: Free the memory after use
            free(receivedPtr);
        }
    }
}



void app_main(void)
{
    xQueue = xQueueCreate(5, sizeof(data_t *));

    if (xQueue == NULL)
    {
        printf("Queue creation failed\n");
        return;
    }

    xTaskCreate(sender_task, "Sender", 4096, NULL, 1, NULL);
    xTaskCreate(receiver_task, "Receiver", 4096, NULL, 1, NULL);
}

