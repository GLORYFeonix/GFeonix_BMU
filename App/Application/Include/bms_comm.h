#ifndef BMS_COMM_H
#define BMS_COMM_H

#include "uart.h"

enum
{
    BMS_DATA = 0x0101,
    BMS_STATUS = 0x0102,
    BMS_UART_T = 0x0103,
    BMS_SLEEP = 0x013D,
};

typedef struct
{
    uint16_t soc;
    uint16_t soh;
    uint16_t sumVolt;
    int8_t maxCellTemp;
    int8_t minCellTemp;
} BmsData;
extern BmsData bmsData;

void bms_comm_data(void);
void bms_comm_status(void);
void bms_comm_sleep(void);

static void bms_comm_data_rx(void);

static void bms_receive(uint8_t num, void *data);
static void bms_send(uint8_t *data);

void BmsCommTask(void *argument);

#endif /* BMS_COMM_H */
