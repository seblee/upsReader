
#include "cmsis_os.h"  // CMSIS RTOS header file
#include "spi.h"
#include "crc16.h"
#include "usart.h"
#include "string.h"
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

osPoolDef(mpool, 4, message_t);  // define memory pool
osPoolId mpool;

osMessageQDef(queue, 4, message_t);  // define message queue
osMessageQId queue;

int initSpiThread(void)
{
    tidSpiThread = osThreadCreate(osThread(spiThread), NULL);
    if (!tidSpiThread)
        return (-1);

    return (0);
}

// spiRX
void spiThread(void const *argument)
{
    osEvent Event;
    static uint8_t dataIndex = 0xa2;

    spiInit();
    uart1_init(115200);

    while (1)
    {
        Event = osSignalWait(1, osWaitForever);
        if (Event.status == osEventTimeout)
        {
        }
        else
        {
            if (dataIndex != spi0DmaRxBuffer[1])
            {
                dataIndex = spi0DmaRxBuffer[1];
                if (CRC16_Modbus(spi0DmaRxBuffer, spi0DmaRxBuffer[3] + 6) == 0)
                {
                    if (spi0DmaRxBuffer[2] == 0)
                    {
                        memcpy(tx0buffer, &spi0DmaRxBuffer[4], spi0DmaRxBuffer[3]);
                        comSendBuf(COM0, &spi0DmaRxBuffer[4], spi0DmaRxBuffer[3]);
                        // uart0_dma_send(tx0buffer, spi0DmaRxBuffer[3]);
                        // dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
                    }
                    if (spi0DmaRxBuffer[2] == 1)
                    {
                        dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
                        memcpy(tx1buffer, &spi0DmaRxBuffer[4], spi0DmaRxBuffer[3]);
                        uart1_dma_send(tx1buffer, spi0DmaRxBuffer[3]);
                    }
                }
            }
            Event = osMessageGet(queue, osWaitForever);  // wait for message to arrive

            if (Event.status == osEventMessage)
            {
                message_t *message = (message_t *)Event.value.p;  // read the pointer to memory pool buffer
                memcpy(spi0DmaTxBuffer, message->rxData, message->length);
                osPoolFree(mpool, message);  // Release the buffer
            }
        }
    }
}

void spiDataPack(uint8_t *cache, uint16_t len, uint8_t Uid)
{
    static uint8_t dataIndex = 0;
    uint16_t crc;
    message_t *message;
    message            = (message_t *)osPoolAlloc(mpool);  // Allocate a memory pool buffer
    message->rxData[0] = 0x68;
    message->rxData[1] = Uid;
    message->rxData[2] = dataIndex++;
    message->rxData[3] = len;
    memcpy(&(message->rxData[4]), cache, len);
    crc = CRC16_Modbus(message->rxData, len + 4);

    message->rxData[len + 4] = crc >> 8;
    message->rxData[len + 5] = crc;
    message->length          = len + 6;
    osMessagePut(queue, (uint32_t)message, osWaitForever);  // Post pointer to memory pool buffer
}
