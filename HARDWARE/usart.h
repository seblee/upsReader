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

/* 串口设备结构体 */
typedef struct
{
    uint32_t uart;           /* STM32内部串口设备指针 */
    uint8_t *pTxBuf;         /* 发送缓冲区 */
    uint8_t *pRxBuf;         /* 接收缓冲区 */
    uint16_t usTxBufSize;    /* 发送缓冲区大小 */
    uint16_t usRxBufSize;    /* 接收缓冲区大小 */
    __IO uint16_t usTxWrite; /* 发送缓冲区写指针 */
    __IO uint16_t usTxRead;  /* 发送缓冲区读指针 */
    __IO uint16_t usTxCount; /* 等待发送的数据个数 */

    __IO uint16_t usRxWrite; /* 接收缓冲区写指针 */
    __IO uint16_t usRxRead;  /* 接收缓冲区读指针 */
    __IO uint16_t usRxCount; /* 还未读取的新数据个数 */

    void (*SendBefor)(void); /* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
    void (*SendOver)(void); /* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
    void (*ReciveNew)(uint8_t _byte); /* 串口收到数据的回调函数指针 */
    void (*ReceiveOver)(void);        /* 接收完毕的回调函数指针（空闲中断） */
} UART_T;
/* 定义端口号 */
typedef enum
{
    COM0 = 0, /* USART0  PA9, PA10 */
    COM1 = 1, /* USART1, PA2, PA3 */
} COM_PORT_E;

/* Private define ------------------------------------------------------------*/
#define UART0_FIFO_EN 1
#define UART1_FIFO_EN 0
#define UART2_FIFO_EN 0
#define UART3_FIFO_EN 0
#define UART4_FIFO_EN 0

#define USART_BUF_SIZE 120

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART0_FIFO_EN == 1
#define UART0_BAUD 2400
#define UART0_TX_BUF_SIZE 1 * 1024
#define UART0_RX_BUF_SIZE 1 * 1024
#endif

#if UART1_FIFO_EN == 1
#define UART1_BAUD 115200
#define UART1_TX_BUF_SIZE 1 * 1024
#define UART1_RX_BUF_SIZE 1 * 1024
#endif

/* Private macro -------------------------------------------------------------*/
//#define RS485_ISOLATE
#ifdef RS485_ISOLATE
#define U0_TX_EN() gpio_bit_reset(GPIOB, GPIO_PIN_1)
#define U0_RX_EN() gpio_bit_set(GPIOB, GPIO_PIN_1)

#define U1_TX_EN() gpio_bit_reset(GPIOA, GPIO_PIN_1)
#define U1_RX_EN() gpio_bit_set(GPIOA, GPIO_PIN_1)

#else
#define U0_TX_EN() gpio_bit_set(GPIOB, GPIO_PIN_1)
#define U0_RX_EN() gpio_bit_reset(GPIOB, GPIO_PIN_1)

#define U1_TX_EN() gpio_bit_set(GPIOA, GPIO_PIN_1)
#define U1_RX_EN() gpio_bit_reset(GPIOA, GPIO_PIN_1)
#endif

/* 开关全局中断的宏 */
#define ENABLE_INT() __set_PRIMASK(0)  /* 使能全局中断 */
#define DISABLE_INT() __set_PRIMASK(1) /* 禁止全局中断 */
/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern uint8_t rx0buffer[USART_BUF_SIZE];
extern uint16_t rx0Count;
extern uint8_t tx0buffer[USART_BUF_SIZE];
extern uint16_t tx0Count;

extern uint8_t rx1buffer[USART_BUF_SIZE];
extern uint16_t rx1Count;
extern uint8_t tx1buffer[USART_BUF_SIZE];
extern uint16_t tx1Count;
/* Private function prototypes -----------------------------------------------*/
void uart0_init(uint32_t baudrate);
void uart1_init(uint32_t baudrate);
void uart1_dma_send(uint8_t *s_addr, uint16_t length);
int fputc(int ch, FILE *f);
void bspInitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comReceiveBuff(COM_PORT_E _ucPort, uint8_t *_pByte, uint8_t len);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

/* Private user code ---------------------------------------------------------*/

#endif /* __UART_H */
