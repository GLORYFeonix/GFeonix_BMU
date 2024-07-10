#include "uart.h"

uint8_t txBuf0[128];
uint8_t rxBuf0[128];
static uart_t pcsUart;
uint8_t txBuf1[128];
uint8_t rxBuf1[128];
uint8_t upperData[192];
static uart_t upperUart;
uint8_t txBuf2[128];
uint8_t rxBuf2[128];
static uart_t iotUart;
uint8_t txBuf3[128];
uint8_t rxBuf3[256];
static uart_t bmsUart;

void uart_Init(void)
{
    UART0_Init();
    UART1_Init();
    UART2_Init();
    UART3_Init();
}

static void UART0_Init(void)
{
    nvic_irq_enable(USART0_IRQn, 0, 0);
    nvic_irq_enable(DMA0_Channel4_IRQn, 0, 1);
    nvic_irq_enable(DMA0_Channel3_IRQn, 0, 0);

    rcu_periph_clock_enable(RCU_USART0);

    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_flag_clear(USART0, USART_FLAG_IDLE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
    usart_flag_clear(USART0, USART_FLAG_TC);
    usart_interrupt_enable(USART0, USART_INT_TC);

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0, DMA_CH4);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxBuf0;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(rxBuf0);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH4, &dma_init_struct);

    dma_deinit(DMA0, DMA_CH3);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txBuf0;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(txBuf0);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH3, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH4);
    dma_memory_to_memory_disable(DMA0, DMA_CH4);
    dma_circulation_disable(DMA0, DMA_CH3);
    dma_memory_to_memory_disable(DMA0, DMA_CH3);

    usart_dma_receive_config(USART0, USART_RECEIVE_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
    dma_channel_enable(DMA0, DMA_CH4);

    usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH3, DMA_INT_FTF);
    dma_channel_disable(DMA0, DMA_CH3);

    usart_enable(USART0);
}

static void UART1_Init(void)
{
    nvic_irq_enable(USART1_IRQn, 0, 0);
    nvic_irq_enable(DMA0_Channel5_IRQn, 0, 0);
    nvic_irq_enable(DMA0_Channel6_IRQn, 0, 1);

    rcu_periph_clock_enable(RCU_USART1);

    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_flag_clear(USART1, USART_FLAG_IDLE);
    usart_interrupt_enable(USART1, USART_INT_IDLE);
    usart_flag_clear(USART1, USART_FLAG_TC);
    usart_interrupt_enable(USART1, USART_INT_TC);

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0, DMA_CH5);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxBuf1;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(rxBuf1);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH5, &dma_init_struct);

    dma_deinit(DMA0, DMA_CH6);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txBuf1;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(txBuf1);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH6, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH5);
    dma_memory_to_memory_disable(DMA0, DMA_CH5);
    dma_circulation_disable(DMA0, DMA_CH6);
    dma_memory_to_memory_disable(DMA0, DMA_CH6);

    usart_dma_receive_config(USART1, USART_RECEIVE_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH5, DMA_INT_FTF);
    dma_channel_enable(DMA0, DMA_CH5);

    usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
    dma_channel_disable(DMA0, DMA_CH6);

    usart_enable(USART1);
}

static void UART2_Init(void)
{
    nvic_irq_enable(USART2_IRQn, 0, 0);
    nvic_irq_enable(DMA0_Channel2_IRQn, 0, 0);
    nvic_irq_enable(DMA0_Channel1_IRQn, 0, 1);

    rcu_periph_clock_enable(RCU_USART2);

    usart_deinit(USART2);
    usart_baudrate_set(USART2, 115200U);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_flag_clear(USART2, USART_FLAG_IDLE);
    usart_interrupt_enable(USART2, USART_INT_IDLE);
    usart_flag_clear(USART2, USART_FLAG_TC);
    usart_interrupt_enable(USART2, USART_INT_TC);

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA0, DMA_CH2);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxBuf2;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(rxBuf2);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);

    dma_deinit(DMA0, DMA_CH1);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txBuf2;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(txBuf2);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART2);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH1, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);
    dma_circulation_disable(DMA0, DMA_CH1);
    dma_memory_to_memory_disable(DMA0, DMA_CH1);

    usart_dma_receive_config(USART2, USART_RECEIVE_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
    dma_channel_enable(DMA0, DMA_CH2);

    usart_dma_transmit_config(USART2, USART_TRANSMIT_DMA_ENABLE);
    dma_interrupt_enable(DMA0, DMA_CH1, DMA_INT_FTF);
    dma_channel_disable(DMA0, DMA_CH1);

    usart_enable(USART2);
}

static void UART3_Init(void)
{
    nvic_irq_enable(UART3_IRQn, 0, 0);
    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 1);
    nvic_irq_enable(DMA1_Channel3_Channel4_IRQn, 0, 0);

    rcu_periph_clock_enable(RCU_UART3);

    usart_deinit(UART3);
    usart_baudrate_set(UART3, 9600U);
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
    usart_flag_clear(UART3, USART_FLAG_IDLE);
    usart_interrupt_enable(UART3, USART_INT_IDLE);
    usart_flag_clear(UART3, USART_FLAG_TC);
    usart_interrupt_enable(UART3, USART_INT_TC);

    dma_parameter_struct dma_init_struct;
    dma_deinit(DMA1, DMA_CH2);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxBuf3;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(rxBuf3);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH2, &dma_init_struct);

    dma_deinit(DMA1, DMA_CH4);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)txBuf3;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = sizeof(txBuf3);
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(UART3);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA1, DMA_CH4, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH2);
    dma_memory_to_memory_disable(DMA1, DMA_CH2);
    dma_circulation_disable(DMA1, DMA_CH4);
    dma_memory_to_memory_disable(DMA1, DMA_CH4);

    usart_dma_receive_config(UART3, USART_RECEIVE_DMA_ENABLE);
    dma_interrupt_enable(DMA1, DMA_CH2, DMA_INT_FTF);
    dma_channel_disable(DMA1, DMA_CH2);

    usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);
    dma_interrupt_enable(DMA1, DMA_CH4, DMA_INT_FTF);
    dma_channel_disable(DMA1, DMA_CH4);

    usart_enable(UART3);
}

static void uart_dma_config(uart_dma_chn_e chn, uint32_t addr, uint32_t num)
{
    dma_parameter_struct dma_init_struct;

    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;

    switch (chn)
    {
    case PCS_UART_RX:
        dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
        dma_init_struct.memory_addr = addr;
        dma_init_struct.number = num;
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
        dma_deinit(DMA0, DMA_CH4);
        dma_init(DMA0, DMA_CH4, &dma_init_struct);

        dma_circulation_disable(DMA0, DMA_CH4);
        dma_memory_to_memory_disable(DMA0, DMA_CH4);
        dma_interrupt_enable(DMA0, DMA_CH4, DMA_INT_FTF);
        dma_channel_enable(DMA0, DMA_CH4);
        break;

    case PCS_UART_TX:
        dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
        dma_init_struct.memory_addr = (uint32_t)txBuf0;
        dma_init_struct.number = num;
        dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
        dma_deinit(DMA0, DMA_CH3);
        dma_init(DMA0, DMA_CH3, &dma_init_struct);

        dma_circulation_disable(DMA0, DMA_CH3);
        dma_memory_to_memory_disable(DMA0, DMA_CH3);
        dma_interrupt_enable(DMA0, DMA_CH3, DMA_INT_FTF);
        dma_channel_enable(DMA0, DMA_CH3);
        break;

    default:
        break;
    }
}

void USART0_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))
    {
        usart_data_receive(USART0);

        dma_channel_disable(DMA0, DMA_CH4);

        uint32_t rxNum = dma_transfer_number_get(DMA0, DMA_CH4);
        pcsUart.len = sizeof(rxBuf0) - rxNum;
        memcpy(pcsUart.buf + pcsUart.index, rxBuf0, pcsUart.len);
        pcsUart.index += pcsUart.len;
        dma_transfer_number_config(DMA0, DMA_CH4, sizeof(rxBuf0));
        dma_channel_enable(DMA0, DMA_CH4);

        extern osEventFlagsId_t pcsRxEvent;
        osEventFlagsSet(pcsRxEvent, 0x00000001U);
    }

    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TC))
    {
        usart_flag_clear(USART0, USART_FLAG_TC);

        pcsUart.index = 0;
        dma_channel_disable(DMA0, DMA_CH3);
        PCS_485_RX_ENABLE();
    }
}

void DMA0_Channel4_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH4, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH4, DMA_INT_FLAG_G);
}

void DMA0_Channel3_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_G);
}

void USART1_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE))
    {
        usart_data_receive(USART1);

        dma_channel_disable(DMA0, DMA_CH5);

        uint32_t rxNum = dma_transfer_number_get(DMA0, DMA_CH5);
        upperUart.len = sizeof(rxBuf1) - rxNum;
        memcpy(upperUart.buf + upperUart.index, rxBuf1, upperUart.len);
        upperUart.index += upperUart.len;
        dma_transfer_number_config(DMA0, DMA_CH5, sizeof(rxBuf1));
        dma_channel_enable(DMA0, DMA_CH5);

        extern osEventFlagsId_t upperRxEvent;
        osEventFlagsSet(upperRxEvent, 0x00000001U);
    }

    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TC))
    {
        usart_flag_clear(USART1, USART_FLAG_TC);

        upperUart.index = 0;
        dma_channel_disable(DMA0, DMA_CH6);
    }
}

void DMA0_Channel5_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH5, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH5, DMA_INT_FLAG_G);
}

void DMA0_Channel6_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_G);
}

void USART2_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE))
    {
        usart_data_receive(USART2);

        dma_channel_disable(DMA0, DMA_CH2);

        uint32_t rxNum = dma_transfer_number_get(DMA0, DMA_CH2);
        iotUart.len = sizeof(rxBuf2) - rxNum;
        memcpy(iotUart.buf + iotUart.index, rxBuf1, iotUart.len);
        iotUart.index += iotUart.len;
        dma_transfer_number_config(DMA0, DMA_CH2, sizeof(rxBuf2));
        dma_channel_enable(DMA0, DMA_CH2);

        extern osEventFlagsId_t iotRxEvent;
        osEventFlagsSet(iotRxEvent, 0x00000001U);
    }

    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_TC))
    {
        usart_flag_clear(USART2, USART_FLAG_TC);

        iotUart.index = 0;
        dma_channel_disable(DMA0, DMA_CH1);
    }
}

void DMA0_Channel2_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH2, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH2, DMA_INT_FLAG_G);
}

void DMA0_Channel1_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA0, DMA_CH1, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA0, DMA_CH1, DMA_INT_FLAG_G);
}

void UART3_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_IDLE))
    {
        usart_data_receive(UART3);

        dma_channel_disable(DMA1, DMA_CH2);

        uint32_t rxNum = dma_transfer_number_get(DMA1, DMA_CH2);
        bmsUart.len = sizeof(rxBuf3) - rxNum;
        memcpy(bmsUart.buf + bmsUart.index, rxBuf3, bmsUart.len);
        bmsUart.index += bmsUart.len;
        dma_transfer_number_config(DMA1, DMA_CH2, sizeof(rxBuf3));
        dma_channel_enable(DMA1, DMA_CH2);

        extern osEventFlagsId_t bmsRxEvent;
        osEventFlagsSet(bmsRxEvent, 0x00000001U);
    }

    if (RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_TC))
    {
        usart_flag_clear(UART3, USART_FLAG_TC);

        bmsUart.index = 0;
        dma_channel_disable(DMA1, DMA_CH4);
        BMS_485_RX_ENABLE();
    }
}

void DMA1_Channel2_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INT_FLAG_G);
}

void DMA1_Channel3_4_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA1, DMA_CH4, DMA_INT_FLAG_FTF))
    {
    }
    dma_interrupt_flag_clear(DMA1, DMA_CH4, DMA_INT_FLAG_G);
}

void uart_receive(uart_e uart, uint8_t *data, uint16_t num)
{
    uint16_t i = 0;

    switch (uart)
    {
    case PCS_UART:
        for (i = 0; i < sizeof(pcsUart.buf) - (7 + num + 2); i++)
        {
            if (pcsUart.buf[i] == 0x5A && pcsUart.buf[i + 1] == 0xA5)
            {
                uint16_t crc = (pcsUart.buf[i + 7 + num + 1] << 8) | (pcsUart.buf[i + 7 + num] << 0);
                if (crc == crc16_modbus(&pcsUart.buf[i + 2], 5 + num))
                {
                    memcpy(data, &pcsUart.buf[i + 7], num);
                    break;
                }
            }
        }
        if (i == sizeof(pcsUart.buf) - (7 + num + 2))
        {
            memset(data, 0, num);
        }
        break;

    case UPPER_UART:
        for (i = 0; i < sizeof(upperUart.buf) - (4 + num + 2); i++)
        {
            if (upperUart.buf[i] == 0x5A && upperUart.buf[i + 1] == 0xA5)
            {
                uint16_t dataLen = ((uint16_t)upperUart.buf[i + 6] << 8) | ((uint16_t)upperUart.buf[i + 5] << 0);
                uint16_t crc = (upperUart.buf[i + 7 + dataLen + 1] << 8) | (upperUart.buf[i + 7 + dataLen] << 0);
                if (crc == crc16_modbus(&upperUart.buf[i + 2], 5 + dataLen))
                {
                    memcpy(data, &upperUart.buf[i + 4], 3); // only service id and data length
                    memcpy(upperData, &upperUart.buf[i + 7], dataLen);
                    upperUart.index = 0;
                    break;
                }
            }
        }
        if (i == sizeof(upperUart.buf) - (4 + num + 2))
        {
            memset(data, 0, num);
        }
        break;

    case IOT_UART:
        for (i = 0; i < sizeof(iotUart.buf) - (4 + num + 2); i++)
        {
            if (iotUart.buf[i] == 0x5A && iotUart.buf[i + 1] == 0xA5)
            {
                uint16_t dataLen = ((uint16_t)iotUart.buf[i + 6] << 8) | ((uint16_t)iotUart.buf[i + 5] << 0);
                uint16_t crc = (iotUart.buf[i + 7 + dataLen + 1] << 8) | (iotUart.buf[i + 7 + dataLen] << 0);
                if (crc == crc16_modbus(&iotUart.buf[i + 2], 5 + dataLen))
                {
                    memcpy(data, &iotUart.buf[i + 4], 3); // only service id and data length
                    memcpy(upperData, &iotUart.buf[i + 7], dataLen);
                    iotUart.index = 0;
                    break;
                }
            }
        }
        if (i == sizeof(iotUart.buf) - (4 + num + 2))
        {
            memset(data, 0, num);
        }
        break;

    case BMS_UART:
        for (i = 0; i < sizeof(bmsUart.buf) - 150; i++)
        {
            if (bmsUart.buf[i] == 0xEA && bmsUart.buf[i + 1] == 0x00 && bmsUart.buf[i + 2] == 0x01 && bmsUart.buf[i + 3] == 0x01)
            {
                // uint16_t crc = (bmsUart.buf[i + 7 + num + 1] << 8) | (bmsUart.buf[i + 7 + num] << 0);
                // if (crc == crc16_modbus(&bmsUart.buf[i + 2], 5 + num))
                // {
                    memcpy(data, &bmsUart.buf[i], num);
                    break;
                // }
            }
        }
        if (i == sizeof(bmsUart.buf) - 150)
        {
            memset(data, 0, num);
        }
        break;

    default:
        break;
    }
}

void uart_send(uart_e uart, uint16_t num)
{
    switch (uart)
    {
    case PCS_UART:
        PCS_485_TX_ENABLE();
        dma_transfer_number_config(DMA0, DMA_CH3, num);
        dma_channel_enable(DMA0, DMA_CH3);
        break;

    case UPPER_UART:
        dma_transfer_number_config(DMA0, DMA_CH6, num);
        dma_channel_enable(DMA0, DMA_CH6);
        break;

    case IOT_UART:
        dma_transfer_number_config(DMA0, DMA_CH1, num);
        dma_channel_enable(DMA0, DMA_CH1);
        break;

    case BMS_UART:
        BMS_485_TX_ENABLE();
        dma_transfer_number_config(DMA1, DMA_CH4, num);
        dma_channel_enable(DMA1, DMA_CH4);
        break;

    default:
        break;
    }
}
