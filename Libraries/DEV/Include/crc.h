#ifndef CRC_H
#define CRC_H

#include <string.h>
#include "gd32f30x.h"

#define CRC_POLY_16 0xA001
#define CRC_START_MODBUS 0xFFFF

static void init_crc16_tab(void);
uint16_t crc16_modbus(const unsigned char *input_str, uint32_t num_bytes);

#endif /* CRC_H */
