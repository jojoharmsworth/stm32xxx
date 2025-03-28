#ifndef __TASK_INIT_H
#define __TASK_INIT_H

#include "includes.h"

#define ADC_TASK_STACK_SIZE  128
#define ADC_TASK_PRIORITY    5

#define PRINT_TASK_STACK_SIZE 128
#define PRINT_TASK_PRIORITY   5

#if configSUPPORT_STATIC_ALLOCATION

typedef struct
{
    TaskFunction_t func;          // 任务函数
    const char    *name;          // 任务名
    const uint32_t stack_size;    // 任务堆栈大小
    UBaseType_t    priority;      // 任务优先级
    StackType_t   *stack;         // 任务堆栈
    StaticTask_t  *TCB;           // 任务控制块
    TaskHandle_t   handle;        // 任务句柄
    ErrorStatus    create_err;    // 任务创建状态
} task_static_t;
#endif

#if configSUPPORT_DYNAMIC_ALLOCATION

typedef struct
{
    TaskFunction_t         func;          // 任务函数
    const char            *name;          // 任务名
    configSTACK_DEPTH_TYPE stack_size;    // 任务堆栈大小
    void *const            param;         // 任务参数
    UBaseType_t            priority;      // 任务优先级
    TaskHandle_t          *handle;        // 任务句柄
    BaseType_t             status;        // 任务创建状态
} task_dynamic_t;

#endif

typedef struct
{
    const char             *name;          // 定时器名
    TickType_t              tick;          // 定时器周期
    UBaseType_t             reload;        // 自动重载
    void                   *id;            // 定时器ID
    TimerCallbackFunction_t callback;      // 定时器回调函数
    TimerHandle_t           handle;        // 定时器句柄
} timer_t;

uint8_t Task_Init(void);

#endif /* __TASK_INIT_H */
