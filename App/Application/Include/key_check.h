#ifndef KEY_CHECK_H
#define KEY_CHECK_H

#include "gd32f30x.h"
#include "cmsis_os2.h"

#include "multi_button.h"

#include "gpio.h"

#include "pcs_comm.h"
#include "bms_comm.h"
#include "lcd_display.h"
#include "buzzer_beep.h"

void KeyTimer_Callback(void *arg);

uint8_t read_button_GPIO(uint8_t button_id);
void key_init(void);

static void Dc_Led_Press_Down_CB(void *btn);
static void Dc_Led_Press_Up_CB(void *btn);
static void Dc_Led_Press_Long_CB(void *btn);

static void Dc_Iot_Press_Down_CB(void *btn);
static void Dc_Iot_Press_Up_CB(void *btn);
static void Dc_Iot_Press_Long_CB(void *btn);

static void Power_Single_Click_CB(void *btn);
static void Power_Double_Click_CB(void *btn);
static void Power_Press_Long_CB(void *btn);

static void Dc_Single_Click_CB(void *btn);
static void Dc_Double_Click_CB(void *btn);
static void Dc_Press_Long_CB(void *btn);

static void Ac_Single_Click_CB(void *btn);

static void Led_Single_Click_CB(void *btn);
static void Led_Press_Long_CB(void *btn);

static void Iot_Single_Click_CB(void *btn);

#endif /* KEY_CHECK_H */
