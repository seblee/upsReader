/*!
    \file  gd32f1x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
*/

#include "gd32f1x0_it.h"
#include "usart.h"
#define osObjectsPublic  // define objects in main module
#include "osObjects.h"   // RTOS object definitions

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void USART0_IRQHandler(void)
// {
//     if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
//     {
//         /* receive data */
//         if (rx0Count >= USART_BUF_SIZE)
//             rx0Count = 0;
//         rx1buffer[rx0Count++] = usart_data_receive(USART0);
//         usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
//     }
//     if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_AM))
//     {
//         usart_interrupt_flag_clear(USART0, USART_INT_FLAG_AM);
//         spiDataFifo(rx1buffer, rx1Count, 1);
//     }

//     if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))
//     {
//         /* transmit data */
//         usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TBE);
//     }
//     if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TC))
//     {
//         /* transmit data */
//         usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TC);
//         usart_interrupt_disable(USART0, USART_INT_TC);
//     }
// }

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {
        /* receive data */
        if (rx1Count >= USART_BUF_SIZE)
        {
            spiDataFifoPush(rx1buffer, rx1Count, 1);
            rx1Count = 0;
        }
        rx1buffer[rx1Count++] = usart_data_receive(USART1);
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
        usart_interrupt_enable(USART1, USART_INT_IDLE);
    }
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE))
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);
        usart_interrupt_enable(USART1, USART_INT_RBNE);
        usart_interrupt_disable(USART1, USART_INT_IDLE);
        spiDataFifoPush(rx1buffer, rx1Count, 1);
        rx1Count = 0;
    }

    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TBE))
    {
        /* transmit data */
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_TBE);
    }
    if (RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_TC))
    {
        /* transmit data */
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_TC);
        usart_interrupt_disable(USART1, USART_INT_TC);
        U1_RX_EN();
    }
}
/*!
       \brief      this function handles DMA_Channel3_4_IRQHandler interrupt
       \param[in]  none
       \param[out] none
       \retval     none
   */
#include <string.h>
#include "spi.h"
void DMA_Channel1_2_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA_CH1, DMA_INT_FLAG_FTF))
    {
        spiRxCallBack();
        dma_interrupt_flag_clear(DMA_CH1, DMA_INT_FLAG_G);
        dma_flag_clear(DMA_CH1, DMA_FLAG_G);
    }
    if (dma_interrupt_flag_get(DMA_CH2, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_G);
    }
}
/*!
    \brief      this function handles DMA_Channel3_4_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA_Channel3_4_IRQHandler(void)
{
    if (dma_interrupt_flag_get(DMA_CH3, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA_CH3, DMA_INT_FLAG_G);
        usart_interrupt_enable(USART1, USART_INT_TC);
    }
}
