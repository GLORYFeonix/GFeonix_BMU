#include "bsp.h"

void bsp_Init(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);

    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMA1);

    gpio_Init();
    uart_Init();
    i2c_Init();
    timer_Init();
}
