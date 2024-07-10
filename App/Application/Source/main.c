#include "main.h"

osThreadId_t ThreadTest;
osThreadId_t ThreadSysMoniter;
osThreadId_t ThreadUpperComm;
osThreadId_t ThreadIotComm;
osThreadId_t ThreadPcsComm;
osThreadId_t ThreadBmsComm;
osThreadId_t ThreadLcdDisplay;
osThreadId_t ThreadBuzzerBeep;
osMutexId_t MutexDma0Ch3;
osTimerId_t TimerKey;
osTimerId_t TimerScreenHalfOff;
osTimerId_t TimerScreenFullOff;
osTimerId_t TimerScreenBlink;

const osThreadAttr_t ThreadTest_Attr =
    {
        .name = "TestThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityNormal,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadSysMoniter_Attr =
    {
        .name = "SysMoniterThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityHigh,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadUpperComm_Attr =
    {
        .name = "UpperCommThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityHigh,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadIotComm_Attr =
    {
        .name = "IotCommThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityHigh,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadPcsComm_Attr =
    {
        .name = "PcsCommThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityHigh,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadBmsComm_Attr =
    {
        .name = "BmsCommThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityHigh,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadLcdDisplay_Attr =
    {
        .name = "LcdDisplayThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityLow,
        .stack_size = 2048,
};

const osThreadAttr_t ThreadBuzzerBeep_Attr =
    {
        .name = "BuzzerBeepThread",
        .attr_bits = osThreadDetached,
        .priority = osPriorityLow,
        .stack_size = 2048,
};

const osMutexAttr_t MutexDma0Ch3_Attr =
    {
        "Dma0Ch3Mutex",
        osMutexRecursive | osMutexPrioInherit,
        NULL,
        0U,
};

const osTimerAttr_t TimerKey_Attr =
    {
        "KeyTimer",
        0,
        NULL,
        0U,
};

const osTimerAttr_t TimerScreenHalfOff_Attr =
    {
        "ScreenHalfOffTimer",
        0,
        NULL,
        0U,
};

const osTimerAttr_t TimerScreenFullOff_Attr =
    {
        "ScreenFullOffTimer",
        0,
        NULL,
        0U,
};

const osTimerAttr_t TimerScreenBlink_Attr =
    {
        "ScreenBlinkTimer",
        0,
        NULL,
        0U,
};

osEventFlagsId_t upperRxEvent;
osEventFlagsId_t pcsRxEvent;
osEventFlagsId_t iotRxEvent;
osEventFlagsId_t bmsRxEvent;

int main(void)
{
    __disable_irq();

    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x8003000);

    systick_config();

    bsp_Init();
    key_init();

    __enable_irq();

    osKernelInitialize();

    ThreadTest = osThreadNew(TestTask, NULL, &ThreadTest_Attr);
    ThreadSysMoniter = osThreadNew(SysMoniterTask, NULL, &ThreadSysMoniter_Attr);
    ThreadUpperComm = osThreadNew(UpperCommTask, NULL, &ThreadUpperComm_Attr);
    // ThreadIotComm = osThreadNew(IotCommTask, NULL, &ThreadIotComm_Attr);
    ThreadPcsComm = osThreadNew(PcsCommTask, NULL, &ThreadPcsComm_Attr);
    ThreadBmsComm = osThreadNew(BmsCommTask, NULL, &ThreadBmsComm_Attr);
    ThreadLcdDisplay = osThreadNew(LcdDisplayTask, NULL, &ThreadLcdDisplay_Attr);
    ThreadBuzzerBeep = osThreadNew(BuzzerBeepTask, NULL, &ThreadBuzzerBeep_Attr);

    MutexDma0Ch3 = osMutexNew(&MutexDma0Ch3_Attr);

    TimerKey = osTimerNew(KeyTimer_Callback, osTimerPeriodic, NULL, &TimerKey_Attr);
    TimerScreenHalfOff = osTimerNew(ScreenHalfOffTimer_Callback, osTimerOnce, NULL, &TimerScreenHalfOff_Attr);
    TimerScreenFullOff = osTimerNew(ScreenFullOffTimer_Callback, osTimerOnce, NULL, &TimerScreenFullOff_Attr);
    TimerScreenBlink = osTimerNew(ScreenBlinkTimer_Callback, osTimerPeriodic, NULL, &TimerScreenBlink_Attr);

    upperRxEvent = osEventFlagsNew(NULL);
    iotRxEvent = osEventFlagsNew(NULL);
    pcsRxEvent = osEventFlagsNew(NULL);
    bmsRxEvent = osEventFlagsNew(NULL);

    osKernelStart();

    while (1)
    {
        // LOGD("DEBUG");
        // LOGI("INFO");
        // LOGW("WARN");
        // LOGE("ERROR");
        // LOGF("FATAL");
    }
}

void TestTask(void *argument)
{
    if (TimerKey != NULL)
    {
        osTimerStart(TimerKey, 5); // 移到开机动画后
    }

    while (1)
    {
        // printf("Hi\n");
        // uint8_t gzybuf[128];
        // uart_receive(IOT_UART, gzybuf, 32);
        // usart_data_transmit(USART2, 0xfe);
        // osDelay(1000);
        // gpio_bit_set(IOT_UART_TX_PORT, IOT_UART_TX_PIN);
        // osDelay(1000);
        // gpio_bit_reset(IOT_UART_TX_PORT, IOT_UART_TX_PIN);
        osDelay(1000);
    }
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while (RESET == usart_flag_get(USART2, USART_FLAG_TBE))
        ;
    return ch;
}
