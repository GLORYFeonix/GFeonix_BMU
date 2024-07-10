#include "crc.h"

static uint8_t crc_tab16_init = 0;
static uint16_t crc_tab16[256];

static void init_crc16_tab(void)
{
    uint16_t i;
    uint16_t j;
    uint16_t crc;
    uint16_t c;
    for (i = 0; i < 256; i++)
    {
        crc = 0;
        c = i;
        for (j = 0; j < 8; j++)
        {
            if ((crc ^ c) & 0x0001)
                crc = (crc >> 1) ^ CRC_POLY_16;
            else
                crc = crc >> 1;
            c = c >> 1;
        }
        crc_tab16[i] = crc;
    }
    crc_tab16_init = 1;
}

uint16_t crc16_modbus(const unsigned char *input_str, uint32_t num_bytes)
{
    uint16_t crc;
    uint16_t tmp;
    uint16_t short_c;
    const unsigned char *ptr;
    uint32_t a;
    if (!crc_tab16_init)
        init_crc16_tab();
    crc = CRC_START_MODBUS;
    ptr = input_str;
    if (ptr != NULL)
        for (a = 0; a < num_bytes; a++)
        {
            short_c = 0x00ff & (uint16_t)*ptr;
            tmp = crc ^ short_c;
            crc = (crc >> 8) ^ crc_tab16[tmp & 0xff];
            ptr++;
        }
    return crc;
}
