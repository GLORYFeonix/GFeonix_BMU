#include "key_check.h"

void KeyTimer_Callback(void *arg)
{
    button_ticks();
}

enum Button_IDs
{
    btn_dc_led,
    btn_dc_iot,
    btn_power,
    btn_dc,
    btn_ac,
    btn_led,
    btn_iot,
};

static struct Button dc_ledButton;
static struct Button dc_iotButton;
static struct Button powerButton;
static struct Button dcButton;
static struct Button acButton;
static struct Button ledButton;
static struct Button iotButton;

uint8_t read_button_GPIO(uint8_t button_id)
{
    // you can share the GPIO read function with multiple Buttons
    switch (button_id)
    {
    case btn_dc_led:
        return !gpio_input_bit_get(DC_KEY_PORT, DC_KEY_PIN) && !gpio_input_bit_get(LED_KEY_PORT, LED_KEY_PIN);

    case btn_dc_iot:
        return !gpio_input_bit_get(DC_KEY_PORT, DC_KEY_PIN) && !gpio_input_bit_get(IOT_KEY_PORT, IOT_KEY_PIN);

    case btn_power:
        return gpio_input_bit_get(POWER_KEY_PORT, POWER_KEY_PIN);

    case btn_dc:
        return gpio_input_bit_get(DC_KEY_PORT, DC_KEY_PIN) || !gpio_input_bit_get(LED_KEY_PORT, LED_KEY_PIN) || !gpio_input_bit_get(IOT_KEY_PORT, IOT_KEY_PIN);

    case btn_ac:
        return gpio_input_bit_get(AC_KEY_PORT, AC_KEY_PIN);

    case btn_led:
        return gpio_input_bit_get(LED_KEY_PORT, LED_KEY_PIN) || !gpio_input_bit_get(DC_KEY_PORT, DC_KEY_PIN);

    case btn_iot:
        return gpio_input_bit_get(IOT_KEY_PORT, IOT_KEY_PIN) || !gpio_input_bit_get(DC_KEY_PORT, DC_KEY_PIN);

    default:
        return 0;
    }
}

void key_init(void)
{
    button_init(&dc_ledButton, read_button_GPIO, SET, btn_dc_led);
    button_attach(&dc_ledButton, PRESS_DOWN, Dc_Led_Press_Down_CB);
    button_attach(&dc_ledButton, PRESS_UP, Dc_Led_Press_Up_CB);
    button_attach(&dc_ledButton, LONG_PRESS_START, Dc_Led_Press_Long_CB);
    button_start(&dc_ledButton);

    button_init(&dc_iotButton, read_button_GPIO, SET, btn_dc_iot);
    button_attach(&dc_iotButton, PRESS_DOWN, Dc_Iot_Press_Down_CB);
    button_attach(&dc_iotButton, PRESS_UP, Dc_Iot_Press_Up_CB);
    button_attach(&dc_iotButton, LONG_PRESS_START, Dc_Iot_Press_Long_CB);
    button_start(&dc_iotButton);

    button_init(&powerButton, read_button_GPIO, RESET, btn_power);
    button_attach(&powerButton, SINGLE_CLICK, Power_Single_Click_CB);
    button_attach(&powerButton, DOUBLE_CLICK, Power_Double_Click_CB);
    button_attach(&powerButton, LONG_PRESS_START, Power_Press_Long_CB);
    button_start(&powerButton);

    button_init(&dcButton, read_button_GPIO, RESET, btn_dc);
    button_attach(&dcButton, SINGLE_CLICK, Dc_Single_Click_CB);
    button_attach(&dcButton, DOUBLE_CLICK, Dc_Double_Click_CB);
    button_attach(&dcButton, LONG_PRESS_START, Dc_Press_Long_CB);
    button_start(&dcButton);

    button_init(&acButton, read_button_GPIO, RESET, btn_ac);
    button_attach(&acButton, SINGLE_CLICK, Ac_Single_Click_CB);
    button_start(&acButton);

    button_init(&ledButton, read_button_GPIO, RESET, btn_led);
    button_attach(&ledButton, SINGLE_CLICK, Led_Single_Click_CB);
    button_attach(&ledButton, LONG_PRESS_START, Led_Press_Long_CB);
    button_start(&ledButton);

    button_init(&iotButton, read_button_GPIO, RESET, btn_iot);
    button_attach(&iotButton, SINGLE_CLICK, Iot_Single_Click_CB);
    button_start(&iotButton);
}

static void Dc_Led_Press_Down_CB(void *btn)
{
    dcButton.cb[SINGLE_CLICK] = NULL;
    ledButton.cb[SINGLE_CLICK] = NULL;
}

static void Dc_Led_Press_Up_CB(void *btn)
{
    button_attach(&dcButton, SINGLE_CLICK, Dc_Single_Click_CB);
    button_attach(&ledButton, SINGLE_CLICK, Led_Single_Click_CB);
}

static void Dc_Led_Press_Long_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_LONG);
    Display_OnOff(DISPLAY_ON);
}

static void Dc_Iot_Press_Down_CB(void *btn)
{
    dcButton.cb[SINGLE_CLICK] = NULL;
    iotButton.cb[SINGLE_CLICK] = NULL;
}

static void Dc_Iot_Press_Up_CB(void *btn)
{
    button_attach(&dcButton, SINGLE_CLICK, Dc_Single_Click_CB);
    button_attach(&iotButton, SINGLE_CLICK, Iot_Single_Click_CB);
}

static void Dc_Iot_Press_Long_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_LONG);
    Display_OnOff(DISPLAY_ON);
}

static void Power_Single_Click_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_SHORT);
    Display_OnOff(DISPLAY_TOGGLE);
}

static void Power_Double_Click_CB(void *btn)
{
    buzzer_beep_set(2, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);

    if (sysData.sysMode == SYS_MODE_NORMAL)
    {
        sysData.sysMode = SYS_MODE_ECHO;
    }
    else
    {
        sysData.sysMode = SYS_MODE_NORMAL;
    }
}

static void Power_Press_Long_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_LONG);
    Display_OnOff(DISPLAY_ON);

    bms_comm_sleep();
}

static void Dc_Single_Click_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);

    sysData.dcOutEn = !sysData.dcOutEn;
}

static void Dc_Double_Click_CB(void *btn)
{
    buzzer_beep_set(2, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);

    sysData.usbOutEn = !sysData.usbOutEn;
}

static void Dc_Press_Long_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_LONG);
    Display_OnOff(DISPLAY_ON);
}

static void Ac_Single_Click_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);

    sysData.acOutEn = !sysData.acOutEn;
}

static void Led_Single_Click_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);
}

static void Led_Press_Long_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_LONG);
    Display_OnOff(DISPLAY_ON);
}

static void Iot_Single_Click_CB(void *btn)
{
    buzzer_beep_set(1, BEEP_SHORT);
    Display_OnOff(DISPLAY_ON);
}
