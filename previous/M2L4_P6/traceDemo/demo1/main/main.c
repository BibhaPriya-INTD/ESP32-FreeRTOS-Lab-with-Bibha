#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_app_trace.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_trace.h"
#include "soc/uart_pins.h"
#include"esp_log.h"
#include "esp_system.h"

static const char *TAG = "demo";

esp_trace_open_params_t esp_trace_get_user_params(void)
{
    static esp_apptrace_config_t app_trace_config = APPTRACE_CONFIG_DEFAULT();
#if CONFIG_APPTRACE_DEST_UART
    /* Override default values to use console pins as a uart channel */
    app_trace_config.dest_cfg.uart.tx_pin_num = U0TXD_GPIO_NUM;
    app_trace_config.dest_cfg.uart.rx_pin_num = U0RXD_GPIO_NUM;
#endif

    esp_trace_open_params_t trace_params = {
        .core_cfg = NULL,
        .encoder_name = "sysview",
        .encoder_cfg = NULL,
        .transport_name = "apptrace",
        .transport_cfg = &app_trace_config,
    };
    return trace_params;
}
void task1(void* params){
    
    for(;;){
        ESP_LOGI(TAG, "Hello from sysview_tracing example!");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void task2(void* params){
    
    for(;;){
        ESP_LOGI(TAG, "Hello from sysview_tracing example!");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(task1, "Task1", 4096, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(task2, "Task2", 4096, NULL, 3, NULL, 0);
    
}