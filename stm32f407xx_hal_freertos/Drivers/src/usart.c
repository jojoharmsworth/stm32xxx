#include "usart.h"

UART_HandleTypeDef DBG_USART_Handler;
DMA_HandleTypeDef  DBG_USART_DMA_Handler;

uint8_t send_buff[SENDBUFF_SIZE];
uint8_t dma_transfer_in_progress;    // DMA 传输中标志

void UART_GPIO_Init(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if(huart->Instance == DBG_USART)
    {
        DBG_USART_CLK_ENABLE();

        DBG_USART_RX_GPIO_CLK_ENABLE();
        DBG_USART_TX_GPIO_CLK_ENABLE();

        /* 配置 Tx 引脚为复用功能 */
        GPIO_InitStruct.Pin       = DBG_USART_TX_GPIO_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = DBG_USART_TX_AF;
        HAL_GPIO_Init(DBG_USART_TX_GPIO, &GPIO_InitStruct);

        /* 配置 Rx 引脚为复用功能*/
        GPIO_InitStruct.Pin       = DBG_USART_RX_GPIO_PIN;
        GPIO_InitStruct.Alternate = DBG_USART_RX_AF;
        HAL_GPIO_Init(DBG_USART_RX_GPIO, &GPIO_InitStruct);

        /* 中断 */
        HAL_NVIC_SetPriority(DBG_USART_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(DBG_USART_IRQn);
    }
}

static void DBG_USART_Config(void)
{
    DBG_USART_Handler.Instance          = DBG_USART;
    DBG_USART_Handler.Init.BaudRate     = DBG_USART_BAUDRATE;
    DBG_USART_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    DBG_USART_Handler.Init.StopBits     = UART_STOPBITS_1;
    DBG_USART_Handler.Init.Parity       = UART_PARITY_NONE;
    DBG_USART_Handler.Init.Mode         = UART_MODE_TX_RX;
    DBG_USART_Handler.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    DBG_USART_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&DBG_USART_Handler);

    /* 使能串口接收中断 */
    __HAL_UART_ENABLE_IT(&DBG_USART_Handler, UART_IT_RXNE);
}

static void DBG_USART_DMA_Config(void)
{
    DBG_USART_DMA_CLK_ENABLE();

    DBG_USART_DMA_Handler.Instance                 = DBG_USART_DMA_STREAM;
    DBG_USART_DMA_Handler.Init.Channel             = DBG_USART_DMA_CHANNEL;
    DBG_USART_DMA_Handler.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    DBG_USART_DMA_Handler.Init.PeriphInc           = DMA_PINC_DISABLE;
    DBG_USART_DMA_Handler.Init.MemInc              = DMA_MINC_ENABLE;
    DBG_USART_DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DBG_USART_DMA_Handler.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    DBG_USART_DMA_Handler.Init.Mode                = DMA_NORMAL;
    DBG_USART_DMA_Handler.Init.Priority            = DMA_PRIORITY_LOW;
    DBG_USART_DMA_Handler.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
    DBG_USART_DMA_Handler.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    DBG_USART_DMA_Handler.Init.MemBurst            = DMA_MBURST_SINGLE;
    DBG_USART_DMA_Handler.Init.PeriphBurst         = DMA_PBURST_SINGLE;
    HAL_DMA_DeInit(&DBG_USART_DMA_Handler);
    HAL_DMA_Init(&DBG_USART_DMA_Handler);

    __HAL_LINKDMA(&DBG_USART_Handler, hdmatx, DBG_USART_DMA_Handler);    // 关联串口和DMA发送
}

void DBG_USART_Init(void)
{
    DBG_USART_Config();
    DBG_USART_DMA_Config();
}

