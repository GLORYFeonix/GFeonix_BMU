#include "lcd.h"

_lcd_buf lcd_buf =
    {
        .data.cfg.ADSET = 0x00,
        .data.cfg.APCTL = 0xF8,
        .data.cfg.BLKCTL = 0xF0,
        .data.cfg.DISCTL = 0xB2,
        .data.cfg.EVRSET = 0xE0,
        .data.cfg.ICSET = 0xEA,
        .data.cfg.MODESET = 0xC8};

const uint8_t SegNumTab[] =
{
    (SEG_A| SEG_B| SEG_C| SEG_D| SEG_E| SEG_F| 0),      // 0
    (0    | SEG_B| SEG_C| 0    | 0    | 0    | 0),      // 1
    (SEG_A| SEG_B| 0    | SEG_D| SEG_E| 0    | SEG_G),  // 2
    (SEG_A| SEG_B| SEG_C| SEG_D| 0    | 0    | SEG_G),  // 3
    (0    | SEG_B| SEG_C| 0    | 0    | SEG_F| SEG_G),  // 4
    (SEG_A| 0    | SEG_C| SEG_D| 0    | SEG_F| SEG_G),  // 5
    (SEG_A| 0    | SEG_C| SEG_D| SEG_E| SEG_F| SEG_G),  // 6
    (SEG_A| SEG_B| SEG_C| 0    | 0    | 0    | 0),      // 7
    (SEG_A| SEG_B| SEG_C| SEG_D| SEG_E| SEG_F| SEG_G),  // 8
    (SEG_A| SEG_B| SEG_C| SEG_D| 0    | SEG_F| SEG_G),  // 9
};

typedef struct
{
    segName_e segName;
    port_e dataLBytePort;   // 数据低字节端口
    portPos_e dataLBytePos; // 存放在端口的位置
    port_e dataHBytePort;   // 数据高字节端口
    portPos_e dataHBytePos; // 存放在端口的位置
} seg_tab_t;

const seg_tab_t SegPosTab[] =
{
    {SEG_INPWR_UNIT   , PORT_7 , PORT_H, PORT_6 , PORT_L},
    {SEG_INPWR_TENS   , PORT_6 , PORT_H, PORT_5 , PORT_L},
    {SEG_INPWR_HUND   , PORT_5 , PORT_H, PORT_4 , PORT_L},
    {SEG_INPWR_THOU   , PORT_4 , PORT_H, PORT_3 , PORT_L},
    
    {SEG_INTIME_UNIT  , PORT_9 , PORT_H, PORT_8 , PORT_L},
    {SEG_INTIME_TENS  , PORT_8 , PORT_H, PORT_7 , PORT_L},
    {SEG_INTIME_TENTH , PORT_10, PORT_H, PORT_9 , PORT_L},

    {SEG_SOC_UNIT     , PORT_13, PORT_H, PORT_12, PORT_L},
    {SEG_SOC_TENS     , PORT_12, PORT_H, PORT_11, PORT_L},
    {SEG_OUTPWR_UNIT  , PORT_17, PORT_L, PORT_17, PORT_H},
    {SEG_OUTPWR_TENS  , PORT_16, PORT_L, PORT_16, PORT_H},
    {SEG_OUTPWR_HUND  , PORT_15, PORT_L, PORT_15, PORT_H},
    {SEG_OUTPWR_THOU  , PORT_14, PORT_L, PORT_14, PORT_H},
    
    {SEG_OUTTIME_TENS , PORT_19, PORT_L, PORT_19, PORT_H},
    {SEG_OUTTIME_UNIT , PORT_18, PORT_L, PORT_18, PORT_H},
    {SEG_OUTTIME_TENTH, PORT_20, PORT_L, PORT_20, PORT_H},
};

typedef struct
{
    iconName_e name;
    port_e port;
    uint8_t value;
} iconTab_t;

const iconTab_t iconTab[] =
{
    {ICON_T4          , PORT_0 , BIT(7)}, 
    {ICON_T3          , PORT_0 , BIT(6)}, 
    {ICON_T5          , PORT_0 , BIT(3)}, 
    {ICON_T1          , PORT_0 , BIT(2)}, 
    {ICON_T6          , PORT_1 , BIT(7)}, 
    {ICON_T7          , PORT_1 , BIT(6)}, 
    {ICON_T2          , PORT_1 , BIT(5)}, 
    {ICON_USB_C       , PORT_1 , BIT(3)}, // s8
    {ICON_DC          , PORT_1 , BIT(2)}, // s10
    {ICON_50HZ_INC    , PORT_1 , BIT(1)}, // s11
    {ICON_TEMP        , PORT_1 , BIT(0)}, // s37
    {ICON_OVERLOAD    , PORT_2 , BIT(7)}, // s7
    {ICON_USB_A       , PORT_2 , BIT(6)}, // S9
    {ICON_50HZ        , PORT_2 , BIT(5)}, // S12
    {ICON_HOT         , PORT_2 , BIT(4)}, // S13
    {ICON_ECO         , PORT_2 , BIT(3)}, // S0
    {ICON_NOROPEN_MODE, PORT_2 , BIT(2)}, // S1
    {ICON_CHG_JOIN    , PORT_2 , BIT(1)}, // S3
    {ICON_IN          , PORT_3 , BIT(7)}, // S5
    {ICON_IN_TIME     , PORT_3 , BIT(6)}, // S4
    {ICON_PWR_DOWN    , PORT_3 , BIT(5)}, // S2
    {ICON_LONG_LINE   , PORT_3 , BIT(4)}, // S6
    {ICON_IN_TIME_P1  , PORT_9 , BIT(4)}, // P1
    {ICON_SEG_2       , PORT_10, BIT(4)}, // S31
    {ICON_IN_WATT     , PORT_10, BIT(3)}, // S35
    {ICON_IN_HOUR     , PORT_10, BIT(2)}, // S34
    {ICON_IN_MINUTE   , PORT_10, BIT(1)}, // S33
    {ICON_SEG_1       , PORT_10, BIT(0)}, // S32
    {ICON_SOC_1       , PORT_11, BIT(7)}, // S36
    {ICON_SHORT_LINE  , PORT_11, BIT(6)}, // S27
    {ICON_SEG_4       , PORT_11, BIT(5)}, // S29
    {ICON_SEG_3       , PORT_11, BIT(4)}, // S30
    {ICON_SEG_5       , PORT_12, BIT(4)}, // S28
    {ICON_PERCENT     , PORT_13, BIT(4)}, // S26
    {ICON_OUT         , PORT_13, BIT(3)}, // S24
    {ICON_OUT_TIME    , PORT_13, BIT(2)}, // S25
    {ICON_COLD        , PORT_18, BIT(0)}, // S14
    {ICON_OUT_TIME_P2 , PORT_19, BIT(0)}, // P2
    {ICON_OUT_MINUTE  , PORT_20, BIT(0)}, // S23
    {ICON_OUT_WATT    , PORT_21, BIT(7)}, // S21
    {ICON_OUT_HOUR    , PORT_21, BIT(6)}, // S22
    {ICON_WIFI        , PORT_21, BIT(5)}, // S20
    {ICON_BT          , PORT_21, BIT(4)}, // S15
};

uint8_t lcdBlinkFlag;

static void i2c_lcd_start(void)
{
    /* wait until I2C bus is idle */
    while (i2c_flag_get(I2C1, I2C_FLAG_I2CBSY))
        ;
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    /* wait until SBSEND bit is set */
    while (!i2c_flag_get(I2C1, I2C_FLAG_SBSEND))
        ;
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, LCD_DEVICE_ADDR, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while (!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND))
        ;
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while (!i2c_flag_get(I2C1, I2C_FLAG_TBE))
        ;
}

static void i2c_lcd_stop(void)
{
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C1);
    /* wait until stop condition generate */
    while (I2C_CTL0(I2C1) & 0x0200)
        ;
}

static void i2c_lcd_send_byte(uint8_t data)
{
    /* data transmission */
    i2c_data_transmit(I2C1, data);
    /* wait until the TBE bit is set */
    while (!i2c_flag_get(I2C1, I2C_FLAG_TBE))
        ;
}

static void i2c_lcd_send_buf(void)
{
    // /* initialize DMA channel3 */
    // dma_deinit(DMA0, DMA_CH3);
    // dma_parameter_struct dma_init_struct;
    // dma_struct_para_init(&dma_init_struct);

    // dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    // dma_init_struct.memory_addr = (uint32_t)lcd_buf.data.ram;
    // dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    // dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    // dma_init_struct.periph_addr = (uint32_t)&I2C_DATA(I2C1);
    // dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    // dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    // dma_init_struct.number = sizeof(lcd_buf.data.ram);
    // dma_init_struct.priority = DMA_PRIORITY_LOW;
    // dma_init(DMA0, DMA_CH3, &dma_init_struct);

    // // dma_channel_disable(DMA0, DMA_CH3);
    // // dma_memory_address_config(DMA0, DMA_CH3, (uint32_t)lcd_buf.data.ram);
    // // dma_transfer_number_config(DMA0, DMA_CH3, sizeof(lcd_buf.data.ram));

    // /* enable DMA0 channel1 */
    // dma_channel_enable(DMA0, DMA_CH3);
    // /* DMA0 channel3 full transfer finish flag */
    // while (!dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF))
    //     ;
    for (uint8_t i = 0; i < 22; i++)
    {
        i2c_lcd_send_byte(lcd_buf.data.ram[i]);
    }
}

void LCD_Init(void)
{
    i2c_lcd_start();

    lcd_buf.data.cfg.ICSET = 0xEA;
    i2c_lcd_send_byte(lcd_buf.data.cfg.ICSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.MODESET);
    lcd_buf.data.cfg.ICSET = 0xE8;
    i2c_lcd_send_byte(lcd_buf.data.cfg.ICSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.ADSET);

    i2c_lcd_send_buf();

    i2c_lcd_stop();
}

void LCD_DispOn(void)
{
    i2c_lcd_start();

    i2c_lcd_send_byte(lcd_buf.data.cfg.ICSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.DISCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.BLKCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.APCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.EVRSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.MODESET);

    i2c_lcd_stop();
}

void LCD_RamWrite(void)
{
    i2c_lcd_start();

    i2c_lcd_send_byte(lcd_buf.data.cfg.DISCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.BLKCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.APCTL);
    i2c_lcd_send_byte(lcd_buf.data.cfg.EVRSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.MODESET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.ICSET);
    i2c_lcd_send_byte(lcd_buf.data.cfg.ADSET);

    i2c_lcd_send_buf();

    i2c_lcd_stop();
}

void LCD_DispOff(void)
{

}

void LCD_DisplaySeg(uint8_t num, segName_e segName, lcdOnOff_e status)
{
    const seg_tab_t *pTab = &SegPosTab[segName];
    switch (status)
    {
    case LCD_ON:
        if (pTab->dataHBytePos == PORT_H)
        {
            lcd_buf.data.ram[pTab->dataHBytePort] |= ((0xF0 & SegNumTab[num]));
        }
        else
        {
            lcd_buf.data.ram[pTab->dataHBytePort] |= (0x0F & (SegNumTab[num] >> 4));
        }
        if (pTab->dataLBytePos == PORT_H)
        {
            lcd_buf.data.ram[pTab->dataLBytePort] |= (0xF0 & (SegNumTab[num] << 4));
        }
        else
        {
            lcd_buf.data.ram[pTab->dataLBytePort] |= (0x0F & (SegNumTab[num]));
        }
        break;

    case LCD_OFF:
        if (pTab->dataHBytePos == PORT_H)
        {
            lcd_buf.data.ram[pTab->dataHBytePort] &= ~((0xF0 & SegNumTab[num]));
        }
        else
        {
            lcd_buf.data.ram[pTab->dataHBytePort] &= ~(0x0F & (SegNumTab[num] >> 4));
        }
        if (pTab->dataLBytePos == PORT_H)
        {
            lcd_buf.data.ram[pTab->dataLBytePort] &= ~(0xF0 & (SegNumTab[num] << 4));
        }
        else
        {
            lcd_buf.data.ram[pTab->dataLBytePort] &= ~(0x0F & (SegNumTab[num]));
        }
        break;

    case LCD_FLASH:
        if (lcdBlinkFlag) // 置位才亮
        {
            if (pTab->dataHBytePos == PORT_H)
            {
                lcd_buf.data.ram[pTab->dataHBytePort] |= ((0xF0 & SegNumTab[num]));
            }
            else
            {
                lcd_buf.data.ram[pTab->dataHBytePort] |= (0x0F & (SegNumTab[num] >> 4));
            }
            if (pTab->dataLBytePos == PORT_H)
            {
                lcd_buf.data.ram[pTab->dataLBytePort] |= (0xF0 & (SegNumTab[num] << 4));
            }
            else
            {
                lcd_buf.data.ram[pTab->dataLBytePort] |= (0x0F & (SegNumTab[num]));
            }
        }
        else
        {
        }
        break;

    default:
        break;
    }
}

void LCD_DisplayIcon(iconName_e iconName, lcdOnOff_e status)
{
    const iconTab_t *pTab = &iconTab[iconName];

    switch (status)
    {
    case LCD_OFF:
        lcd_buf.data.ram[pTab->port] &= ~pTab->value;
        break;

    case LCD_ON:
        lcd_buf.data.ram[pTab->port] |= pTab->value;
        break;

    case LCD_FLASH:
        if (lcdBlinkFlag)
            lcd_buf.data.ram[pTab->port] |= pTab->value;
        else
            lcd_buf.data.ram[pTab->port] &= ~pTab->value;
        break;

    default:
        break;
    }
}
