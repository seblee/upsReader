#ifndef __UART_H
#define __UART_H

#include "gd32f1x0.h"
#include <stdio.h>

#define TE485 gpio_bit_set(GPIOA, GPIO_PIN_1)
#define RE485 gpio_bit_reset(GPIOA, GPIO_PIN_1)

void uart1_init(uint32_t baudrate);
void uart1_dma_send(uint8_t *s_addr, uint16_t length);
int fputc(int ch, FILE *f);

#endif /* __UART_H */
