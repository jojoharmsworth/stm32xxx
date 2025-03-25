#include "xtimer.h"

void WatchDog_Callback(TimerHandle_t xTimer)
{
    // WATCHDOG_TOGGLE;    // watchdog toggle
    LED1_TOGGLE;
}
