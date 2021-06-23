
#include "cmsis_os.h"  // CMSIS RTOS header file
#include "usart.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
#define ARRAYNUM(arr_nanme) (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
uint8_t welcome[10] = {1,2,3,4,5,6,7,8,9,10};
 
void ups0Thread(void const *argument);            // thread function
osThreadId tid_Ups0Thread;                        // thread id
osThreadDef(ups0Thread, osPriorityNormal, 1, 0);  // thread object

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
    while (1)
    {
        osDelay(1000);
         

         uart1_dma_send(welcome, ARRAYNUM(welcome));

        // Insert thread code here...
        // osThreadYield();  // suspend thread
    }
}
