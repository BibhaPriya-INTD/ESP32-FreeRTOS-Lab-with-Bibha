#include<stdint.h>
#include"freertos/FreeRTOS.h"
#include"freertos/task.h"

#define REG_WRITE_M(a,v) (*((volatile uint32_t*)(a)) = (uint32_t)(v))
#define REG_READ_M(a) (*((volatile uint32_t*)(a))


#define IO_MUX_GPIO2_REG                0x3FF49040UL
#define GPIO_FUNC2_OUT_SEL_CFG_REG      0x3FF44538UL
#define GPIO_ENABLE_W1TS_REG            0x3FF44024UL

#define IO_MUX_GPIO0_REG                0x3FF49044UL
#define GPIO_FUNC0_OUT_SEL_CFG_REG      0x3FF44530UL
#define GPIO_ENABLE_W1TC_REG            0x3FF44028UL
#define GPIO_IN_REG                     0x3FF4403CUL

#define GPIO_OUT_W1TS_REG               0x3FF44008UL
#define GPIO_OUT_W1TC_REG               0x3FF4400CUL

static void gpio2_out_init(void){
    REG_WRITE_M(IO_MUX_GPIO2_REG, ((2UL<<12)|(2UL<<10)));
    REG_WRITE_M(GPIO_FUNC2_OUT_SEL_CFG_REG,((256UL)|(1UL<<10)));
    REG_WRITE_M(GPIO_ENABLE_W1TS_REG,(1UL<<2));

}

static void gpio0_in_init(void){
    REG_WRITE_M(IO_MUX_GPIO0_REG,((2UL<<12)|(1UL<<9)|(1UL<<3)));
    REG_WRITE_M(GPIO_FUNC0_OUT_SEL_CFG_REG,(256UL|(1UL<<10)));
    REG_WRITE_M(GPIO_ENABLE_W1TC_REG,1UL<<0);
}

void app_main(void){
    gpio2_out_init();
    gpio0_in_init();
    REG_WRITE_M(GPIO_OUT_W1TC_REG,(1UL<<2));
    while (1)
    {
        if((REG_READ_M(GPIO_IN_REG)&(1UL<<0))==0)){
            REG_WRITE_M(GPIO_OUT_W1TS_REG,(1UL<<2));
        }
        else{
            REG_WRITE_M(GPIO_OUT_W1TC_REG,(1UL<<2));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
}
