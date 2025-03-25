#ifndef __USART_H
#define __USART_H

#include "includes.h"

/*********************************** CONFIG **************************************** */
#define DMA_ENABLE                     0

/************************************ USART **************************************** */
#define DBG_USART                      USART1
#define DBG_USART_BAUDRATE             115200
#define DBG_USART_CLK_ENABLE()         __HAL_RCC_USART1_CLK_ENABLE()

#define DBG_USART_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define DBG_USART_RX_GPIO              GPIOA
#define DBG_USART_RX_GPIO_PIN          GPIO_PIN_10
#define DBG_USART_RX_AF                GPIO_AF7_USART1

#define DBG_USART_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define DBG_USART_TX_GPIO              GPIOA
#define DBG_USART_TX_GPIO_PIN          GPIO_PIN_9
#define DBG_USART_TX_AF                GPIO_AF7_USART1

#define DBG_USART_IRQn                 USART1_IRQn
#define DBG_USART_IRQHandler           USART1_IRQHandler

/************************************ DMA **************************************** */
#define SENDBUFF_SIZE                  128

#define DBG_USART_DMA_CLK_ENABLE()     __HAL_RCC_DMA2_CLK_ENABLE()
#define DBG_USART_DMA_STREAM           DMA2_Stream7
#define DBG_USART_DMA_CHANNEL          DMA_CHANNEL_4
#define DBG_USART_DR_ADDRESS           (uint32_t)&(DBG_USART->DR)

#define DBG_USART_DMA_IRQn             DMA2_Stream7_IRQn
#define DBG_USART_DMA_IRQHandler       DMA2_Stream7_IRQHandler

void DBG_USART_Init(void);
void debug(const char *fmt, ...);

extern uint8_t sendBuff[SENDBUFF_SIZE];
extern uint8_t dma_transfer_in_progress;

#endif    // __USART_H
