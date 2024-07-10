#include "timer.h"

void timer_Init(void)
{
    TIMER8_Init();
}

static void TIMER8_Init(void)
{
    /* -----------------------------------------------------------------------
    TIMER8CLK = SystemCoreClock / 120 = 1MHz

    TIMER8 channel0 duty cycle = (200/ 400)* 100  = 50%
    ----------------------------------------------------------------------- */
    rcu_periph_clock_enable(RCU_TIMER8);

    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    timer_deinit(TIMER8);

    /* TIMER8 configuration */
    timer_initpara.prescaler = 119;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 399;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER8, &timer_initpara);

    /* CH0,CH1 and CH2 configuration in PWM mode */
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER8, TIMER_CH_0, &timer_ocintpara);

    /* CH0 configuration in PWM mode0,duty cycle 75% */
    timer_channel_output_pulse_value_config(TIMER8, TIMER_CH_0, 0);
    timer_channel_output_mode_config(TIMER8, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER8, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER8);
    /* auto-reload preload enable */
    timer_enable(TIMER8);
}