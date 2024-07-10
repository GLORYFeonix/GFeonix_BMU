#ifndef I2C_H
#define I2C_H

#include <string.h>
#include "gd32f30x.h"
#include "cmsis_os2.h"

#define I2C1_OWN_ADDRESS7 0x72

void i2c_Init(void);

static void I2C1_Init(void);

#endif /* I2C_H */