/**
 * @file usart.c
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

/* Private includes ----------------------------------------------------------*/
#include "usart.h"
#include "gd32f1x0.h"
#include <stdarg.h>
#include <stdio.h>
#define osObjectsPublic  // define objects in main module
#include "osObjects.h"   // RTOS object definitions
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BPS 9600

#define USART0_TDATA_ADDRESS ((uint32_t)0x40013828) /* 130_150 device */
#define USART0_RDATA_ADDRESS ((uint32_t)0x40013824) /* 130_150 device */

/* Private macro -------------------------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
uint8_t rx0buffer[USART_BUF_SIZE] = {0};
uint16_t rx0Count                 = 0;
uint8_t tx0buffer[USART_BUF_SIZE] = {0};
uint16_t tx0Count                 = 0;

uint8_t rx1buffer[USART_BUF_SIZE] = {0};
uint16_t rx1Count                 = 0;
uint8_t tx1buffer[USART_BUF_SIZE] = {0};
uint16_t tx1Count                 = 0;
#if UART0_FIFO_EN
static UART_T g_tUart0;
#endif
#if UART1_FIFO_EN
static UART_T g_tUart1;
#endif
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void UartVarInit(void);
static void InitHardUart(void);
static void ConfigUartNVIC(void);

#if UART0_FIFO_EN
static void uart0_gpio_init(void);
void u0SendBefor(void);
void u0SendOver(void);
#endif

#if UART1_FIFO_EN
static void uart1_gpio_init(void);
void u1SendBefor(void);
void u1SendOver(void);
#endif
// void u0ReciveNew(uint8_t ch);

static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
/* Private user code ---------------------------------------------------------*/

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bspInitUart(void)
{
    UartVarInit(); /* 必须先初始化全局变量,再配置硬件 */

    InitHardUart(); /* 配置串口的硬件参数(波特率等) */

    ConfigUartNVIC(); /* 配置串口中断 */
#if UART0_FIFO_EN
    u0SendOver();
#endif
#if UART1_FIFO_EN
    u1SendOver();
#endif
}

/*
*********************************************************************************************************
*	函 数 名: ComToUart
*	功能说明: 将COM端口号转换为UART指针
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: uart指针
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
    if (_ucPort == COM0)
    {
#if UART0_FIFO_EN == 1
        return &g_tUart0;
#else
        return 0;
#endif
    }
    else if (_ucPort == COM1)
    {
#if UART1_FIFO_EN == 1
        return &g_tUart1;
#else
        return 0;
#endif
    }
    else
    {
        /* 不做任何处理 */
        return 0;
    }
}
/*
 *********************************************************************************************************
 *	函 数 名: comSendBuf
 *	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
 *	形    参: _ucPort: 端口号(COM1 - COM6)
 *			  _ucaBuf: 待发送的数据缓冲区
 *			  _usLen : 数据长度
 *	返 回 值: 无
 *********************************************************************************************************
 */
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    if (pUart->SendBefor != 0)
    {
        pUart->SendBefor(); /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }

    UartSend(pUart, _ucaBuf, _usLen);
}
/*
*********************************************************************************************************
*	函 数 名: comSendChar
*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _ucByte: 待发送的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
    comSendBuf(_ucPort, &_ucByte, 1);
}
/*
*********************************************************************************************************
*	函 数 名: comReceiveBuff
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 表示读取到有效字节的长度
*********************************************************************************************************
*/
uint8_t comReceiveBuff(COM_PORT_E _ucPort, uint8_t *_pByte, uint8_t len)
{
    UART_T *pUart;
    uint8_t count = 0;
    uint8_t *p    = _pByte;
    pUart         = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return 0;
    }

    do
    {
        if (UartGetChar(pUart, p++))
            count++;
        else
            break;
    } while (count < len);

    return count;
}
/*
*********************************************************************************************************
*	函 数 名: comGetChar
*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
*	形    参: _ucPort: 端口号(COM1 - COM6)
*			  _pByte: 接收到的数据存放在这个地址
*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return 0;
    }

    return UartGetChar(pUart, _pByte);
}
/*
*********************************************************************************************************
*	函 数 名: comClearTxFifo
*	功能说明: 清零串口发送缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->usTxWrite = 0;
    pUart->usTxRead  = 0;
    pUart->usTxCount = 0;
}
/*
*********************************************************************************************************
*	函 数 名: comClearRxFifo
*	功能说明: 清零串口接收缓冲区
*	形    参: _ucPort: 端口号(COM1 - COM6)
*	返 回 值: 无
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
    UART_T *pUart;

    pUart = ComToUart(_ucPort);
    if (pUart == 0)
    {
        return;
    }

    pUart->usRxWrite = 0;
    pUart->usRxRead  = 0;
    pUart->usRxCount = 0;
}

#if UART0_FIFO_EN == 1
void u0SendBefor(void)
{
    U0_TX_EN();
}
void u0SendOver(void)
{
    U0_RX_EN();
}
void u0ReceiveNew(uint8_t ch)
{
    usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
}

void u0ReceiveOver(void)
{
    uint8_t cache[USART_BUF_SIZE] = {0};
    rx0Count                      = comReceiveBuff(COM0, cache, USART_BUF_SIZE);
    spiDataFifoPush(cache, rx0Count, 0);
    rx0Count = 0;
#ifdef RS485_ISOLATE
    comClearRxFifo(COM0);
#endif
}
#endif
#if UART1_FIFO_EN == 1
void u1SendBefor(void)
{
    U1_TX_EN();
}
void u1SendOver(void)
{
    U1_RX_EN();
}
#endif

/*
*********************************************************************************************************
*	函 数 名: UartVarInit
*	功能说明: 初始化串口相关的变量
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartVarInit(void)
{
#if UART0_FIFO_EN == 1
    g_tUart0.uart        = USART0;         /* 串口外设设备 */
    g_tUart0.pTxBuf      = tx0buffer;      /* 发送缓冲区指针 */
    g_tUart0.pRxBuf      = rx0buffer;      /* 接收缓冲区指针 */
    g_tUart0.usTxBufSize = USART_BUF_SIZE; /* 发送缓冲区大小 */
    g_tUart0.usRxBufSize = USART_BUF_SIZE; /* 接收缓冲区大小 */
    g_tUart0.usTxWrite   = 0;              /* 发送FIFO写索引 */
    g_tUart0.usTxRead    = 0;              /* 发送FIFO读索引 */
    g_tUart0.usRxWrite   = 0;              /* 接收FIFO写索引 */
    g_tUart0.usRxRead    = 0;              /* 接收FIFO读索引 */
    g_tUart0.usRxCount   = 0;              /* 接收到的新数据个数 */
    g_tUart0.usTxCount   = 0;              /* 待发送的数据个数 */
    g_tUart0.SendBefor   = u0SendBefor;    /* 发送数据前的回调函数 */
    g_tUart0.SendOver    = u0SendOver;     /* 发送完毕后的回调函数 */
    g_tUart0.ReciveNew   = u0ReceiveNew;   /* 接收到新数据后的回调函数 */
    g_tUart0.ReceiveOver = u0ReceiveOver;  /* 接收完毕的回调函数指针（空闲中断） */
#endif

#if UART1_FIFO_EN == 1
    g_tUart1.uart        = USART1;         /* 串口外设设备 */
    g_tUart1.pTxBuf      = tx1buffer;      /* 发送缓冲区指针 */
    g_tUart1.pRxBuf      = rx1buffer;      /* 接收缓冲区指针 */
    g_tUart1.usTxBufSize = USART_BUF_SIZE; /* 发送缓冲区大小 */
    g_tUart1.usRxBufSize = USART_BUF_SIZE; /* 接收缓冲区大小 */
    g_tUart1.usTxWrite   = 0;              /* 发送FIFO写索引 */
    g_tUart1.usTxRead    = 0;              /* 发送FIFO读索引 */
    g_tUart1.usRxWrite   = 0;              /* 接收FIFO写索引 */
    g_tUart1.usRxRead    = 0;              /* 接收FIFO读索引 */
    g_tUart1.usRxCount   = 0;              /* 接收到的新数据个数 */
    g_tUart1.usTxCount   = 0;              /* 待发送的数据个数 */
    g_tUart1.SendBefor   = u1SendBefor;    /* 发送数据前的回调函数 */
    g_tUart1.SendOver    = u1SendOver;     /* 发送完毕后的回调函数 */
    g_tUart1.ReciveNew   = 0;              /* 接收到新数据后的回调函数 */
#endif
}

/*
*********************************************************************************************************
*	函 数 名: InitHardUart
*	功能说明: 配置串口的硬件参数（波特率，数据位，停止位，起始位，校验位，中断使能）适合于STM32-F4开发板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitHardUart(void)
{
#if UART0_FIFO_EN == 1 /* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/
    uart0_gpio_init();
    rcu_periph_clock_enable(RCU_USART0);
    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, UART0_BAUD);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_address_config(USART0, '\r');
    usart_address_detection_mode_config(USART0, USART_ADDM_FULLBIT);

    usart_enable(USART0);

    usart_flag_clear(USART0, USART_FLAG_TC);
#endif
#if UART1_FIFO_EN == 1 /* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/
    uart1_gpio_init();
    rcu_periph_clock_enable(RCU_USART1);
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, baudrate);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_address_config(USART1, '\r');
    usart_address_detection_mode_config(USART1, USART_ADDM_FULLBIT);

    /* enable USART TBE interrupt */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    usart_interrupt_enable(USART1, USART_INT_AM);

    usart_enable(USART1);
    usart_flag_clear(USART1, USART_FLAG_TC);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: ConfigUartNVIC
*	功能说明: 配置串口硬件中断.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ConfigUartNVIC(void)
{
#if UART0_FIFO_EN == 1
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    /* enable USART TBE interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    // usart_interrupt_enable(USART0, USART_INT_AM);
#endif

#if UART1_FIFO_EN == 1
    /* 使能串口1中断 */
    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* enable USART TBE interrupt */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
    usart_interrupt_enable(USART1, USART_INT_AM);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: UartSend
*	功能说明: 填写数据到UART发送缓冲区,并启动发送中断。中断处理函数发送完毕后，自动关闭发送中断
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
    uint16_t i;

    for (i = 0; i < _usLen; i++)
    {
        /* 如果发送缓冲区已经满了，则等待缓冲区空 */
#if 0
		/*
			在调试GPRS例程时，下面的代码出现死机，while 死循环
			原因： 发送第1个字节时 _pUart->usTxWrite = 1；_pUart->usTxRead = 0;
			将导致while(1) 无法退出
		*/
		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			DISABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
#else
        /* 当 _pUart->usTxBufSize == 1 时, 下面的函数会死掉(待完善) */
        while (1)
        {
            __IO uint16_t usCount;

            DISABLE_INT();
            usCount = _pUart->usTxCount;
            ENABLE_INT();

            if (usCount < _pUart->usTxBufSize)
            {
                break;
            }
        }
#endif

        /* 将新数据填入发送缓冲区 */
        _pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

        DISABLE_INT();
        if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
        {
            _pUart->usTxWrite = 0;
        }
        _pUart->usTxCount++;
        ENABLE_INT();
    }
    usart_interrupt_enable(_pUart->uart, USART_INT_TBE);
}

/*
*********************************************************************************************************
*	函 数 名: UartGetChar
*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
*	形    参: _pUart : 串口设备
*			  _pByte : 存放读取数据的指针
*	返 回 值: 0 表示无数据  1表示读取到数据
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
    uint16_t usCount;

    /* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
    DISABLE_INT();
    usCount = _pUart->usRxCount;
    ENABLE_INT();

    /* 如果读和写索引相同，则返回0 */
    // if (_pUart->usRxRead == usRxWrite)
    if (usCount == 0) /* 已经没有数据 */
    {
        return 0;
    }
    else
    {
        *_pByte = _pUart->pRxBuf[_pUart->usRxRead]; /* 从串口接收FIFO取1个数据 */

        /* 改写FIFO读索引 */
        DISABLE_INT();
        if (++_pUart->usRxRead >= _pUart->usRxBufSize)
        {
            _pUart->usRxRead = 0;
        }
        _pUart->usRxCount--;
        ENABLE_INT();
        return 1;
    }
}

/*
*********************************************************************************************************
*	函 数 名: UartIRQ
*	功能说明: 供中断服务程序调用，通用串口中断处理函数
*	形    参: _pUart : 串口设备
*	返 回 值: 无
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
    /* 处理接收中断  */
    if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_RBNE) != RESET)
    {
        /* 从串口接收数据寄存器读取数据存放到接收FIFO */
        uint8_t ch;

        ch = usart_data_receive(_pUart->uart);

        _pUart->pRxBuf[_pUart->usRxWrite] = ch;
        if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
        {
            _pUart->usRxWrite = 0;
        }
        if (_pUart->usRxCount < _pUart->usRxBufSize)
        {
            _pUart->usRxCount++;
        }

        /* 回调函数,通知应用程序收到新数据,一般是发送1个消息或者设置一个标记 */
        // if (_pUart->usRxWrite == _pUart->usRxRead)
        // if (_pUart->usRxCount == 1)
        {
            if (_pUart->ReciveNew)
            {
                _pUart->ReciveNew(ch);
            }
        }
        usart_interrupt_flag_clear(_pUart->uart, USART_INT_FLAG_RBNE);
    }
    else if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_ERR_ORERR | USART_INT_FLAG_ERR_NERR |
                                                        USART_INT_FLAG_ERR_FERR | USART_INT_FLAG_PERR))
    {
        usart_data_receive(_pUart->uart);
    }

    if (RESET != usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_IDLE))
    {
        if (_pUart->ReciveNew)
        {
            _pUart->ReceiveOver();
        }
        usart_interrupt_flag_clear(_pUart->uart, USART_INT_FLAG_IDLE);
        usart_interrupt_enable(_pUart->uart, USART_INT_RBNE);
        usart_interrupt_disable(_pUart->uart, USART_INT_IDLE);
    }

    /* 处理发送缓冲区空中断 */
    if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TBE) != RESET)
    {
        // if (_pUart->usTxRead == _pUart->usTxWrite)
        if (_pUart->usTxCount == 0)
        {
            /* 发送缓冲区的数据已取完时， 禁止发送缓冲区空中断 （注意：此时最后1个数据还未真正发送完毕）*/
            usart_interrupt_disable(_pUart->uart, USART_INT_TBE);
            /* 使能数据发送完毕中断 */
            usart_interrupt_enable(_pUart->uart, USART_INT_TC);
        }
        else
        {
            /* 从发送FIFO取1个字节写入串口发送数据寄存器 */
            usart_data_transmit(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
            if (++_pUart->usTxRead >= _pUart->usTxBufSize)
            {
                _pUart->usTxRead = 0;
            }
            _pUart->usTxCount--;
        }
    }
    /* 数据bit位全部发送完毕的中断 */
    else if (usart_interrupt_flag_get(_pUart->uart, USART_INT_FLAG_TC) != RESET)
    {
        // if (_pUart->usTxRead == _pUart->usTxWrite)
        if (_pUart->usTxCount == 0)
        {
            /* 如果发送FIFO的数据全部发送完毕，禁止数据发送完毕中断 */
            usart_interrupt_disable(_pUart->uart, USART_INT_TC);

            /* 回调函数, 一般用来处理RS485通信，将RS485芯片设置为接收模式，避免抢占总线 */
            if (_pUart->SendOver)
            {
                _pUart->SendOver();
            }
        }
        else
        {
            /* 正常情况下，不会进入此分支 */

            /* 如果发送FIFO的数据还未完毕，则从发送FIFO取1个数据写入发送数据寄存器 */
            usart_data_transmit(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
            if (++_pUart->usTxRead >= _pUart->usTxBufSize)
            {
                _pUart->usTxRead = 0;
            }
            _pUart->usTxCount--;
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	功能说明: USART中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if UART0_FIFO_EN == 1
void USART0_IRQHandler(void)
{
    UartIRQ(&g_tUart0);
}
#endif

#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{
    UartIRQ(&g_tUart1);
}
#endif

/*********************************************************
 * @name   uart0_gpio_init
 * @brief  USART0 GPIO initialization. Both uart port pins and direction control pins are configured
 * @calls  device drivers
 * @called uart0_init()
 * @param  None
 * @retval None
 *********************************************************/
static void uart0_gpio_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);

    rcu_periph_clock_enable(RCU_GPIOB);
    // RS485 direction control
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

/*********************************************************
 * @name   uart0_nvic_init
 * @brief  USART0 related IRQ vector initialization, include DMA and USART IRQs
 * @calls  device drivers
 * @called uart0_init()
 * @param  None
 * @retval None
 *********************************************************/
static void uart0_nvic_init(void)
{
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    /* enable USART TBE interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(USART0, USART_INT_AM);
}

/*********************************************************
 * @name   uart0_init
 * @brief  General USART0 initialization, after which USART0 is ready to use
 * @calls  uart0_gpio_init() uart0_nvic_init()
 * @called Communiction_proc()
 * @param  None
 * @retval None
 *********************************************************/
void uart0_init(uint32_t baudrate)
{
    uart0_gpio_init();

    rcu_periph_clock_enable(RCU_USART0);

    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudrate);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_address_config(USART0, '\r');
    usart_address_detection_mode_config(USART0, USART_ADDM_FULLBIT);

    usart_enable(USART0);

    uart0_nvic_init();
}

/*********************************************************
 * @name   uart1_gpio_init
 * @brief  USART1 GPIO initialization. Both uart port pins and direction control pins are configured
 * @calls  device drivers
 * @called uart1_init()
 * @param  None
 * @retval None
 *********************************************************/
static void uart1_gpio_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);
    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);

    // RS485 direction control
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    U1_RX_EN();
}

/*********************************************************
 * @name   uart1_nvic_init
 * @brief  USART1 related IRQ vector initialization, include DMA and USART IRQs
 * @calls  device drivers
 * @called uart1_init()
 * @param  None
 * @retval None
 *********************************************************/
static void uart1_nvic_init(void)
{
    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0, 0);
    /* enable USART TBE interrupt */
    usart_interrupt_enable(USART1, USART_INT_RBNE);
}

/*********************************************************
 * @name   uart1_init
 * @brief  General USART1 initialization, after which USART1 is ready to use
 * @calls  uart1_gpio_init() uart1_nvic_init()
 * @called Communiction_proc()
 * @param  None
 * @retval None
 *********************************************************/
void uart1_init(uint32_t baudrate)
{
    uart1_gpio_init();

    rcu_periph_clock_enable(RCU_USART1);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, baudrate);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_address_config(USART1, '\r');
    usart_address_detection_mode_config(USART1, USART_ADDM_FULLBIT);

    usart_enable(USART1);

    uart1_nvic_init();
}

/*********************************************************
  * @name   uart1_dma_send
    * @brief  send data from USART1 port with DMA and auto direction control(RS485)
    * @calls  device drivers
  * @called framing()
  * @param  s_addr  : data buffer to be sent out through uart port
                        length : data length
  * @retval None
*********************************************************/
void uart1_dma_send(uint8_t *s_addr, uint16_t length)
{
    dma_parameter_struct dma_init_struct;

    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    /* deinitialize DMA channel3 */
    dma_deinit(DMA_CH3);
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr  = (uint32_t)s_addr;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number       = length;
    dma_init_struct.periph_addr  = (uint32_t)(&USART_TDATA(USART1));
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_MEDIUM;
    dma_init(DMA_CH3, &dma_init_struct);

    nvic_irq_enable(DMA_Channel3_4_IRQn, 0, 0);
    dma_interrupt_enable(DMA_CH3, DMA_INT_FTF);
    // usart_interrupt_enable(USART1, USART_INT_IDLE);
    // usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);

    // usart_interrupt_disable(USART1, USART_INT_RBNE);

    /* configure DMA mode */
    dma_circulation_disable(DMA_CH3);
    dma_memory_to_memory_disable(DMA_CH3);

    /* USART DMA enable for transmission */
    usart_dma_transmit_config(USART1, USART_DENT_ENABLE);

    /* enable DMA transfer complete interrupt */

    /* enable DMA channel3 */
    dma_channel_enable(DMA_CH3);
}
