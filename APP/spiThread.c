
#include "cmsis_os.h"  // CMSIS RTOS header file
#include "spi.h"
#include "crc16.h"
#include "usart.h"
#include "string.h"
#include "led.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/

void spiThread(void const *argument);            // thread function
osThreadId tidSpiThread;                         // thread id
osThreadDef(spiThread, osPriorityNormal, 1, 0);  // thread object

typedef struct
{
    uint8_t rxData[128];
    uint16_t length;
} message_t;
#define mPoolSize 3 
osPoolId mpool;
osPoolDef(mpool, mPoolSize, message_t);  // define memory pool

osMessageQId spiQueue;
osMessageQDef(spiQueue, 5, unsigned int);  // define message queue

static message_t *messageLib[mPoolSize] = {0};
static uint8_t readIndex = 0, writeIndex = 0, packageCount = 0;

static uint16_t spiDataFifoPull(uint8_t *cache);

int initSpiThread(void)
{
    tidSpiThread = osThreadCreate(osThread(spiThread), NULL);
    if (!tidSpiThread)
        return (-1);
    mpool = osPoolCreate(osPool(mpool));
    if (!mpool)
        return (-1);
    spiQueue = osMessageCreate(osMessageQ(spiQueue), NULL);
    if (!spiQueue)
        return (-1);
    return (0);
}

void spiThread(void const *argument)
{
    osEvent Event;
    static uint8_t dataIndex = 0xa2;

    uart1_init(2400);
    osDelay(100);
    spiInit();
    while (1)
    {
        Event = osMessageGet(spiQueue, osWaitForever);
        if (Event.status == osEventTimeout)
        {
        }
        else
        {
            uint8_t *p;
            p = spi0DmaRxBuffer;

            if (spi0DmaRxBuffer[0] == 0x68)
            {
                if (dataIndex != *(p + 1))
                {
                    uint16_t crc;

                    crc = CRC16_Modbus(spi0DmaRxBuffer, *(p + 3) + 6);
                    if (crc == 0)
                    {
                        dataIndex = *(p + 1);
                        if (*(p + 2) == 0)
                        {
                            memcpy(tx0buffer, p + 4, *(p + 3));
                            comSendBuf(COM0, p + 4, *(p + 3));
                        }

                        if (*(p + 2) == 1)
                        {
                            memcpy(tx1buffer, p + 4, *(p + 3));
                            U1_TX_EN();
                            uart1_dma_send(tx1buffer, *(p + 3));
                            dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
                        }
                    }
                    else
                    {
                        led_toggle();  // Insert thread code here...
                    }
                }
            }
            memset(spi0DmaRxBuffer, 0, 128);
            spiDataFifoPull(spi0DmaTxBuffer);
            spiRestart();
        }
    }
}

void spiDataFifoPush(uint8_t *cache, uint16_t len, uint8_t Uid)
{
    static uint8_t dataId = 0;
    uint16_t crc;
    message_t *message;
    if (len == 0)
        return;
    message = (message_t *)osPoolAlloc(mpool);  // Allocate a memory pool buffer
    if (!message)
        return;
    message->rxData[0] = 0x68;
    message->rxData[1] = dataId++;
    message->rxData[2] = Uid;
    message->rxData[3] = len;
    memcpy(&(message->rxData[4]), cache, len);
    crc = CRC16_Modbus(message->rxData, len + 4);

    message->rxData[len + 4] = crc >> 8;
    message->rxData[len + 5] = crc;
    message->length          = len + 6;

    messageLib[writeIndex] = message;
    if (++writeIndex >= mPoolSize)
    {
        writeIndex = 0;
    }
    if (packageCount < mPoolSize)
    {
        packageCount++;
    }
}
static uint16_t spiDataFifoPull(uint8_t *cache)
{
    message_t *message;
    uint16_t length = 0;
    if (packageCount == 0)
        return 0;

    message = messageLib[readIndex];
    if (++readIndex >= mPoolSize)
    {
        readIndex = 0;
    }
    memcpy(cache, message->rxData, message->length);
    length = message->length;
    packageCount--;
    osPoolFree(mpool, message);  // Release the buffer
    return length;
}

void spiRxCallBack(void)
{
    osMessagePut(spiQueue, 1, 0);  // Post pointer to memory pool buffer
    // {
    //     memcpy(tx1buffer, spi0DmaRxBuffer, 30);
    //     U1_TX_EN();
    //     uart1_dma_send(tx1buffer, 30);
    //     dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
    // }
}
