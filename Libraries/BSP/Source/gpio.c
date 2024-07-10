#include "gpio.h"

void gpio_Init(void)
{
    for (rcu_periph_enum i = RCU_GPIOA; i <= RCU_GPIOG; i++)
    {
        rcu_periph_clock_enable(i);
    }
    rcu_periph_clock_enable(RCU_AF);

    switch_gpio_init();
    key_gpio_init();
    uart_gpio_init();
    i2c_gpio_init();
    pwm_gpio_init();
}

static void switch_gpio_init(void)
{
    gpio_init(MCU_CTRL_5V_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, MCU_CTRL_5V_PIN);
    MCU_CTRL_5V_ENABLE();

    gpio_init(MCU_DCDC_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, MCU_DCDC_PIN);
    MCU_DCDC_ENABLE();

    gpio_init(BLE_ENABLE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, BLE_ENABLE_PIN);
    BLE_ENABLE_ENABLE();

    gpio_init(PCS_REDE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PCS_REDE_PIN);
    gpio_init(BMS_REDE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BMS_REDE_PIN);

    gpio_init(LCD_POWER_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LCD_POWER_PIN);
    LCD_POWER_ON();

    gpio_init(LCD_BL_1_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LCD_BL_1_PIN);
    gpio_init(LCD_BL_2_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, LCD_BL_2_PIN);
    LCD_BL_1_ON();
    LCD_BL_2_ON();
}

static void key_gpio_init(void)
{
    gpio_init(POWER_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, POWER_KEY_PIN);

    gpio_init(DC_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, DC_KEY_PIN);

    gpio_init(AC_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, AC_KEY_PIN);

    gpio_init(LED_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, LED_KEY_PIN);

    gpio_init(IOT_KEY_PORT, GPIO_MODE_IPU, GPIO_OSPEED_2MHZ, IOT_KEY_PIN);
}

static void uart_gpio_init(void)
{
    gpio_init(PCS_UART_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, PCS_UART_RX_PIN);
    gpio_init(PCS_UART_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, PCS_UART_TX_PIN);

    gpio_pin_remap_config(GPIO_USART1_REMAP, ENABLE);
    gpio_init(UPPER_UART_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, UPPER_UART_RX_PIN);
    gpio_init(UPPER_UART_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, UPPER_UART_TX_PIN);

    gpio_pin_remap_config(GPIO_USART2_FULL_REMAP, ENABLE);
    gpio_init(IOT_UART_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, IOT_UART_RX_PIN);
    gpio_init(IOT_UART_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, IOT_UART_TX_PIN);
    gpio_init(IOT_UART_TX_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_2MHZ, IOT_UART_TX_PIN);

    gpio_init(BMS_UART_RX_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, BMS_UART_RX_PIN);
    gpio_init(BMS_UART_TX_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, BMS_UART_TX_PIN);
}

static void i2c_gpio_init(void)
{
    gpio_init(LCD_SCL_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, LCD_SCL_PIN);
    gpio_init(LCD_SDA_PORT, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, LCD_SDA_PIN);
}

static void pwm_gpio_init(void)
{
    gpio_pin_remap_config(GPIO_TIMER8_REMAP, ENABLE);
    gpio_init(BUZZER_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, BUZZER_PIN);
}
