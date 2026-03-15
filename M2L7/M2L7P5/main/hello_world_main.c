#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

SemaphoreHandle_t mutex;

TaskHandle_t lowTaskHandle;

static const char *TAG = "PRIORITY_INHERIT";

/* LOW PRIORITY TASK */
void LowTask(void *pv)
{
    ESP_LOGI(TAG, "Low: Trying to take mutex");

    xSemaphoreTake(mutex, portMAX_DELAY);

    ESP_LOGI(TAG, "Low: Got mutex");

    for (int i = 0; i < 5; i++)
    {
        UBaseType_t pr = uxTaskPriorityGet(NULL);
        ESP_LOGI(TAG, "Low: working... current priority = %d", pr);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    ESP_LOGI(TAG, "Low: Releasing mutex");

    xSemaphoreGive(mutex);

    for (;;)
    {
        vTaskDelay(1000);
    }
}

/* MEDIUM PRIORITY TASK */
void MediumTask(void *pv)
{
    vTaskDelay(pdMS_TO_TICKS(300));

    while (1)
    {
        ESP_LOGI(TAG, "Medium: Running");

        vTaskDelay(pdMS_TO_TICKS(400));
    }
}

/* HIGH PRIORITY TASK */
void HighTask(void *pv)
{
    vTaskDelay(pdMS_TO_TICKS(600));

    ESP_LOGI(TAG, "High: Trying to take mutex");

    xSemaphoreTake(mutex, portMAX_DELAY);

    ESP_LOGI(TAG, "High: Acquired mutex");

    xSemaphoreGive(mutex);

    for (;;)
    {
        vTaskDelay(1000);
    }
}

void app_main()
{
    mutex = xSemaphoreCreateMutex();

    /* Create tasks on SAME CORE (core 1) */
    xTaskCreatePinnedToCore(LowTask, "LowTask", 2048, NULL, 1, &lowTaskHandle, 1);

    xTaskCreatePinnedToCore(MediumTask, "MediumTask", 2048, NULL, 2, NULL, 1);

    xTaskCreatePinnedToCore(HighTask, "HighTask", 2048, NULL, 3, NULL, 1);
}