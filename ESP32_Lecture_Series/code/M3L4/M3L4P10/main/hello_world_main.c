#include"freertos/FreeRTOS.h"
#include"driver/gpio.h"
#include"hal/gpio_ll.h"

uint32_t last_state_seen = 0;

void IRAM_ATTR ISR(void* arg){
    gpio_ll_get_intr_status(&GPIO, esp_cpu_get_core_id(), &last_state_seen);
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_4);
    gpio_ll_intr_disable(&GPIO, GPIO_NUM_19);
    if(last_state_seen & (1<<GPIO_NUM_4)){
        gpio_ll_set_level(&GPIO, GPIO_NUM_2, !(gpio_ll_get_level(&GPIO, GPIO_NUM_2)));
    }  
    if(last_state_seen & (1<<GPIO_NUM_19)){
        gpio_ll_set_level(&GPIO, GPIO_NUM_17, !(gpio_ll_get_level(&GPIO, GPIO_NUM_17)));
    }
    gpio_ll_clear_intr_status(&GPIO, last_state_seen);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_4);
    gpio_ll_intr_enable_on_core(&GPIO, esp_cpu_get_core_id(), GPIO_NUM_19);
}

void app_main(){
    gpio_isr_register(ISR, NULL, ESP_INTR_FLAG_LOWMED, NULL);

    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);

    gpio_reset_pin(GPIO_NUM_17);
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT_OUTPUT);

    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_pulldown_en(GPIO_NUM_4);

    gpio_reset_pin(GPIO_NUM_19);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
    gpio_pulldown_en(GPIO_NUM_19);

    gpio_intr_disable(GPIO_NUM_4);
    gpio_intr_disable(GPIO_NUM_19);

    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);
    gpio_set_intr_type(GPIO_NUM_19, GPIO_INTR_ANYEDGE);

    gpio_intr_enable(GPIO_NUM_4);
    gpio_intr_enable(GPIO_NUM_19);
}