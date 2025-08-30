#include "init.h"

void bsp_init(void)
{
    HAL_Init();
    SystemClock_Config();    // System Clock 168MHz
    delay_init(168);         // SysTick 1ms

    gpio_config();
    dbg_usart_init(115200); // USART1
}
