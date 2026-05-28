#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/timers.h>

TimerHandle_t xAutoReloadTimer = NULL;
uint8_t uCounter = 1;

void prvCallBack(TimerHandle_t xtimer){
    TickType_t xNow;
    xNow = xTaskGetTickCount();
    xTimerChangePeriod(xtimer, pdMS_TO_TICKS(uCounter*1000), 0);
    printf("The auto Reload timer period is %ld and is executing at %ld\n", pdMS_TO_TICKS(uCounter*1000),  xNow);
    uCounter++;

}

void app_main(void){
    BaseType_t xTimerStarted;
    xAutoReloadTimer = xTimerCreate("Timer1 AutoReload Timer", pdMS_TO_TICKS(1000), pdTRUE, 0, prvCallBack);
    if(xAutoReloadTimer != NULL){
        xTimerStarted = xTimerStart(xAutoReloadTimer, 0);
    }
}