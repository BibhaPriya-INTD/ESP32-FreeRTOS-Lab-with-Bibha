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
        while(gpio_get_level(4)==0){
            vTaskDelay(pdMS_TO_TICKS(10));
        }//wait for the press
        vTaskDelay(pdMS_TO_TICKS(10));

        int heldTicks = 0;

        while(gpio_get_level(4)==1){
            vTaskDelay(pdMS_TO_TICKS(100));
            heldTicks++;
        }//wait for release

        if(heldTicks>20){
            for(int n =0; n<10; n++){
                gpio_set_level(2,1);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(2,0);
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        } else{
            gpio_set_level(2,1);
            vTaskDelay(pdMS_TO_TICKS(1000));
            gpio_set_level(2,0);
        }
    }    
}