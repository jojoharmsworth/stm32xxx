#include "main.h"

int main(void)
{
    dbg_usart_init();

    while(1)
    {
        debug("Hello World!\n");
        debug("sdklhf\r\n");
        debug("just a moment %d minite\r\n", 10);
        delay_ms(1000);
    }
}
