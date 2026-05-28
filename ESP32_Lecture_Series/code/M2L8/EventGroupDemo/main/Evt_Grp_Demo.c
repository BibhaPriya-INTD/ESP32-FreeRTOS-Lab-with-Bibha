#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/event_groups.h"

#define WIFI_CONNECTED_BIT (1<<0)
#define SENSOR_READY_BIT   (1<<1)

EventGroupHandle_t eventGroup;

void wifiTask(void* pv){
    printf("Wifi Task: Connecting....\n");
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("Wifi Task: Connected...\n");
    xEventGroupSetBits(eventGroup,WIFI_CONNECTED_BIT);
    vTaskDelete(NULL);
}

void sensorTask(void* pv){
    printf("Sendor Task: Initilizing....\n");
    vTaskDelay(pdMS_TO_TICKS(2000));
    printf("Sensor Task: ready...\n");
    xEventGroupSetBits(eventGroup,SENSOR_READY_BIT);
    vTaskDelete(NULL);
}

void processingTask(void* pv){
    printf("Processing Task: Waiting for events....\n");
    xEventGroupWaitBits(eventGroup,
                        WIFI_CONNECTED_BIT | SENSOR_READY_BIT,
                        pdTRUE,
                        pdTRUE,
                        portMAX_DELAY
                    );
    printf("Processing Task: All Conditions set processing Data...\n");
    vTaskDelete(NULL);
}

void app_main(){
    eventGroup = xEventGroupCreate();

    xTaskCreatePinnedToCore(wifiTask, "WIFI", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(sensorTask, "SENSOR", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(processingTask, "PROCESSING", 2048, NULL, 1, NULL, 1);

}