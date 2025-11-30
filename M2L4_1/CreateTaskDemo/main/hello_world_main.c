/*Create task Demo*/
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
/*task function*/
TaskHandle_t taskHandler1, taskHandler2;
void taskFunction(void* pvPrameter){
    long lstackVar = 0;
    static long lstaticvar = 0;
    const char* name;
    for(;;){
        name = pcTaskGetName(NULL);
        printf("Task name: %s\n", name);
        printf("Print Dynamic variable, %ld\n", lstackVar++);
        printf("print Static Variable, %ld\n", lstaticvar++);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}


void app_main(void){
    xTaskCreatePinnedToCore(
        taskFunction,
        "dynamicTask1",
        10240,
        NULL,
        1,
        NULL,
        1);
    xTaskCreatePinnedToCore(
        taskFunction,
        "dynamicTask2",
        10240,
        NULL,
        1,
        NULL,
        1);

}
