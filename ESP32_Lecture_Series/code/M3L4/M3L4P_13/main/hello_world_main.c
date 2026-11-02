#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"driver/gpio.h"
#include"esp_rom_sys.h"

volatile uint32_t counter = 0;
volatile uint32_t main_tries = 0;
volatile uint32_t isr_tries = 0;

void IRAM_ATTR ISR(void* arg){
    gpio_intr_disable(4);
    counter++;
    isr_tries++;
    gpio_intr_enable(4);
}

void app_main(){
    gpio_isr_register(ISR, NULL, ESP_INTR_FLAG_LOWMED, NULL);
    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_pulldown_en(4);
    gpio_intr_disable(4);
    gpio_set_intr_type(4, GPIO_INTR_ANYEDGE);
    gpio_intr_enable(4);
    while(main_tries<2000000){
        uint32_t temp = counter;
        esp_rom_delay_us(5);
        temp++;
        counter = temp;
        main_tries++;
    }
    uint32_t expected = main_tries + isr_tries;
    printf("main Tries: %lu\n", main_tries);
    printf("isr Tries: %lu\n", isr_tries);
    printf("Expected: %lu\n", expected);
    printf("Actual: %lu\n", counter);
    printf("Loss: %lu\n", expected - counter);
}