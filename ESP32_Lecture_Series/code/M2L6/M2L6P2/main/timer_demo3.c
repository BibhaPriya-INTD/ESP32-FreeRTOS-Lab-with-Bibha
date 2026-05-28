#include<stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include<freertos/timers.h>

TimerHandle_t xOneSHotTimer = NULL;
uint8_t uCounter = 1;

void prvCallBack(TimerHandle_t xtimer){
    TickType_t xNow;
    xNow = xTaskGetTickCount();
    printf("The Timer expired at tick %ld , Counter value is %d\n", xNow, uCounter);
    uCounter++;
    xTimerReset(xtimer, 0);
    printf("imer Reset Done\n");
}

void app_main(void){
    BaseType_t xTimerStarted;
    xOneSHotTimer = xTimerCreate("Timer1 Oneshot Timer", pdMS_TO_TICKS(1000), pdFALSE, 0, prvCallBack);
    if(xOneSHotTimer != NULL){
        xTimerStarted = xTimerStart(xOneSHotTimer, 0);
    }

}