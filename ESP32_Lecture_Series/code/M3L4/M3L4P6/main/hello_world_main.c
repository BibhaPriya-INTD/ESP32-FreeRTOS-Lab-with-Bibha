#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"driver/gpio.h"

void app_main(){
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    gpio_reset_pin(16);
    gpio_set_direction(16,GPIO_MODE_OUTPUT);

    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_pulldown_en(4);

    //Button FSM
    int state = 0; //0-NOT PRESSED, 1-PRESSED 
    int holdTIcks = 0;

    //Heart beat LED FSM Pin 16
    int hbState = 0; //0- OFF, 1-ON
    int hbTicks = 0; 

    //Action LED Pin 2 LED FSM
    int ledState = 0; //0-IDLE, 1-BLINKING, 2-GLOWING
    int ledTicks = 0;
    int led2state = 0;

    while(1){
        int i = gpio_get_level(4);
        //Button FSM
        switch(state){
            case 0:
                if(i == 1){
                    state = 1;
                    holdTIcks = 0;
                }
                break;
            case 1:
                if(i == 1){
                    holdTIcks++;
                }else{
                    state = 0;
                    if(holdTIcks>=20){
                        ledState = 1;
                        ledTicks = 0;
                        gpio_set_level(2,1);
                        led2state = 1;
                    }
                    else{
                        ledState = 2;
                        ledTicks = 0;
                        gpio_set_level(2,1);
                        led2state = 1;
                    }
                }
                break;
        }

        //Action LED FSM
        switch(ledState){
            case 0:
                break;
            case 1:
                led2state = !led2state;
                ledTicks++;
                gpio_set_level(2,led2state);
                if(ledTicks>=20){
                    gpio_set_level(2,0);
                    led2state=0;
                    ledState =0; 
                }
                break;
            case 3:
                ledTicks++;
                if(ledTicks>=10){
                    gpio_set_level(2,0);
                    led2state =0;
                    ledState =0;
                }
                break;
        }

        //Heart Beat Block
        hbTicks++;
        if(hbTicks>=5){
            hbTicks=0;
            hbState = !hbState;
            gpio_set_level(16,hbState);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}