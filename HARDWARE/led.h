#ifndef __LED_H
#define __LED_H

#include "gd32f1x0.h"

#define LED1_PIN GPIO_PIN_1
#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_CLK RCU_GPIOF

void led_init(void);
void led_open(void);
void led_close(void);
void led_toggle(void);
#endif /* __LED_H */
