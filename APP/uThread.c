#include "cmsis_os.h"  // CMSIS RTOS header file
#include "usart.h"
#include <string.h>
#include "spi.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
// #define ARRAYNUM(arr_nanme) (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
// uint8_t welcome[] = {'h', 'i', 't', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 'e',  'x', 'a',
//                      'm', 'p', 'l', 'e', ':', ' ', 'R', 'A', 'M', '_', 'T', 'O', '_',  'U', 'S',
//                      'A', 'R', 'T', ' ', 'b', 'y', ' ', 'D', 'M', 'A', ' ', '!', '\r', '\n'};

void u1TxThread(void const *argument);            // thread function
osThreadId tid_u1TxThread;                        // thread id
osThreadDef(u1TxThread, osPriorityNormal, 1, 0);  // thread object
osEvent u1TxEvent;

int Init_u1TxThread(void)
{
    tid_u1TxThread = osThreadCreate(osThread(u1TxThread), NULL);
    if (!tid_u1TxThread)
        return (-1);

    return (0);
}

void u1TxThread(void const *argument)
{
    while (1)
    {
        u1TxEvent = osSignalWait(0, osWaitForever);
        if (u1TxEvent.status == osEventTimeout)
        {
            // spiRestart();
            // uart1_dma_send(welcome, ARRAYNUM(welcome));
        }
        else
        {
            if (dma_interrupt_flag_get(DMA_CH1, DMA_INT_FLAG_FTF))
            {
                memcpy(tx1buffer, spi0DmaRxBuffer, 10);
                uart1_dma_send(tx1buffer, 10);
                dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
                spiRestart();
            }

            // rxProcess();
            //               char *p;
            //            p = strstr((void *)rx1buffer, commandMap[UPS_Q3GS].bkHead);
            //            if ((p != NULL) && (commandMap[UPS_Q3GS].process != NULL))
            //            {
            //                commandMap[UPS_Q3GS].process(p);
            //            }
        }

        // Insert thread code here...
        // osThreadYield();  // suspend thread
    }
}
