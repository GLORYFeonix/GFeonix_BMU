#ifndef UART_H
#define UART_H

#include <string.h>
#include "cmsis_os2.h"
#include "gd32f30x.h"
#include "gpio.h"
#include "crc.h"

typedef enum
{
    PCS_UART,
    UPPER_UART,
    IOT_UART,
    BMS_UART,
} uart_e;

typedef enum
{
    PCS_UART_RX,
    PCS_UART_TX,
    UPPER_UART_RX,
    UPPER_UART_TX,
    BMS_UART_RX,
    BMS_UART_TX,
} uart_dma_chn_e;

typedef enum
{
    TRANS_RX,
    TRANS_TX,
} uart_transmit_dir_e;

typedef struct
{
    uint16_t head;
    uint16_t tail;
    uint16_t size;
    uint8_t buf[128];
    uint16_t len;
    uint16_t index;
}uart_t;

extern uint8_t txBuf0[128];
extern uint8_t rxBuf0[128];
extern uint8_t txBuf1[128];
extern uint8_t rxBuf1[128];
extern uint8_t txBuf3[128];
extern uint8_t rxBuf3[256]; // BMS一帧151字节

void uart_Init(void);

static void UART0_Init(void);
static void UART1_Init(void);
static void UART2_Init(void);
static void UART3_Init(void);

void uart_receive(uart_e uart, uint8_t *data, uint16_t num);
void uart_send(uart_e uart, uint16_t num);

#endif /* UART_H */