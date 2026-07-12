#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/queue.h"
#include"driver/gpio.h"


static QueueHandle_t gesture_queue;

#define EVENT_SHORT_PRESS 1
#define EVENT_LONG_PRESS 2

//Button task
void button_task(void* pvParam){
    int state =0;
    int holdTicks = 0;
    while(1){
        int i = gpio_get_level(4);
        switch(state){
            case 0:
                if(i == 1){
                    state = 1;
                    holdTicks = 0;
                }
                break;
            case 1:
                if(i == 1){
                    holdTicks++;
                } else {
                    state = 0;
                    int event = (holdTicks >= 20) ? EVENT_LONG_PRESS : EVENT_SHORT_PRESS;
                    xQueueSend(gesture_queue,&event, 0);
                }
                break;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//Action LED
void led_task(void* pvParam){
    int event;
    while(1){
        xQueueReceive(gesture_queue, &event, portMAX_DELAY);
        if(event == EVENT_LONG_PRESS){
            for(int n=0; n<10; n++){
                gpio_set_level(2,1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(2, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }else{
            gpio_set_level(2, 1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_set_level(2,0);
        }
    }
}

//HeartBeat Task
void heartbeat_task(void* pvParam){
    while(1){
        gpio_set_level(16, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_set_level(16, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void app_main(){
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    gpio_reset_pin(16);
    gpio_set_direction(16, GPIO_MODE_OUTPUT);

    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_pulldown_en(4);

    gesture_queue = xQueueCreate(4, sizeof(int));

    xTaskCreate(button_task, "button", 2048, NULL, 2, NULL);

    xTaskCreate(led_task, "led", 2048, NULL, 2, NULL);

    xTaskCreate(heartbeat_task, "hb", 2048, NULL, 1, NULL);

}