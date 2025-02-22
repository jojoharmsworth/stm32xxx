#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include "ringbuffer.h"
#include <stdarg.h>

/*********************************** CONFIG **************************************** */
#define DMA_ENABLE               1

/*********************************** USART **************************************** */
#define DBG_USART                USART1
#define DBG_USART_BAUDRATE       115200
#define DBG_USART_CLK            RCC_APB2Periph_USART1

#define DBG_USART_RX_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define DBG_USART_RX_GPIO        GPIOA
#define DBG_USART_RX_GPIO_PIN    GPIO_Pin_10
#define DBG_USART_RX_SOURCE      GPIO_PinSource10
#define DBG_USART_RX_AF          GPIO_AF_USART1

#define DBG_USART_TX_GPIO_CLK    RCC_AHB1Periph_GPIOA
#define DBG_USART_TX_GPIO        GPIOA
#define DBG_USART_TX_GPIO_PIN    GPIO_Pin_9
#define DBG_USART_TX_SOURCE      GPIO_PinSource9
#define DBG_USART_TX_AF          GPIO_AF_USART1

#define DBG_USART_IRQn           USART1_IRQn
#define DBG_USART_IRQHandler     USART1_IRQHandler

/************************************ DMA **************************************** */
#define DBG_USART_DMA_CLK        RCC_AHB1Periph_DMA2
#define DBG_USART_DMA_STREAM     DMA2_Stream7
#define DBG_USART_DMA_CHANNEL    DMA_Channel_4
#define DBG_USART_DR_ADDRESS     (uint32_t)&(DBG_USART->DR)

#define DBG_USART_DMA_IRQn       DMA2_Stream7_IRQn
#define DBG_USART_DMA_IRQHandler DMA2_Stream7_IRQHandler

#define SENDBUFF_SIZE            128

void dbg_usart_init(void);
void debug(const char *fmt, ...);

extern uint8_t sendBuff[SENDBUFF_SIZE];
extern uint8_t dma_transfer_in_progress;

#endif    // __USART_H
