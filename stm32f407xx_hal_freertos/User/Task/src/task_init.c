#include "task_init.h"

static TaskHandle_t AppTaskCreate_Handel;

task_dynamic_t Tasks[] = {
    // { ADC_Task, "ADC_Task", ADC_TASK_STACK_SIZE, NULL, ADC_TASK_PRIORITY, NULL },
    { UsartTask, "USART_Task", UART_TASK_STACK_SIZE, NULL, UART_TASK_PRIORITY, NULL }
};

timer_t Timers[] = {
    { "WatchDog", 500, pdTRUE, (void *)1, WatchDog_Callback, NULL },
};


static void AppTaskCreate(void *pvParameters)
{
    taskENTER_CRITICAL();

    for(int i = 0; i < sizeof(Tasks) / sizeof(Tasks[0]); i++)
    {
        Tasks[i].status =
            xTaskCreate(Tasks[i].func, Tasks[i].name, Tasks[i].stack_size, Tasks[i].param, Tasks[i].priority, Tasks[i].handle);

        if(Tasks[i].status != pdPASS)
            Error_Handler();
    }

    for(int i = 0; i < sizeof(Timers) / sizeof(Timers[0]); i++)
    {
        Timers[i].handle = xTimerCreate(Timers[i].name, Timers[i].tick, Timers[i].reload, Timers[i].id, Timers[i].callback);

        if(Timers[i].handle != NULL)
            xTimerStart(Timers[i].handle, 0);    // start timer
        else
            Error_Handler();
    }
    vTaskDelete(AppTaskCreate_Handel);

    taskEXIT_CRITICAL();
}

uint8_t Task_Init(void)
{
    BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值，默认为pdPASS */

    /* 创建AppTaskCreate 任务 */
    xReturn            = xTaskCreate(AppTaskCreate, "AppTaskCreate", 128, NULL, 1, &AppTaskCreate_Handel);

    if(xReturn == pdPASS)
    {
        vTaskStartScheduler();
    }
    else
        return 1;

    return 0;
}
