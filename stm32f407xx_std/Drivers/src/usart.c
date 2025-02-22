#include "string.h"
#include "usart.h"

uint8_t send_buff[SENDBUFF_SIZE];
uint8_t dma_transfer_in_progress;    // DMA 传输中标志

static void dbg_usart_gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(DBG_USART_RX_GPIO_CLK | DBG_USART_TX_GPIO_CLK, ENABLE);
    GPIO_PinAFConfig(DBG_USART_RX_GPIO, DBG_USART_RX_SOURCE, DBG_USART_RX_AF);
    GPIO_PinAFConfig(DBG_USART_TX_GPIO, DBG_USART_TX_SOURCE, DBG_USART_TX_AF);

    GPIO_InitStructure.GPIO_Pin   = DBG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DBG_USART_TX_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = DBG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(DBG_USART_RX_GPIO, &GPIO_InitStructure);
}

static void dbg_usart_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel                   = DBG_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void dbg_usart_mode_config(void)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(DBG_USART_CLK, ENABLE);

    USART_InitStructure.USART_BaudRate            = DBG_USART_BAUDRATE;                // 波特率
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;               // 数据位
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;                  // 停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                   // 无校验位
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;     // 发送接收模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    // 无硬件流控制
    USART_Init(DBG_USART, &USART_InitStructure);
    dbg_usart_nvic_config();
    //   USART_ITConfig(DBG_USART, USART_IT_RXNE, ENABLE);

    USART_Cmd(DBG_USART, ENABLE);
}

static void dbg_usart_dma_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(DBG_USART_DMA_CLK, ENABLE);

    DMA_DeInit(DBG_USART_DMA_STREAM);
    DMA_InitStructure.DMA_Channel            = DBG_USART_DMA_CHANNEL;          // 通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = DBG_USART_DR_ADDRESS;           // 外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr    = send_buff;                      // 存储器地址F
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;     // 方向
    DMA_InitStructure.DMA_BufferSize         = SENDBUFF_SIZE;                  // 缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;      // 外设地址不增
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;           // 存储器地址增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    // 外设数据大小
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;        // 存储器数据大小
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;                // 单次传输
    DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;          // 优先级
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;           // FIFO模式禁用
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;         // FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;         // 存储器单次突发
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;     // 外设单次突发

    DMA_Init(DBG_USART_DMA_STREAM, &DMA_InitStructure);
}

static void dbg_usart_dma_nvic_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel                   = DBG_USART_DMA_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void dbg_usart_dma_send(uint16_t length)
{
    // 关闭DMA流
    DMA_Cmd(DBG_USART_DMA_STREAM, DISABLE);
    while(DMA_GetCmdStatus(DBG_USART_DMA_STREAM) != DISABLE);
    // 清除DMA标志位
    DMA_ClearFlag(DBG_USART_DMA_STREAM, DMA_FLAG_TCIF7);
    DMA_SetCurrDataCounter(DBG_USART_DMA_STREAM, length);
    // 使能DMA流
    DMA_Cmd(DBG_USART_DMA_STREAM, ENABLE);
    while(DMA_GetCmdStatus(DBG_USART_DMA_STREAM) != ENABLE);
    // 使能USART的DMA发送
    USART_DMACmd(DBG_USART, USART_DMAReq_Tx, ENABLE);
}

void dbg_usart_init(void)
{
    dbg_usart_gpio_config();
    dbg_usart_mode_config();
    dbg_usart_dma_config();
}

void debug(const char *fmt, ...)
{
    char buffer[SENDBUFF_SIZE];
    int  size = 0;

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, SENDBUFF_SIZE, fmt, args);
    va_end(args);

#if DMA_ENABLE

    for(int i = 0; buffer[i] != '\0'; i++)
    {
        send_buff[i] = buffer[i];
        size++;
    }
    dbg_usart_dma_send(size);
    while(DMA_GetFlagStatus(DBG_USART_DMA_STREAM, DMA_FLAG_TCIF7) != SET);
    DMA_ClearFlag(DBG_USART_DMA_STREAM, DMA_FLAG_TCIF7);
#else
    for(int i = 0; buffer[i] != '\0'; i++)
    {
        USART_SendData(DBG_USART, buffer[i]);
        while(USART_GetFlagStatus(DBG_USART, USART_FLAG_TC) == RESET);
    }
#endif
}

void DBG_USART_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(DBG_USART, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(DBG_USART);
    }
}
