/******************** (C) COPYRIGHT 2011 青风电子 ********************
 * 文件名  ：led.c
 * 描述    ：
 * 实验平台：青风stm32f051开发板
 * 描述    ：led驱动函数
 * 作者    ：青风
 * 店铺    ：qfv5.taobao.com
 **********************************************************************************/

#include "led.h"
/*********************************************************
 * @name   led_init
 * @brief  led gpio and port bank clock initilization
 * @calls  gpio dirvers
 * @called BackGround_proc()
 * @param  None
 * @retval None
 *********************************************************/
void led_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(LED1_GPIO_CLK);
    /* configure led GPIO port */
    gpio_mode_set(LED1_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_PIN);
    gpio_output_options_set(LED1_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_PIN);
    GPIO_BC(LED1_GPIO_PORT) = LED1_PIN;

    gpio_bit_set(LED1_GPIO_PORT, LED1_PIN);
}

/*********************************************************
 * @name   led_open
 * @brief  led highlight
 * @calls  GPIO_ResetBits()
 * @called None
 * @param  None
 * @retval None
 *********************************************************/
void led_open(void)
{
    GPIO_BC(LED1_GPIO_PORT) = LED1_PIN;
    // gpio_bit_reset(LED1_GPIO_PORT, LED1_PIN);
}
/*********************************************************
 * @name   led_close
 * @brief  led shut
 * @calls  GPIO_SetBits()
 * @called None
 * @param  None
 * @retval None
 *********************************************************/
void led_close(void)
{
    GPIO_BOP(LED1_GPIO_PORT) = LED1_PIN;
    // gpio_bit_set(LED1_GPIO_PORT, LED1_PIN);
}

/*********************************************************
 * @name   led_toggle
 * @brief  led toggles each time called
 * @calls  GPIO_WriteBit()
 * @called None
 * @param  None
 * @retval None
 *********************************************************/
void led_toggle(void)
{
    // gpio_bit_write(LED1_GPIO_PORT, LED1_PIN, (RESET == gpio_output_bit_get(LED1_GPIO_PORT, LED1_PIN)) ? SET : RESET);
    GPIO_OCTL(LED1_GPIO_PORT) ^= LED1_PIN;
}
