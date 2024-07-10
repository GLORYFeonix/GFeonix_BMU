#include "buzzer.h"


void buzzer_beep(uint8_t onoff)
{
    if (onoff)
    {
        timer_channel_output_pulse_value_config(TIMER8, TIMER_CH_0, 199);
    }
    else
    {
        timer_channel_output_pulse_value_config(TIMER8, TIMER_CH_0, 0);
    }
}
