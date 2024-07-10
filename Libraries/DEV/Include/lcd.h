#ifndef LCD_H
#define LCD_H

#include "i2c.h"
#include "gpio.h"

#define LCD_DEVICE_ADDR 0x7C

#define SEG_A BIT(7)
#define SEG_B BIT(3)
#define SEG_C BIT(1)
#define SEG_D BIT(4)
#define SEG_E BIT(5)
#define SEG_F BIT(6)
#define SEG_G BIT(2)

typedef struct
{
    uint8_t ICSET;
    uint8_t DISCTL;
    uint8_t ADSET;
    uint8_t MODESET;
    uint8_t BLKCTL;
    uint8_t APCTL;
    uint8_t EVRSET;
} lcd_cfg;

typedef union
{
    uint8_t all[29];
    struct 
    {
        lcd_cfg cfg;
        uint8_t ram[22];
    }data;
} _lcd_buf;

typedef enum
{
    LCD_INIT,
    LCD_DISPON,
    LCD_RAM_WRITE,
    LCD_DISPOFF
} _lcd_state;

extern _lcd_buf lcd_buf;

static void i2c_lcd_start(void);
static void i2c_lcd_stop(void);
static void i2c_lcd_send_byte(uint8_t data);
static void i2c_lcd_send_buf(void);

void LCD_Init(void);
void LCD_DispOn(void);
void LCD_RamWrite(void);
void LCD_DispOff(void);

typedef enum
{
    LCD_OFF,
    LCD_FLASH,
    LCD_ON,
} lcdOnOff_e;

typedef enum
{
    PORT_H,
    PORT_L,
} portPos_e;

typedef enum
{
    PORT_0 = 0,
    PORT_1,
    PORT_2,
    PORT_3,
    PORT_4,
    PORT_5,
    PORT_6,
    PORT_7,
    PORT_8,
    PORT_9,
    PORT_10,
    PORT_11,
    PORT_12,
    PORT_13,
    PORT_14,
    PORT_15,
    PORT_16,
    PORT_17,
    PORT_18,
    PORT_19,
    PORT_20,
    PORT_21,
    PORT_END,
} port_e;

typedef enum
{
    SEG_INPWR_UNIT, // 个位
    SEG_INPWR_TENS, // 十位
    SEG_INPWR_HUND, // 百位
    SEG_INPWR_THOU, // 千位

    SEG_INTIME_UNIT,  // 个位
    SEG_INTIME_TENS,  // 十位
    SEG_INTIME_TENTH, // 小数点后一位

    SEG_SOC_UNIT, // 个位
    SEG_SOC_TENS, // 十位

    SEG_OUTPWR_UNIT, // 个位
    SEG_OUTPWR_TENS, // 十位
    SEG_OUTPWR_HUND, // 百位
    SEG_OUTPWR_THOU, // 千位

    SEG_OUTTIME_UNIT,  // 个位
    SEG_OUTTIME_TENS,  // 十位
    SEG_OUTTIME_TENTH, // 小数点后一位
} segName_e;

typedef enum
{
    ICON_T4,
    ICON_T3,
    ICON_T5,
    ICON_T1,
    ICON_T6,
    ICON_T7,
    ICON_T2,
    ICON_USB_C,        // s8
    ICON_DC,           // s10
    ICON_50HZ_INC,     // s11
    ICON_TEMP,         // s37
    ICON_OVERLOAD,     // s7
    ICON_USB_A,        // S9
    ICON_50HZ,         // S12
    ICON_HOT,          // S13
    ICON_ECO,          // S0
    ICON_NOROPEN_MODE, // S1
    ICON_CHG_JOIN,     // S3
    ICON_IN,           // S5
    ICON_IN_TIME,      // S4
    ICON_PWR_DOWN,     // S2
    ICON_LONG_LINE,    // S6
    ICON_IN_TIME_P1,   // P1
    ICON_SEG_2,        // S31
    ICON_IN_WATT,      // S35
    ICON_IN_HOUR,      // S34
    ICON_IN_MINUTE,    // S33
    ICON_SEG_1,        // S32
    ICON_SOC_1,        // S36
    ICON_SHORT_LINE,   // S27
    ICON_SEG_4,        // S29
    ICON_SEG_3,        // S30
    ICON_SEG_5,        // S28
    ICON_PERCENT,      // S26
    ICON_OUT,          // S24
    ICON_OUT_TIME,     // S25
    ICON_COLD,         // S14
    ICON_OUT_TIME_P2,  // P2
    ICON_OUT_MINUTE,   // S23
    ICON_OUT_WATT,     // S21
    ICON_OUT_HOUR,     // S22
    ICON_WIFI,         // S20
    ICON_BT,           // S15
} iconName_e;

void LCD_DisplaySeg(uint8_t num, segName_e segName, lcdOnOff_e status);
void LCD_DisplayIcon(iconName_e iconName, lcdOnOff_e status);

#endif /* LCD_H */