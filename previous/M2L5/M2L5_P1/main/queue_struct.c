#include<inttypes.h>
#include<freertos/task.h>
#include<freertos/queue.h>


typedef enum{
    sender1,
    sender2
} Sender_t;

typedef struct{
    uint8_t ucValue;
    Sender_t source;
} Data_t;

static const Data_t xData_to_send[2]={
    {100, sender1},
    {200, sender2}
};

static void vSenderTask(void* pvParameters){
    BaseType_t xstatus;
    for(;;){
        xstatus = xQueueSend()
    }
}