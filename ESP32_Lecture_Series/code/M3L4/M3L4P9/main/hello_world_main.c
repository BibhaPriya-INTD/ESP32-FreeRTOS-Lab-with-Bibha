#include"freertos/FreeRTOS.h"
#include"driver/gpio.h"

void ISR(void* arg){
    //diable gpio4 interrupt
    gpio_intr_disable(GPIO_NUM_4);
    gpio_set_level(GPIO_NUM_2, !(gpio_get_level(GPIO_NUM_2)));
    gpio_intr_enable(GPIO_NUM_4);
}

void app_main(){
    gpio_isr_register(ISR,NULL, ESP_INTR_FLAG_LOWMED,NULL);
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);

    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_pulldown_en(GPIO_NUM_4);

    gpio_intr_disable(GPIO_NUM_4);
    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_ANYEDGE);

    gpio_intr_enable(GPIO_NUM_4);
}