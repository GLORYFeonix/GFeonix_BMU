#ifndef BUZZER_BEEP_H
#define BUZZER_BEEP_H

#include "buzzer.h"

#define BEEP_ENABLE (1)

typedef enum
{
    BEEP_SHORT,
    BEEP_LONG,
} beep_type_e;

typedef struct
{
    beep_type_e type;
    uint8_t times;
} beep_t;

void buzzer_beep_set(uint8_t times, beep_type_e type);

void BuzzerBeepTask(void *argument);

#endif /* BUZZER_BEEP_H */
