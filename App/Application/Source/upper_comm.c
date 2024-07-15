#include "upper_comm.h"

static void upper_SocSoh(uint8_t *data)
{
    *(data + 0) = (bmsData.soc >> 0);
    *(data + 1) = (bmsData.soc >> 8);
    *(data + 2) = (bmsData.soh >> 0);
    *(data + 3) = (bmsData.soh >> 8);
}

void UpperCommTask(void *argument)
{
    while (1)
    {
        uint32_t rxFlag;
        extern osEventFlagsId_t upperRxEvent;
        rxFlag = osEventFlagsWait(upperRxEvent, 0x00000001U, osFlagsWaitAny, 300);
        if (rxFlag != 0x00000001U)
        {
            continue;
        }

        uint8_t buf[3];
        uart_receive(UPPER_UART, buf, 3);
        uint8_t serviceId = buf[0];
        uint16_t dataLen = ((uint16_t)buf[1] << 8) | ((uint16_t)buf[2] << 0);
        if (serviceId == 0x00)
        {
            continue;
        }

        memset(txBuf1, 0x00, sizeof(txBuf1));
        uint8_t len = 0;
        uint16_t crc;
        txBuf1[len++] = 0x5A;
        txBuf1[len++] = 0xA5;
        txBuf1[len++] = 0xC0;
        txBuf1[len++] = 0xA0;
        txBuf1[len++] = serviceId;
        txBuf1[len++] = (uint8_t)(UPPER_DATA_PER_FRAME >> 0);
        txBuf1[len++] = (uint8_t)(UPPER_DATA_PER_FRAME >> 8);

        switch (serviceId)
        {
        case 0x01:
            upper_SocSoh(&txBuf1[len]);
            break;

        default:
            break;
        }

        crc = crc16_modbus(&txBuf1[2], UPPER_FRAME_LEN - 4);
        txBuf1[UPPER_FRAME_LEN - 2] = (uint8_t)(crc >> 0);
        txBuf1[UPPER_FRAME_LEN - 1] = (uint8_t)(crc >> 8);

        uart_send(UPPER_UART, UPPER_FRAME_LEN);
    }
}
