#include "cmsis_os.h"  // CMSIS RTOS header file
#include "usart.h"
#include "upsContext.h"
#include <string.h>
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
// #define ARRAYNUM(arr_nanme) (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
// uint8_t welcome[] = {'h', 'i', 't', 'h', 'i', 's', ' ', 'i', 's', ' ', 'a', ' ', 'e',  'x', 'a',
//                      'm', 'p', 'l', 'e', ':', ' ', 'R', 'A', 'M', '_', 'T', 'O', '_',  'U', 'S',
//                      'A', 'R', 'T', ' ', 'b', 'y', ' ', 'D', 'M', 'A', ' ', '!', '\r', '\n'};

void ups0Thread(void const *argument);            // thread function
osThreadId tid_Ups0Thread;                        // thread id
osThreadDef(ups0Thread, osPriorityNormal, 1, 0);  // thread object
osEvent ups0Event;

int Init_Ups0Thread(void)
{
    tid_Ups0Thread = osThreadCreate(osThread(ups0Thread), NULL);
    if (!tid_Ups0Thread)
        return (-1);

    return (0);
}

void ups0Thread(void const *argument)
{
    uart1_init(115200);
    contextVariablesInit();
    while (1)
    {
        ups0Event = osSignalWait(1, 500);
        if (ups0Event.status == osEventTimeout)
        {
            upsCommand(UPS_QMD, 5000); 
            // uart1_dma_send(welcome, ARRAYNUM(welcome));
        }
        else
        {
            ups0Event.status = osOK;
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
