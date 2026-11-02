#include<stdio.h>
#include"freertos/FreeRTOS.h"
#include"driver/gpio.h"
#include"freertos/task.h"
#include"freertos/queue.h"

QueueHandle_t inc_queue;

volatile uint32_t counter = 0;
volatile uint32_t main_tries = 0;
volatile uint32_t isr_tries = 0;

void IRAM_ATTR ISR(void* arg){
    gpio_intr_disable(4);
    uint8_t src = 1;
    xQueueSendFromISR(inc_queue, &src, NULL);
    gpio_intr_enable(4);
}

void counter_task(void* arg){
    uint8_t src;
    while(1){
        if(xQueueReceive(inc_queue, &src, portMAX_DELAY)){
            counter++;
            if(src==1){
                isr_tries++;
            } else {
                main_tries++;
            }
        }
    }
}

void app_main(){
    inc_queue = xQueueCreate(500, sizeof(uint8_t));
    xTaskCreate(counter_task, "counter_task", 2048, NULL, 1, NULL);
    gpio_isr_register(ISR, NULL, ESP_INTR_FLAG_LOWMED, NULL);
    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_pulldown_en(4);
    gpio_intr_disable(4);
    gpio_set_intr_type(4, GPIO_INTR_ANYEDGE);
    gpio_intr_enable(4);
    uint32_t sent = 0;
    while(sent<2000000){
        uint8_t src =0;
        xQueueSend(inc_queue, &src, 0);
        sent++;
    }
    vTaskDelay(pdMS_TO_TICKS(500));

    uint32_t expected = main_tries + isr_tries;
    printf("main Tries: %lu\n", main_tries);
    printf("isr Tries: %lu\n", isr_tries);
    printf("Expected: %lu\n", expected);
    printf("Actual: %lu\n", counter);
    printf("Loss: %lu\n", expected - counter);
}