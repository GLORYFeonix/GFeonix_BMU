#ifndef BSP_H
#define BSP_H

#include "gd32f30x.h"

#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "timer.h"

void bsp_Init(void);

void uart_Init(void);

#endif /* BSP_H */