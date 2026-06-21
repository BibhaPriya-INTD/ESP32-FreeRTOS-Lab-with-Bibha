#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#define REG_WRITE_M(a,v)  (*((volatile uint32_t*)(a)) = (uint32_t)(v))
// ── Addresses from TRM v5.7 Table 6.12-1 and Table 6.12-2 ─────────────
#define IO_MUX_GPIO2_REG     0x3FF49040UL  // TRM Table 6.12-2
#define GPIO_FUNC2_OUT_SEL   0x3FF44538UL  // 0x3FF44530+4*2, §6.12.1
#define GPIO_ENABLE_W1TS_REG 0x3FF44024UL  // TRM Table 6.12-1
#define GPIO_OUT_W1TS_REG    0x3FF44008UL  // TRM Table 6.12-1
#define GPIO_OUT_W1TC_REG    0x3FF4400CUL  // TRM Table 6.12-1

static void gpio2_output_init(void) {
    // Step 1: IO MUX — MCU_SEL=2 (GPIO Matrix), FUN_DRV=2
    REG_WRITE_M(IO_MUX_GPIO2_REG, (2UL<<12)|(2UL<<10));
    // Step 2: OUT_SEL=256 -> GPIO_OUT_REG drives pin; OEN_SEL=1
    REG_WRITE_M(GPIO_FUNC2_OUT_SEL, 256UL|(1UL<<10));
    // Step 3: Enable output for GPIO2 (atomic W1TS)
    REG_WRITE_M(GPIO_ENABLE_W1TS_REG, 1UL<<2);
}
void app_main(void) {
    gpio2_output_init();
    while (1) {
        REG_WRITE_M(GPIO_OUT_W1TS_REG, 1UL<<2); // HIGH — LED ON
        vTaskDelay(pdMS_TO_TICKS(500));

        REG_WRITE_M(GPIO_OUT_W1TC_REG, 1UL<<2); // LOW  — LED OFF
        vTaskDelay(pdMS_TO_TICKS(500));
        
}
}