#include "pcs_comm.h"

PcsData pcsData;
PcsCtrl pcsCtrl;

static pcs_comm_t pcsComm[] =
{
    {PCS_VERSION    , pcs_version    , DISABLE},
    {PCS_WORK_STATE , pcs_workState  , ENABLE},
    {PCS_AC_DATA    , pcs_acData     , ENABLE},
    {PCS_PV_DATA    , pcs_pvData     , ENABLE},
    {PCS_TEMPERATURE, pcs_temperature, ENABLE},
    {PCS_WARN_INFO  , pcs_warnInfo   , ENABLE},
    {PCS_ERROR_INFO , pcs_errorInfo  , ENABLE},
    {PCS_WORK_MODE  , pcs_workMode   , ENABLE},
    {PCS_OUTPUT_CTRL, pcs_outputCtrl , DISABLE},
    {PCS_FAN_STATUS , pcs_fanStatus  , ENABLE},
    {PCS_CHG_CTRL   , pcs_chgCtrl    , DISABLE},
    {PCS_CHG_VOLT   , pcs_chgVolt    , DISABLE},
    {PCS_CHG_PWR    , pcs_chgPower   , DISABLE},
    {PCS_SET_FREQ   , pcs_acFreq     , DISABLE},
    {PCS_DSG_VOLT   , pcs_dsgVolt    , DISABLE},
    {PCS_DSG_VOLT   , pcs_shutdown   , DISABLE},
};
pcs_comm_t pcsUpdate[] =
{
    {PCS_UPGRADE_REQUEST, pcs_version, ENABLE},
};
pcs_comm_t *pcsComm_p;
static uint8_t pcsIndex;

#pragma pack(1)
static struct PcsHead
{
    uint16_t header;
    uint8_t srcAddr;
    uint8_t dstAddr;
    uint8_t serviceId;
    uint16_t dateLen;
} pcsHead = {0xA55A, 0xC0, 0xD0, 0x00, 0x00};
#pragma pack()

static void pcs_version(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.version), &pcsData.version);
    }
    else
    {
        pcsHead.serviceId = PCS_VERSION;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_workState(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.workState), &pcsData.workState);
    }
    else
    {
        pcsHead.serviceId = PCS_WORK_STATE;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_acData(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.acOutputData), &pcsData.acOutputData);
    }
    else
    {
        pcsHead.serviceId = PCS_AC_DATA;
        pcsHead.dateLen = 8;
        pcs_send(&pcsCtrl.acOutputData);
    }
}

static void pcs_pvData(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.pvInputData), &pcsData.pvInputData);
    }
    else
    {
        pcsHead.serviceId = PCS_PV_DATA;
        pcsHead.dateLen = 2;
        pcs_send(&pcsCtrl.pvCurr);
    }
}

static void pcs_temperature(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.pcsTemp), &pcsData.pcsTemp);
    }
    else
    {
        pcsHead.serviceId = PCS_TEMPERATURE;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_warnInfo(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.warnInfo), &pcsData.warnInfo);
    }
    else
    {
        pcsHead.serviceId = PCS_WARN_INFO;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_errorInfo(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.errorInfo), &pcsData.errorInfo);
    }
    else
    {
        pcsHead.serviceId = PCS_ERROR_INFO;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_workMode(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.workMode), &pcsData.workMode);

        if (pcsData.workMode == pcsCtrl.workMode)
        {
            pcsComm[PCS_WORK_MODE].enable = DISABLE;
        }
    }
    else
    {
        pcsHead.serviceId = PCS_WORK_MODE;
        pcsHead.dateLen = 1;
        pcs_send(&pcsCtrl.workMode);
    }
}

static void pcs_outputCtrl(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.outputCtrl), &pcsData.outputCtrl);

        if (pcsData.outputCtrl == pcsCtrl.outputCtrl)
        {
            pcsComm[PCS_OUTPUT_CTRL].enable = DISABLE;
        }
    }
    else
    {
        pcsHead.serviceId = PCS_OUTPUT_CTRL;
        pcsHead.dateLen = 1;
        pcs_send(&pcsCtrl.outputCtrl);
    }
}

static void pcs_fanStatus(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.fanStatus), &pcsData.fanStatus);
    }
    else
    {
        pcsHead.serviceId = PCS_FAN_STATUS;
        pcsHead.dateLen = 0;
        pcs_send(NULL);
    }
}

static void pcs_chgCtrl(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.chgCtrl), &pcsData.chgCtrl);

        if (pcsData.chgCtrl.all == pcsCtrl.chgCtrl.all)
        {
            pcsComm[PCS_OUTPUT_CTRL].enable = DISABLE;
        }
    }
    else
    {
        pcsHead.serviceId = PCS_CHG_CTRL;
        pcsHead.dateLen = 1;
        pcs_send(&pcsCtrl.chgCtrl);
    }
}

static void pcs_chgVolt(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.chgVolt), &pcsData.chgVolt);
    }
    else
    {
        pcsHead.serviceId = PCS_CHG_VOLT;
        pcsHead.dateLen = 2;
        pcs_send(&pcsCtrl.chgVolt);
    }
}

static void pcs_chgPower(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.chgPower), &pcsData.chgPower);
    }
    else
    {
        pcsHead.serviceId = PCS_CHG_PWR;
        pcsHead.dateLen = 2;
        pcs_send(&pcsCtrl.chgPower);
    }
}

static void pcs_acFreq(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.acFreq), &pcsData.acFreq);
    }
    else
    {
        pcsHead.serviceId = PCS_SET_FREQ;
        pcsHead.dateLen = 2;
        pcs_send(&pcsCtrl.acFreq);
    }
}

static void pcs_dsgVolt(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.dsgVolt), &pcsData.dsgVolt);
    }
    else
    {
        pcsHead.serviceId = PCS_DSG_VOLT;
        pcsHead.dateLen = 2;
        pcs_send(&pcsCtrl.dsgVolt);
    }
}

static void pcs_shutdown(uart_transmit_dir_e dir)
{
    if (dir == TRANS_RX)
    {
        pcs_receive(sizeof(pcsData.shutdown), &pcsData.shutdown);
    }
    else
    {
        pcsHead.serviceId = PCS_SHUTDOWN;
        pcsHead.dateLen = 2;
        uint16_t shutdown = 0xA501;
        pcs_send(&shutdown);
    }
}

static void pcs_receive(uint8_t num, void *data)
{
    uart_receive(PCS_UART, data, num);
}

static void pcs_send(void *data)
{
    uint16_t len = 0;
    uint16_t crc = 0;

    txBuf0[len++] = (uint8_t)(pcsHead.header >> 0);
    txBuf0[len++] = (uint8_t)(pcsHead.header >> 8);
    txBuf0[len++] = pcsHead.srcAddr;
    txBuf0[len++] = pcsHead.dstAddr;
    txBuf0[len++] = pcsHead.serviceId;
    txBuf0[len++] = (uint8_t)(pcsHead.dateLen >> 0);
    txBuf0[len++] = (uint8_t)(pcsHead.dateLen >> 8);
    if (data != NULL)
    {
        for (uint16_t i = 0; i < pcsHead.dateLen; i++)
        {
            txBuf0[len++] = *((uint8_t *)data + i);
        }
    }
    crc = crc16_modbus(&txBuf0[2], len - 2);
    txBuf0[len++] = (uint8_t)(crc >> 0);
    txBuf0[len++] = (uint8_t)(crc >> 8);

    uart_send(PCS_UART, len);
}

void SetPcsChgEnable(ControlStatus onoff)
{
    if (onoff)
    {
        pcsCtrl.chgCtrl.all = 0x02;
    }
    else
    {
        pcsCtrl.chgCtrl.all = 0x01;
    }
    pcsCtrl.chgPower = sysData.chgLimitPwr;

    pcsComm[PCS_CHG_PWR].enable = ENABLE;
    pcsComm[PCS_CHG_CTRL].enable = ENABLE;
}

void SetPcsDsgEnable(ControlStatus onoff)
{
    if (onoff)
    {
        pcsCtrl.outputCtrl = 0b10;
    }
    else
    {
        pcsCtrl.outputCtrl = 0b01;
    }
    pcsCtrl.acOutputData.acLimit = sysData.dsgLimitPwr;
    pcsCtrl.workMode = 0b10;

    pcsComm[PCS_WORK_MODE].enable = ENABLE;
    pcsComm[PCS_AC_DATA].enable = ENABLE;
    pcsComm[PCS_OUTPUT_CTRL].enable = ENABLE;
}

void PcsCommTask(void *argument)
{
    while (1)
    {
        if (sysData.sysUpdate == SYS_UPDATE_PCS) // Is updating?
        {
            pcsComm_p = pcsUpdate;
        }
        else
        {
            pcsComm_p = pcsComm;
        }

        if (++pcsIndex >= (sizeof(pcsComm) / sizeof(pcsComm[0])))
        {
            pcsIndex = 0;
        }
        if (pcsComm_p[pcsIndex].enable == ENABLE)
        {
            pcsComm_p[pcsIndex].func(TRANS_TX);

            uint32_t rxFlag;
            extern osEventFlagsId_t pcsRxEvent;
            rxFlag = osEventFlagsWait(pcsRxEvent, 0x00000001U, osFlagsWaitAny, 300);
            if (rxFlag != 0x00000001U)
            {
                // communication error
                continue;
            }

            pcsComm_p[pcsIndex].func(TRANS_RX);
            osDelay(10);
        }
    }
}
