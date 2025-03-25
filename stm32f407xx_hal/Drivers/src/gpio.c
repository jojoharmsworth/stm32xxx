#include "gpio.h"

void gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructer;

    LED0_GPIO_CLK_ENABLE();
    LED1_GPIO_CLK_ENABLE();

    GPIO_InitStructer.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructer.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructer.Pin   = LED0_GPIO_PIN;
    GPIO_InitStructer.Pull  = GPIO_PULLUP;
    HAL_GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructer);

    GPIO_InitStructer.Pin = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructer);

    LED0(OFF);
    LED1(OFF);
}
