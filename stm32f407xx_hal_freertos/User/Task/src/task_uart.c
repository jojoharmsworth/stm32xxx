#include "task_uart.h"

void UsartTask(void *pvParameters)
{
    uint32_t cnt = 0;
    
    while(1)
    {
        debug("adc: %d\r\n", cnt++);
        vTaskDelay(500);
    }
}