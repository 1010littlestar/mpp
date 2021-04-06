/*
 * Copyright (C) Hisilicon Technologies Co., Ltd. 2017-2019. All rights reserved.
 * Description: Deep process unit of rectification.
 * Author: Guo
 * Create: 2017-11-10
 */

#include "uart.h"
#include "uart_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define UART_MULTIPLE_OF_BAUDRATE  16
#define UART_FRACTION_COEF         8
#define UART_BAUDRATE 115200
#define UART_CLOCK    24000000
#define UART_PORTS    \
    {                                                      \
        (void *)UART0_REG_BASE, (void *)UART1_REG_BASE,    \
            (void *)UART2_REG_BASE, (void *)UART3_REG_BASE \
    }

#ifndef FPGA
#define UART_INDEX 2
#else
#define UART_INDEX 1
#endif

typedef unsigned int size_t;

#define UART_WRITE(addr, val) (*(volatile unsigned int *)(addr) = (val))
#define UART_READ(addr)       (*(volatile unsigned int *)(addr))
#define UART_PORT             UART_INDEX

static volatile unsigned char *const g_aucPort[] = UART_PORTS;

extern int virt_tty_direct_write(const char *buf, size_t count);

static void serial_putc(int portnum, char c)
{
    while (UART_READ(g_aucPort[portnum] + UART_PL01x_FR) & UART_PL01x_FR_TXFF) {
        ;
    }
    UART_WRITE(g_aucPort[portnum] + UART_PL01x_DR, c);
}

int serial_init(void)
{
    unsigned int uiTemp;
    unsigned int uiDivider;
    unsigned int uiRemainder;
    unsigned int uiFraction;

    UART_WRITE(g_aucPort[UART_PORT] + UART_PL011_CR, 0x0);

    uiTemp = UART_MULTIPLE_OF_BAUDRATE * UART_BAUDRATE;
    uiDivider = UART_CLOCK / uiTemp;
    uiRemainder = UART_CLOCK % uiTemp;
    uiTemp = (UART_FRACTION_COEF * uiRemainder) / UART_BAUDRATE;
    uiFraction = (uiTemp >> 1) + (uiTemp & 1);

    UART_WRITE(g_aucPort[UART_PORT] + UART_PL011_IBRD, uiDivider);
    UART_WRITE(g_aucPort[UART_PORT] + UART_PL011_FBRD, uiFraction);

    UART_WRITE(g_aucPort[UART_PORT] + UART_PL011_LCRH,
               (UART_PL011_LCRH_WLEN_8 | UART_PL011_LCRH_FEN));

    UART_WRITE(g_aucPort[UART_PORT] + UART_PL011_CR,
               (UART_PL011_CR_UARTEN | UART_PL011_CR_TXE |
                UART_PL011_CR_RXE));

    return 0;
}

void outbyte(int c)
{
    if (c == '\n') {
        serial_putc(UART_PORT, '\r');                       /* physical uart */
        (void)virt_tty_direct_write((const char *)"\r", 1); /* virtual uart */
    }

    serial_putc(UART_PORT, c);                        /* physical uart */
    (void)virt_tty_direct_write((const char *)&c, 1); /* virtual uart */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


