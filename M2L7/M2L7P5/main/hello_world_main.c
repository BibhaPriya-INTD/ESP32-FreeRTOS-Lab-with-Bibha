#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

int a = 0;
SemaphoreHandle_t mutex;

static const char *TAG = "RTOS";

void Task1(void *pvParameters)
{
    while (1)
    {
        printf("Task 1 Trying to take mutex\n");
        if (xSemaphoreTake(mutex, portMAX_DELAY))
        {
            a++;
            ESP_LOGI(TAG, "Task1 before processing a = %d", a);

            
                for (volatile long i = 0; i < 2000000; i++)
                {
                    // empty loop for delay
                }
            

            ESP_LOGI(TAG, "Task1 after processing a = %d", a);

            xSemaphoreGive(mutex);
        }
    }
}

void Task2(void *pvParameters)
{
    while (1)
    {
        printf("Task 2 Trying to take mutex\n");
        if (xSemaphoreTake(mutex, portMAX_DELAY))
        {
            a += 10;
            ESP_LOGI(TAG, "Task2 before processing a = %d", a);

            
                for (volatile long i = 0; i < 5000000; i++)
                {
                    // empty loop for delay
                }
            

            ESP_LOGI(TAG, "Task2 after processing a = %d", a);

            xSemaphoreGive(mutex);
        }


    }
}

void Task3(void *pvParameters)
{
    while (1)
    {
        // WRONG: accessing resource without mutex
        a += 5;
        ESP_LOGI(TAG, "Task3 updated a WITHOUT mutex = %d", a);

        
    for (volatile long i = 0; i < 500000; i++)
    {
        // empty loop for delay
    }

    }
}

void app_main()
{
    mutex = xSemaphoreCreateMutex();

    xTaskCreate(Task1, "Task1", 2048, NULL, 1, NULL);
    vTaskDelay(1);
    xTaskCreate(Task2, "Task2", 2048, NULL, 3, NULL);
    vTaskDelay(1);
    xTaskCreate(Task3, "Task3", 2048, NULL, 2, NULL);
}