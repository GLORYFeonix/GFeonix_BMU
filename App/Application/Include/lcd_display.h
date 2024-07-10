#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include "lcd.h"

#include "sys_moniter.h"
#include "bms_comm.h"

typedef enum
{
    DISPLAY_OFF,
    DISPLAY_HALF,
    DISPLAY_ON,
    DISPLAY_TOGGLE,
} display_onoff_e;

extern display_onoff_e display_onoff;

static void Display_SOC(uint8_t soc, lcdOnOff_e status);
static void Display_Scale(uint8_t soc, lcdOnOff_e status);
static void Display_ChgRemainTime(uint16_t minutes, lcdOnOff_e status);
static void Display_DsgRemainTime(uint16_t minutes, lcdOnOff_e status);
static void Display_WorkMode(sysMode_e workMode, lcdOnOff_e status);
static void Display_Usb(lcdOnOff_e status);
static void Display_Hot(lcdOnOff_e status);
static void Display_Cold(lcdOnOff_e status);

static void Lcd_ClearAll(void);
static void Lcd_Refresh(void);

void Display_OnOff(display_onoff_e onoff);

void ScreenHalfOffTimer_Callback(void *arg);
void ScreenFullOffTimer_Callback(void *arg);
void ScreenBlinkTimer_Callback(void *arg);

void LcdDisplayTask(void *argument);

#endif /* LCD_DISPLAY_H */
