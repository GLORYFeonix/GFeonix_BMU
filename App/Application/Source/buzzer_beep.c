#include "buzzer_beep.h"

static beep_t beep;

void buzzer_beep_set(uint8_t times, beep_type_e type)
{
#if BEEP_ENABLE
    if (beep.times == 0)
    {
        beep.times = times;
        beep.type = type;
    }
#endif
}

void BuzzerBeepTask(void *argument)
{
    while (1)
    {
        if (beep.times > 0)
        {
            buzzer_beep(1);
            if (beep.type == BEEP_SHORT)
            {
                osDelay(200);
            }
            else
            {
                osDelay(500);
            }
            buzzer_beep(0);
            beep.times--;
        }
        osDelay(200);
    }
}