#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>

#include "systick.h"
#include "cmsis_os2.h"

#include "bsp.h"
#include "key_check.h"
#include "upper_comm.h"
#include "pcs_comm.h"
#include "bms_comm.h"
#include "lcd_display.h"
#include "buzzer_beep.h"
#include "log.h"

void TestTask(void *argument);

#endif /* MAIN_H */
