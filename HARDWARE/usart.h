/**
 * @file usart.h
 * @author  xiaowine (xiaowine@sina.cn)
 * @brief
 * @version 01.00
 * @date    2021-06-29
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-06-29 <td>1.0     <td>wangh     <td>内容
 * </table>
 * ******************************************************************
 * *                   .::::
 * *                 .::::::::
 * *                ::::::::::
 * *             ..:::::::::::
 * *          '::::::::::::
 * *            .:::::::::
 * *       '::::::::::::::..        女神助攻,流量冲天
 * *            ..::::::::::::.     永不宕机,代码无bug
 * *          ``:::::::::::::::
 * *           ::::``:::::::::'        .:::
 * *          ::::'   ':::::'       .::::::::
 * *        .::::'      ::::     .:::::::'::::
 * *       .:::'       :::::  .:::::::::' ':::::
 * *      .::'        :::::.:::::::::'      ':::::
 * *     .::'         ::::::::::::::'         ``::::
 * * ...:::           ::::::::::::'              ``::
 * *```` ':.          ':::::::::'                  ::::.
 * *                   '.:::::'                    ':'````.
 * ******************************************************************
 */

#ifndef __UART_H
#define __UART_H
/* Private includes ----------------------------------------------------------*/
#include "gd32f1x0.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 128
/* Private macro -------------------------------------------------------------*/
#define TE485 gpio_bit_set(GPIOA, GPIO_PIN_1)
#define RE485 gpio_bit_reset(GPIOA, GPIO_PIN_1)

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern uint8_t rx1buffer[BUFFER_SIZE];
extern uint8_t rx1Count;

/* Private function prototypes -----------------------------------------------*/
void uart1_init(uint32_t baudrate);
void uart1_dma_send(uint8_t *s_addr, uint16_t length);
int fputc(int ch, FILE *f);

/* Private user code ---------------------------------------------------------*/

#endif /* __UART_H */
