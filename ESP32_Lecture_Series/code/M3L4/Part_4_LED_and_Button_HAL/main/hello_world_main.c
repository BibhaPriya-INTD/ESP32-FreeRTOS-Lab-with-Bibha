#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"driver/gpio.h"
void app_main(){
    gpio_reset_pin(2);
    gpio_set_direction(2,GPIO_MODE_OUTPUT);

    gpio_reset_pin(4);
    gpio_set_direction(4,GPIO_MODE_INPUT);
    gpio_pulldown_en(4);

    while(1){
        if(gpio_get_level(4)){
            gpio_set_level(2,1);
        }else{
            gpio_set_level(2,0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}