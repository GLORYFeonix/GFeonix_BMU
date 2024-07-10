#ifndef PCS_COMM_H
#define PCS_COMM_H

#include "uart.h"

#include "sys_moniter.h"

typedef enum
{
    PCS_VERSION = 0x00,
    PCS_WORK_STATE,
    PCS_AC_DATA,
    PCS_PV_DATA,
    PCS_TEMPERATURE,
    PCS_WARN_INFO,
    PCS_ERROR_INFO,
    PCS_WORK_MODE,
    PCS_OUTPUT_CTRL,
    PCS_FAN_STATUS,
    PCS_CHG_CTRL,
    PCS_CHG_VOLT,
    PCS_CHG_PWR,
    PCS_SET_FREQ,
    PCS_DSG_VOLT,
    PCS_SHUTDOWN,
    PCS_UPGRADE_REQUEST = 0xF0,
}pcs_comm_e;

typedef struct
{
    pcs_comm_e id;
    void (*func)(uart_transmit_dir_e dir);
    uint8_t enable;
} pcs_comm_t;

typedef struct
{
    struct
    {
        uint8_t sw[3];
        uint8_t hw[3];
    } version;
    uint8_t workState;
    struct
    {
        uint16_t acFreq;
        uint16_t acVolt;
        uint16_t acCurr;
        uint16_t acOutPwr;
        uint16_t acInPwr;
    } acOutputData;
    struct
    {
        uint16_t pvVolt;
        uint16_t pvCurr;
        uint16_t pvInPwr;
    } pvInputData;
    struct
    {
        uint16_t invTemp;
        uint16_t boostTemp;
    } pcsTemp;
    union
    {
        uint16_t a : 1;
        uint16_t b : 2;
    } warnInfo;
    union
    {
        uint16_t a : 1;
        uint16_t b : 2;
    } errorInfo;
    uint8_t workMode;
    uint8_t outputCtrl;
    uint8_t fanStatus;
    union
    {
        uint8_t all;
        struct
        {
            uint8_t chgOff : 1;
            uint8_t chgOn : 1;
            uint8_t chgLowNoisesOff : 1;
            uint8_t chgLowNoisesOn : 1;
            uint8_t chgConnect : 1;
        } bit;
    } chgCtrl;
    uint16_t chgVolt;
    uint16_t chgPower;
    uint16_t acFreq;
    uint16_t dsgVolt;
    uint8_t shutdown;
} PcsData;
extern PcsData pcsData;

typedef struct
{
    struct
    {
        uint16_t bmsVolt;
        uint16_t bmsCurr;
        uint16_t cellTemp;
        uint16_t acLimit;
    } acOutputData;
    uint16_t pvCurr;
    uint8_t workMode;
    uint8_t outputCtrl;
    union
    {
        uint8_t all;
        struct
        {
            uint8_t chgOff : 1;
            uint8_t chgOn : 1;
            uint8_t chgLowNoisesOff : 1;
            uint8_t chgLowNoisesOn : 1;
            uint8_t chgConnect : 1;
        } bit;
    } chgCtrl;
    uint16_t chgVolt;
    uint16_t chgPower;
    uint16_t acFreq;
    uint16_t dsgVolt;
} PcsCtrl;
extern PcsCtrl pcsCtrl;

static void pcs_version(uart_transmit_dir_e dir);
static void pcs_workState(uart_transmit_dir_e dir);
static void pcs_acData(uart_transmit_dir_e dir);
static void pcs_pvData(uart_transmit_dir_e dir);
static void pcs_temperature(uart_transmit_dir_e dir);
static void pcs_warnInfo(uart_transmit_dir_e dir);
static void pcs_errorInfo(uart_transmit_dir_e dir);
static void pcs_workMode(uart_transmit_dir_e dir);
static void pcs_outputCtrl(uart_transmit_dir_e dir);
static void pcs_fanStatus(uart_transmit_dir_e dir);
static void pcs_chgCtrl(uart_transmit_dir_e dir);
static void pcs_chgVolt(uart_transmit_dir_e dir);
static void pcs_chgPower(uart_transmit_dir_e dir);
static void pcs_acFreq(uart_transmit_dir_e dir);
static void pcs_dsgVolt(uart_transmit_dir_e dir);
static void pcs_shutdown(uart_transmit_dir_e dir);

static void pcs_receive(uint8_t num, void *data);
static void pcs_send(void *data);

void SetPcsChgEnable(ControlStatus onoff);
void SetPcsDsgEnable(ControlStatus onoff);

void PcsCommTask(void *argument);

#endif /* PCS_COMM_H */
