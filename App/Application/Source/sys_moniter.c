#include "sys_moniter.h"

sysData_t sysData;

static pwrCurve_t pwrCurveTab[] =
{
    {-20, 125,  600},
    {-19, 125,  600},
    {-18, 125,  600},
    {-17, 125,  600},
    {-16, 125,  600},
    {-15, 125,  600},
    {-14, 125,  600},
    {-13, 125,  600},
    {-12, 125,  600},
    {-11, 125,  600},
    {-10, 125,  600},
    {-9,  125,  600},
    {-8,  125,  600},
    {-7,  125,  600},
    {-6,  125,  600},
    {-5,  125,  600},
    {-4,  125,  600},
    {-3,  125,  600},
    {-2,  125,  600},
    {-1,  125,  600},
    {0,   200,  600},
    {1,   200,  1520},
    {2,   200,  1520},
    {3,   200,  1520},
    {4,   200,  1520},
    {5,   200,  1520},
    {6,   256,  1520},
    {7,   312,  1520},
    {8,   368,  1520},
    {9,   424,  1520},
    {10,  592,  1520},
    {11,  648,  1800},
    {12,  480,  1800},
    {13,  536,  1800},
    {14,  704,  1800},
    {15,  760,  1800},
    {16,  816,  1800},
    {17,  872,  1800},
    {18,  928,  1800},
    {19,  984,  1800},
    {20,  1040, 1800},
    {21,  1200, 1800},
    {22,  1200, 1800},
    {23,  1200, 1800},
    {24,  1200, 1800},
    {25,  1200, 1800},
    {26,  1200, 1800},
    {27,  1200, 1800},
    {28,  1200, 1800},
    {29,  1200, 1800},
    {30,  1200, 1800},
    {31,  1200, 1800},
    {32,  1200, 1800},
    {33,  1200, 1800},
    {34,  1200, 1800},
    {35,  1200, 1800},
    {36,  1200, 1800},
    {37,  1200, 1800},
    {38,  1200, 1800},
    {39,  1200, 1800},
    {40,  1200, 1800},
    {41,  820,  1800},
    {42,  753,  1800},
    {43,  686,  1800},
    {44,  619,  1800},
    {45,  552,  1800},
    {46,  485,  1800},
    {47,  418,  1800},
    {48,  333,  1800},
    {49,  333,  1800},
    {50,  333,  1800},
    {51,  125,  1365},
    {52,  125,  1365},
    {53,  125,  1365},
    {54,  125,  1365},
    {55,  125,  1365},
    {56,  125,  1365},
    {57,  125,  1365},
    {58,  125,  1365},
    {59,  125,  1365},
    {60,  125,  1365},
};

void SetSysStatus(sysStatus_e NextStatus)
{
    sysData.nowStatus = NextStatus;
}

static void SwitchEnable(void)
{
    sysData.chgEn = ENABLE;
    sysData.dcOutEn = ENABLE;
    sysData.usbOutEn = ENABLE;

    // 电量条件
    sysData.chgEn &= bmsData.soc < 1000;
    sysData.acOutEn &= bmsData.soc > 0;
    sysData.dcOutEn &= bmsData.soc > 0;
    sysData.usbOutEn &= bmsData.soc > 0;

    if (sysData.nowStatus != SYS_STATUS_WORK)
    {
        sysData.chgEn &= DISABLE;
        sysData.acOutEn &= DISABLE;
        sysData.dcOutEn &= DISABLE;
        sysData.usbOutEn &= DISABLE;
    }
}

static uint16_t GetMaxChgLimPwr(int8_t temp)
{
    uint16_t chgLimit = 125;
    for (uint8_t i = 0; i < sizeof(pwrCurveTab) / sizeof(pwrCurve_t); i++)
    {
        if (temp == pwrCurveTab[i].temp)
        {
            chgLimit = pwrCurveTab[i].chgLimit;
            break;
        }
    }

    if (bmsData.soc <= 1 || bmsData.sumVolt <= 24000)
    {
        chgLimit = 125;
    }
    
    if (bmsData.soc >= 95 || bmsData.sumVolt <= 28200)
    {
        chgLimit = chgLimit < 333 ? chgLimit : 333;
    }

    return chgLimit;
}

static uint16_t GetMaxDsgLimPwr(int8_t temp)
{
    uint16_t dsgLimit = 600;
    for (uint8_t i = 0; i < sizeof(pwrCurveTab) / sizeof(pwrCurve_t); i++)
    {
        if (temp == pwrCurveTab[i].temp)
        {
            dsgLimit = pwrCurveTab[i].dsgLimit;
            break;
        }
    }

    if (bmsData.soc <= 5 && !pcsData.chgCtrl.bit.chgConnect)
    {
        dsgLimit = dsgLimit < 600 ? dsgLimit : 600;
    }

    return dsgLimit;
}

static void ChgCtrl(void)
{
    if (sysData.chgEn)
    {
        sysData.chgLimitPwr = GetMaxChgLimPwr(bmsData.maxCellTemp);
        SetPcsChgEnable(ENABLE);
    }
    else
    {
        sysData.chgLimitPwr = 0;
        SetPcsChgEnable(DISABLE);
    }
}

static void AcOutputCtrl(void)
{
    if (sysData.acOutEn)
    {
        sysData.dsgLimitPwr = GetMaxDsgLimPwr(bmsData.maxCellTemp);
        SetPcsDsgEnable(ENABLE);
    }
    else
    {
        sysData.dsgLimitPwr = 0;
        SetPcsDsgEnable(DISABLE);
    }
}

void SysMoniterTask(void *argument)
{
    while (1)
    {
        if (sysData.nowStatus ^ sysData.preStatus)
        {
            sysData.preStatus = sysData.nowStatus;
            buzzer_beep_set(1, BEEP_LONG);
        }

        SetSysStatus(SYS_STATUS_WORK);

        SwitchEnable();
        ChgCtrl();
        AcOutputCtrl();

        osDelay(10);
    }
}
