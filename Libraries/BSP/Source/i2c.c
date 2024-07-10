#include "i2c.h"

void i2c_Init(void)
{
    I2C1_Init();
}

static void I2C1_Init(void)
{
    rcu_periph_clock_enable(RCU_I2C1);

    /* cofigure I2C clock */
    i2c_clock_config(I2C1, 100000, I2C_DTCY_2);
    /* cofigure I2C address */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C1_OWN_ADDRESS7);
    uint16_t i2cDelay = 1;
    while (i2cDelay++ != 0)
    {
    }
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);

    // /* initialize DMA channel3 */
    // dma_deinit(DMA0, DMA_CH3);
    // dma_parameter_struct dma_init_struct;
    // dma_struct_para_init(&dma_init_struct);

    // dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    // dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    // dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    // dma_init_struct.periph_addr = (uint32_t)&I2C_DATA(I2C1);
    // dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    // dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    // dma_init_struct.priority = DMA_PRIORITY_LOW;
    // dma_init(DMA0, DMA_CH3, &dma_init_struct);

    // /* enable I2C1 DMA */
    // i2c_dma_config(I2C1, I2C_DMA_ON);
}

/* 
void I2C1_ReInit(void)
{
    gpio_bit_set(LCD_SCL_PORT, LCD_SCL_PIN);
    gpio_bit_set(LCD_SDA_PORT, LCD_SDA_PIN);

    gpio_init(LCD_SCL_PORT, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, LCD_SCL_PIN);
    gpio_init(LCD_SDA_PORT, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, LCD_SDA_PIN);

    for (uint8_t i = 0; 9 < i; i++)
    {
        gpio_bit_reset(LCD_SCL_PORT, LCD_SCL_PIN);
        osDelay(1);
        gpio_bit_set(LCD_SCL_PORT, LCD_SCL_PIN);
        osDelay(1);
    }

    gpio_init(LCD_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, LCD_SCL_PIN);
    gpio_init(LCD_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, LCD_SDA_PIN);

    I2C1_Init();
} 
*/
