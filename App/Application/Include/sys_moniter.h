#ifndef SYS_MONITER_H
#define SYS_MONITER_H

#include "cmsis_os2.h"

#include "pcs_comm.h"
#include "bms_comm.h"
#include "buzzer_beep.h"

typedef enum
{
    SYS_STATUS_STANBY,
    SYS_STATUS_WORK,
    SYS_STATUS_ERROR,
    SYS_STATUS_UPDATE,
} sysStatus_e;

typedef enum
{
    SYS_MODE_NORMAL,
    SYS_MODE_ECHO,
} sysMode_e;

typedef enum
{
    SYS_UPDATE_NONE,
    SYS_UPDATE_PCS,
    SYS_UPDATE_IAP,
    SYS_UPDATE_BMS,
} sysUpdate_e;

typedef struct
{
    uint8_t status;
    void (*deal)(void);
} statusDeal_t;

typedef struct
{
    sysStatus_e preStatus;
    sysStatus_e nowStatus;
    sysMode_e sysMode;
    sysUpdate_e sysUpdate;

    // only for enable, not for present status
    uint8_t chgEn : 1;
    uint8_t acOutEn : 1;
    uint8_t dcOutEn : 1;
    uint8_t usbOutEn : 1;
    uint8_t switchReserve : 4;

    uint16_t chgLimitPwr;
    uint16_t dsgLimitPwr;
}sysData_t;
extern sysData_t sysData;

typedef struct
{
    int8_t temp;
    uint16_t chgLimit;
    uint16_t dsgLimit;
} pwrCurve_t;

void SetSysStatus(sysStatus_e NextStatus);

static uint16_t GetMaxChgLimPwr(int8_t temp);
static uint16_t GetMaxDsgLimPwr(int8_t temp);
static void ChgCtrl(void);
static void AcOutputCtrl(void);

void SysMoniterTask(void *argument);

#endif /* SYS_MONITER_H */
