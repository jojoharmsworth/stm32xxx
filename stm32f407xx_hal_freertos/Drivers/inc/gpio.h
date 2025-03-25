#ifndef __GPIO_H
#define __GPIO_H

#include "includes.h"

#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

// Êä³ö·´×ª×´Ì¬
#define digitalToggle(p, i) \
    {                       \
        (p->ODR) ^= (i);    \
    }

#define LED0_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED0_GPIO_PORT         GPIOF
#define LED0_GPIO_PIN          GPIO_PIN_9
#define LED0(a)                HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, a)
#define LED0_TOGGLE            digitalToggle(LED0_GPIO_PORT, LED0_GPIO_PIN)

#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED1_GPIO_PORT         GPIOF
#define LED1_GPIO_PIN          GPIO_PIN_10
#define LED1(a)                HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, a)
#define LED1_TOGGLE            digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)

void GPIO_Config(void);

#endif
