#include "task_print.h"

void PrintTask(void *pvParameters)
{
    uint32_t cnt = 0;
    
    while(1)
    {
        printf("adc: %d\r\n", cnt++);
        vTaskDelay(500);
    }
}
