#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

TimerHandle_t xOneShotTimer = NULL;
uint8_t uCounter = 1;

void prvCallBack(TimerHandle_t xTimer)
{
    TickType_t xNow = xTaskGetTickCount();

    if (xTimer == xOneShotTimer)
    {
        printf("Timer Expired at tick %ld, Counter = %d\n", xNow, uCounter);

        uCounter++;

        /* Reset the timer manually */
        xTimerReset(xOneShotTimer, 0);

        printf("Timer Reset Done\n");
    }
}

void app_main(void)
{
    BaseType_t xTimerStarted;

    /* Create ONE-SHOT timer */
    xOneShotTimer = xTimerCreate("Timer1 OneShot", pdMS_TO_TICKS(1000), pdFALSE, 0, prvCallBack);

    if (xOneShotTimer != NULL)
    {
        xTimerStarted = xTimerStart(xOneShotTimer, 0);
    }
}