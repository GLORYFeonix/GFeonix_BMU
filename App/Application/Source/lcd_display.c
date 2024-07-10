#include "lcd_display.h"

display_onoff_e display_onoff = DISPLAY_ON;

extern osTimerId_t TimerScreenHalfOff;
extern osTimerId_t TimerScreenFullOff;
extern osTimerId_t TimerScreenBlink;

static _lcd_state lcd_state;
static uint8_t gzyCnt;

static void Display_SOC(uint8_t soc, lcdOnOff_e status)
{
    uint8_t unit = soc % 10;
    uint8_t ten = (soc / 10) % 10;
    uint8_t hundred = soc >= 100;

    if (soc < 10)
    {
        LCD_DisplaySeg(unit, SEG_SOC_UNIT, status);
    }
    if (soc >= 10 && soc < 100)
    {
        LCD_DisplaySeg(unit, SEG_SOC_UNIT, status);
        LCD_DisplaySeg(ten, SEG_SOC_TENS, status);
    }
    else if (soc >= 100)
    {
        LCD_DisplaySeg(unit, SEG_SOC_UNIT, status);
        LCD_DisplaySeg(ten, SEG_SOC_TENS, status);
        LCD_DisplayIcon(ICON_SOC_1, status);
    }
    LCD_DisplayIcon(ICON_PERCENT, status);
}

static void Display_Scale(uint8_t soc, lcdOnOff_e status)
{
    if (soc >= 0 && soc <= 19)
    {
        LCD_DisplayIcon(ICON_SEG_1, status);
    }
    else if (soc >= 20 && soc <= 39)
    {
        LCD_DisplayIcon(ICON_SEG_1, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_2, status);
    }
    else if (soc >= 40 && soc <= 59)
    {
        LCD_DisplayIcon(ICON_SEG_1, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_2, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_3, status);
    }
    else if (soc >= 60 && soc <= 79)
    {
        LCD_DisplayIcon(ICON_SEG_1, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_2, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_3, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_4, status);
    }
    else if (soc >= 80 && soc <= 100)
    {
        LCD_DisplayIcon(ICON_SEG_1, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_2, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_3, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_4, LCD_ON);
        LCD_DisplayIcon(ICON_SEG_5, status);
    }
}

static void Display_ChgRemainTime(uint16_t minutes, lcdOnOff_e status)
{
    uint8_t unit = minutes % 10;
    uint8_t ten = (minutes / 10) % 10;
    uint8_t hundred = minutes >= 100;

    LCD_DisplaySeg(hundred, SEG_INTIME_TENTH, status);
    LCD_DisplaySeg(unit, SEG_INTIME_UNIT, status);
    LCD_DisplaySeg(ten, SEG_INTIME_TENS, status);
}

static void Display_DsgRemainTime(uint16_t minutes, lcdOnOff_e status)
{
    uint8_t unit = minutes % 10;
    uint8_t ten = (minutes / 10) % 10;
    uint8_t hundred = minutes >= 100;

    LCD_DisplaySeg(hundred, SEG_OUTTIME_TENTH, status);
    LCD_DisplaySeg(unit, SEG_OUTTIME_UNIT, status);
    LCD_DisplaySeg(ten, SEG_OUTTIME_TENS, status);
}

static void Display_WorkMode(sysMode_e workMode, lcdOnOff_e status)
{
    switch (workMode)
    {
    case SYS_MODE_NORMAL:
        LCD_DisplayIcon(ICON_NOROPEN_MODE, status);
        break;

    case SYS_MODE_ECHO:
        LCD_DisplayIcon(ICON_ECO, status);
        break;

    default:
        break;
    }
}

static void Display_Usb(lcdOnOff_e status)
{
    LCD_DisplayIcon(ICON_DC, status);
    LCD_DisplayIcon(ICON_USB_A, status);
    LCD_DisplayIcon(ICON_USB_C, status);
    LCD_DisplayIcon(ICON_OVERLOAD, status);
    LCD_DisplayIcon(ICON_LONG_LINE, status);
    LCD_DisplayIcon(ICON_SHORT_LINE, status);
}

static void Display_Hot(lcdOnOff_e status)
{
    LCD_DisplayIcon(ICON_HOT, status);
}

static void Display_Cold(lcdOnOff_e status)
{
    LCD_DisplayIcon(ICON_COLD, status);
}

static void Lcd_ClearAll(void)
{
    memset(lcd_buf.data.ram, 0x00, sizeof(lcd_buf.data.ram));
}

static void Lcd_Refresh(void)
{
    Display_SOC(bmsData.soc, LCD_ON);
    Display_Scale(gzyCnt, (1) ? LCD_FLASH : LCD_OFF); // Is charging?
    Display_ChgRemainTime(gzyCnt, LCD_ON);
    Display_DsgRemainTime(gzyCnt, LCD_ON);
    Display_WorkMode(sysData.sysMode, LCD_ON);
    Display_Usb((1) ? LCD_FLASH : LCD_OFF);
    Display_Hot((1) ? LCD_FLASH : LCD_OFF);
    Display_Cold((1) ? LCD_FLASH : LCD_OFF);
}

void Display_OnOff(display_onoff_e onoff)
{
    switch (onoff)
    {
    case DISPLAY_OFF:
        display_onoff = DISPLAY_OFF;
        lcd_buf.data.cfg.MODESET &= ~(1 << 3);
        break;

    case DISPLAY_HALF:
        display_onoff = DISPLAY_HALF;

        LCD_BL_1_OFF();
        LCD_BL_2_OFF();
        break;

    case DISPLAY_ON:
        display_onoff = DISPLAY_ON;
        lcd_buf.data.cfg.MODESET |= (1 << 3);

        LCD_BL_1_ON();
        LCD_BL_2_ON();

        osTimerStart(TimerScreenHalfOff, 30000);
        osTimerStart(TimerScreenFullOff, 60000);
        break;

    case DISPLAY_TOGGLE:
        if (display_onoff == DISPLAY_OFF)
        {
            Display_OnOff(DISPLAY_ON);
        }
        else if (display_onoff == DISPLAY_HALF)
        {
            Display_OnOff(DISPLAY_ON);
        }
        else
        {
            Display_OnOff(DISPLAY_OFF);
        }
        break;
    
    default:
        break;
    }
}

void ScreenHalfOffTimer_Callback(void *arg)
{
    Display_OnOff(DISPLAY_HALF);
}

void ScreenFullOffTimer_Callback(void *arg)
{
    Display_OnOff(DISPLAY_OFF);
}

void ScreenBlinkTimer_Callback(void *arg)
{
    extern uint8_t lcdBlinkFlag;
    lcdBlinkFlag = !lcdBlinkFlag;

    if (++gzyCnt > 99)
    {
        gzyCnt = 0;
    }
}

void LcdDisplayTask(void *argument)
{
    Display_OnOff(DISPLAY_ON);

    osTimerStart(TimerScreenBlink, 500);

    while (1)
    {
        switch (lcd_state)
        {
        case LCD_INIT:
            LCD_Init();
            lcd_state = LCD_DISPON;
            break;

        case LCD_DISPON:
            LCD_DispOn();
            lcd_state = LCD_RAM_WRITE;
            break;

        case LCD_RAM_WRITE:
            Lcd_ClearAll();
            Lcd_Refresh();
            LCD_RamWrite();
            break;

        case LCD_DISPOFF:
            LCD_DispOff();
            lcd_state = LCD_INIT;
            break;

        default:
            break;
        }
        osDelay(100);
    }
}