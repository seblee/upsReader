/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic  // define objects in main module
#include "osObjects.h"   // RTOS object definitions
#include "led.h"
#include "fwdgt.h"
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
//
// <h>main Configuration
// =======================
//
//   <o>Number of concurrent  BUFFSIZE <1-250>
//   <i> Defines max. number of user threads that will run at the same time.
//   <i> Default: 6
#ifndef BUFFSIZE
#define BUFFSIZE 5
#endif

/*
 * main: initialize and start the system
 */
int main(void)
{
    // osKernelInitialize();  // initialize CMSIS-RTOS

    // initialize peripherals here
    led_init();
    IWDG_Configuration();

    // create 'thread' functions that start executing,
    // example: tid_name = osThreadCreate (osThread(name), NULL);
    Init_Ups0Thread();

    // osKernelStart();  // start thread execution
    while (1)
    {
        osDelay(500);
        led_toggle();  // Insert thread code here...
        fwdgt_counter_reload();
        // osThreadYield();  // suspend thread
    }
}

#ifdef USE_FULL_ASSERT
/**
 * [url=home.php?mod=space&uid=247401]@brief[/url]  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#else
void __aeabi_assert(const char* x1, const char* x2, int x3)
{
    while (1)
    {
    }
}
#endif
