#include "bms_comm.h"

BmsData bmsData;
static struct BmsHead
{
    uint8_t header;
    uint8_t addr;
    uint16_t serviceId;
    uint8_t rtn;
    uint16_t dateLen;
} bmsHead = {0xEA, 0x00, 0x0000, 0x00, 0x0000};

void bms_comm_data(void)
{
    bmsHead.serviceId = BMS_DATA;
    bmsHead.dateLen = 1;
    uint8_t data[1] = {0x00};
    bms_send(data);
}

void bms_comm_status(void)
{
    bmsHead.serviceId = BMS_STATUS;
    bmsHead.dateLen = 1;
    uint8_t data[1] = {0x00};
    bms_send(data);
}

void bms_comm_sleep(void)
{
    bmsHead.serviceId = BMS_SLEEP;
    bmsHead.dateLen = 0;
    bms_send(NULL);
}

void bms_uart_test(void)
{
    bmsHead.serviceId = BMS_UART_T;
    bmsHead.dateLen = 0;
    bms_send(NULL);
}

static void bms_comm_data_rx(void)
{
    uint8_t data[160];
    memset(data, 0x00, sizeof(data)); // bms解包没写好，先全部复制
    bms_receive(sizeof(data), data);
    bmsData.soc = data[52];
}

static void bms_receive(uint8_t num, void *data)
{
    uart_receive(BMS_UART, data, num);
}

static void bms_send(uint8_t *data)
{
    uint16_t len = 0;
    uint16_t crc = 0;

    txBuf3[len++] = bmsHead.header;
    txBuf3[len++] = bmsHead.addr;
    txBuf3[len++] = (uint8_t)(bmsHead.serviceId >> 8);
    txBuf3[len++] = (uint8_t)(bmsHead.serviceId >> 0);
    txBuf3[len++] = bmsHead.rtn;
    txBuf3[len++] = (uint8_t)(bmsHead.dateLen >> 8);
    txBuf3[len++] = (uint8_t)(bmsHead.dateLen >> 0);
    if (data != NULL)
    {
        for (uint16_t i = 0; i < bmsHead.dateLen; i++)
        {
            txBuf3[len++] = *(data + i);
        }
    }
    crc = crc16_modbus(&txBuf3[0], len);
    txBuf3[len++] = (uint8_t)(crc >> 8);
    txBuf3[len++] = (uint8_t)(crc >> 0);
    txBuf3[len++] = 0xED;

    uart_send(BMS_UART, len);
}

void BmsCommTask(void *argument)
{
    while (1)
    {
        bms_comm_data();
        osDelay(1000);
        // bms_comm_status();
        // osDelay(1000);
        // bms_uart_test();
        // osDelay(1000);

        uint32_t rxFlag;
        extern osEventFlagsId_t bmsRxEvent;
        rxFlag = osEventFlagsWait(bmsRxEvent, 0x00000001U, osFlagsWaitAny, 300);
        if (rxFlag != 0x00000001U)
        {
            // communication error
            continue;
        }

        bms_comm_data_rx();
    }
}
