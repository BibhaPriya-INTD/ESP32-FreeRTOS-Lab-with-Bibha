#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/semphr.h"
#include"esp_log.h"

SemaphoreHandle_t mutex;

static const char *TAG = "PRIORITY_INHERIT";

void LowTask(void* pv){
    ESP_LOGI(TAG, "LOW: Trying to take the mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);
    ESP_LOGI(TAG, "LOW: Mutex Taken");
    for(int i = 0; i<5; i++){
        ESP_LOGI(TAG, "LOW: Working.... Current Priority = %d", uxTaskPriorityGet(NULL));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    ESP_LOGI(TAG, "LOW: Releasing the mutex");
    xSemaphoreGive(mutex);

    for(;;){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void MediumTask(void* pv){
    vTaskDelay(pdMS_TO_TICKS(300));
    for(;;){
        ESP_LOGI(TAG, "MEDIUM: Running");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void HighTask(void* pv){
    vTaskDelay(pdMS_TO_TICKS(600));
    ESP_LOGI(TAG, "HIGH: Trying to take the mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);
    ESP_LOGI(TAG, "HIGH: Aquired Mutex");
    xSemaphoreGive(mutex);
    for(;;){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(){
    mutex = xSemaphoreCreateMutex();
    xTaskCreatePinnedToCore(LowTask, "LowTask", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(MediumTask, "MediumTask", 2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(HighTask, "HighTask", 2048, NULL, 3, NULL, 1);
}