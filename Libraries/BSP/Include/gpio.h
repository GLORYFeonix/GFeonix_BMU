#ifndef GPIO_H
#define GPIO_H

#include "gd32f30x.h"

#define MCU_CTRL_5V_PORT GPIOB
#define MCU_CTRL_5V_PIN GPIO_PIN_2
#define MCU_CTRL_5V_ENABLE() gpio_bit_set(MCU_CTRL_5V_PORT, MCU_CTRL_5V_PIN)
#define MCU_CTRL_5V_DISABLE() gpio_bit_reset(MCU_CTRL_5V_PORT, MCU_CTRL_5V_PIN)

#define MCU_DCDC_PORT GPIOE
#define MCU_DCDC_PIN GPIO_PIN_7
#define MCU_DCDC_ENABLE() gpio_bit_set(MCU_DCDC_PORT, MCU_DCDC_PIN)
#define MCU_DCDC_DISABLE() gpio_bit_reset(MCU_DCDC_PORT, MCU_DCDC_PIN)

#define BLE_ENABLE_PORT GPIOB
#define BLE_ENABLE_PIN GPIO_PIN_12
#define BLE_ENABLE_ENABLE() gpio_bit_set(BLE_ENABLE_PORT, BLE_ENABLE_PIN)
#define BLE_ENABLE_DISABLE() gpio_bit_reset(BLE_ENABLE_PORT, BLE_ENABLE_PIN)

#define PCS_REDE_PORT GPIOE
#define PCS_REDE_PIN GPIO_PIN_9
#define PCS_485_TX_ENABLE() gpio_bit_set(PCS_REDE_PORT, PCS_REDE_PIN)
#define PCS_485_RX_ENABLE() gpio_bit_reset(PCS_REDE_PORT, PCS_REDE_PIN)

#define BMS_REDE_PORT GPIOE
#define BMS_REDE_PIN GPIO_PIN_1
#define BMS_485_TX_ENABLE() gpio_bit_set(BMS_REDE_PORT, BMS_REDE_PIN)
#define BMS_485_RX_ENABLE() gpio_bit_reset(BMS_REDE_PORT, BMS_REDE_PIN)

#define LCD_POWER_PORT GPIOA
#define LCD_POWER_PIN GPIO_PIN_12
#define LCD_POWER_ON() gpio_bit_set(LCD_POWER_PORT, LCD_POWER_PIN);
#define LCD_POWER_OFF() gpio_bit_reset(LCD_POWER_PORT, LCD_POWER_PIN);

#define LCD_BL_1_PORT GPIOE
#define LCD_BL_1_PIN GPIO_PIN_3
#define LCD_BL_1_ON() gpio_bit_set(LCD_BL_1_PORT, LCD_BL_1_PIN);
#define LCD_BL_1_OFF() gpio_bit_reset(LCD_BL_1_PORT, LCD_BL_1_PIN);

#define LCD_BL_2_PORT GPIOE
#define LCD_BL_2_PIN GPIO_PIN_4
#define LCD_BL_2_ON() gpio_bit_set(LCD_BL_2_PORT, LCD_BL_2_PIN);
#define LCD_BL_2_OFF() gpio_bit_reset(LCD_BL_2_PORT, LCD_BL_2_PIN);

#define POWER_KEY_PORT GPIOA
#define POWER_KEY_PIN GPIO_PIN_2

#define DC_KEY_PORT GPIOD
#define DC_KEY_PIN GPIO_PIN_0

#define AC_KEY_PORT GPIOB
#define AC_KEY_PIN GPIO_PIN_6

#define LED_KEY_PORT GPIOB
#define LED_KEY_PIN GPIO_PIN_7

#define IOT_KEY_PORT GPIOD
#define IOT_KEY_PIN GPIO_PIN_1

#define PCS_UART_RX_PORT GPIOA
#define PCS_UART_RX_PIN GPIO_PIN_10
#define PCS_UART_TX_PORT GPIOA
#define PCS_UART_TX_PIN GPIO_PIN_9

#define UPPER_UART_RX_PORT GPIOD
#define UPPER_UART_RX_PIN GPIO_PIN_6
#define UPPER_UART_TX_PORT GPIOD
#define UPPER_UART_TX_PIN GPIO_PIN_5

#define IOT_UART_RX_PORT GPIOD
#define IOT_UART_RX_PIN GPIO_PIN_9
#define IOT_UART_TX_PORT GPIOD
#define IOT_UART_TX_PIN GPIO_PIN_8

#define BMS_UART_RX_PORT GPIOC
#define BMS_UART_RX_PIN GPIO_PIN_11
#define BMS_UART_TX_PORT GPIOC
#define BMS_UART_TX_PIN GPIO_PIN_10

#define LCD_SCL_PORT GPIOB
#define LCD_SCL_PIN GPIO_PIN_10
#define LCD_SDA_PORT GPIOB
#define LCD_SDA_PIN GPIO_PIN_11

#define BUZZER_PORT GPIOE
#define BUZZER_PIN GPIO_PIN_5

void gpio_Init();

static void switch_gpio_init(void);
static void key_gpio_init(void);
static void uart_gpio_init(void);
static void i2c_gpio_init(void);
static void pwm_gpio_init(void);

#endif /* GPIO_H */