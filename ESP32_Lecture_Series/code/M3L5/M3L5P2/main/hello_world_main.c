#include<stdio.h>
#include"esp32/rom/rtc.h"
#include"freertos/FreeRTOS.h"

void app_main(void){
    RESET_REASON pro_reason = rtc_get_reset_reason(0);
    RESET_REASON app_reason = rtc_get_reset_reason(1);

    printf("PRO CPU RESET REASON %d\n", pro_reason);
    printf("APP CPU RESET REASON %d\n", app_reason);

    for(int i = 5 ; i>0; i--){
        printf("Restarting in  %d\n", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    esp_restart();

}