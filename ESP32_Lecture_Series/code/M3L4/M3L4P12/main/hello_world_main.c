#include"freertos/FreeRTOS.h"
#include"driver/gpio.h"
#include"hal/gpio_ll.h"

void IRAM_ATTR ISR4(void* arg){
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_19);
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_4);
    gpio_ll_set_level(&GPIO, GPIO_NUM_2, 1);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_19);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_4);
}

void IRAM_ATTR ISR19(void* arg){
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_4);
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_19);
    gpio_ll_set_level(&GPIO, GPIO_NUM_2, 0);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_4);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_19);
}

void app_main(){
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM);
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_pulldown_en(GPIO_NUM_4);
    gpio_intr_disable(GPIO_NUM_4);
    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(GPIO_NUM_4, ISR4, NULL);

    gpio_reset_pin(GPIO_NUM_19);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
    gpio_pulldown_en(GPIO_NUM_19);
    gpio_intr_disable(GPIO_NUM_19);
    gpio_set_intr_type(GPIO_NUM_19, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(GPIO_NUM_19, ISR19, NULL);

    gpio_intr_enable(GPIO_NUM_4);
    gpio_intr_enable(GPIO_NUM_19);

    vTaskDelay(pdMS_TO_TICKS(60000));
    gpio_intr_disable(GPIO_NUM_19);
    //gpio_isr_handler_remove(GPIO_NUM_19);
    gpio_intr_disable(GPIO_NUM_4);
    gpio_uninstall_isr_service();

}