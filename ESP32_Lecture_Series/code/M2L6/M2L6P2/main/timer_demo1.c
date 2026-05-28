#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/timers.h>

TimerHandle_t xAutoReloadTimer = NULL;
TimerHandle_t xOneShotTimer = NULL;
uint8_t uCounter = 0;

void prvCallBack(TimerHandle_t xTimer){
    TickType_t xNow;
    xNow = xTaskGetTickCount();
    if(xTimer == xAutoReloadTimer){
        vTimerSetTimerID(xAutoReloadTimer, &uCounter);
        if(*(uint32_t*)pvTimerGetTimerID(xAutoReloadTimer) <= 5){
            printf("The autoReload timer with ID %d is executing after %ld ticks\n",uCounter, xNow);
        }else{
            xTimerStop(xTimer, 0);
        }
        uCounter++;
    }
    if(xTimer == xOneShotTimer){
        printf("The One Shot Timer is executing at %ld\n", xNow);
    }

}
void app_main(void){
    uint8_t timer1ID = 1;
    uint8_t timer2ID = 2;
    BaseType_t xTimer1Started, xTimer2Started;

    xAutoReloadTimer = xTimerCreate("Timer1 AutoReload", pdMS_TO_TICKS(1000), pdTRUE, (void*)&(timer1ID), prvCallBack);
    xOneShotTimer = xTimerCreate("Timer2 OneShotTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)&(timer2ID), prvCallBack);

    if((xAutoReloadTimer != NULL) && (xOneShotTimer != NULL)){
        xTimer1Started = xTimerStart(xAutoReloadTimer, 0);
        xTimer1Started = xTimerStart(xOneShotTimer, 0);
    }

}