#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Define event bits
#define WIFI_CONNECTED_BIT  (1 << 0)
#define SENSOR_READY_BIT    (1 << 1)

// Event group handle
EventGroupHandle_t eventGroup;

// Task 1: WiFi Task
void wifiTask(void *pvParameters)
{
    printf("WiFi Task: Connecting...\n");
    vTaskDelay(pdMS_TO_TICKS(2000));

    printf("WiFi Task: Connected!\n");
    xEventGroupSetBits(eventGroup, WIFI_CONNECTED_BIT);

    vTaskDelete(NULL);
}

// Task 2: Sensor Task
void sensorTask(void *pvParameters)
{
    printf("Sensor Task: Initializing...\n");
    vTaskDelay(pdMS_TO_TICKS(3000));

    printf("Sensor Task: Ready!\n");
    xEventGroupSetBits(eventGroup, SENSOR_READY_BIT);

    vTaskDelete(NULL);
}

// Task 3: Processing Task
void processingTask(void *pvParameters)
{
    printf("Processing Task: Waiting for events...\n");

    // Wait for both bits
    xEventGroupWaitBits(
        eventGroup,
        WIFI_CONNECTED_BIT | SENSOR_READY_BIT, // bits to wait
        pdTRUE,   // clear bits after exit
        pdTRUE,   // wait for ALL bits
        portMAX_DELAY
    );

    printf("Processing Task: All conditions met! Processing data...\n");

    vTaskDelete(NULL);
}

void app_main()
{
    // Create event group
    eventGroup = xEventGroupCreate();

    // Create tasks
    xTaskCreate(wifiTask, "WiFi Task", 2048, NULL, 1, NULL);
    xTaskCreate(sensorTask, "Sensor Task", 2048, NULL, 1, NULL);
    xTaskCreate(processingTask, "Processing Task", 2048, NULL, 1, NULL);
}