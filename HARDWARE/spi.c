/**
 * @file spi.c
 * @author  xiaowine (xiaowine@sina.cn)
 * @brief
 * @version 01.00
 * @date    2021-07-08
 *
 * @copyright Copyright (c) {2020}  xiaowine
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-07-08 <td>1.0     <td>wangh     <td>内容
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
#include "spi.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BufferSize 256
#define transSize 10
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t spi0DmaRxBuffer[BufferSize] = {0};
uint8_t spi0DmaTxBuffer[BufferSize] = {1, 3, 7, 15, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spiRcuConfig(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_DMA);
    rcu_periph_clock_enable(RCU_SPI0);
}
/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpioConfig(void)
{
    /* SPI0 GPIO config: NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dmaConfig(void)
{
    dma_parameter_struct dma_init_struct;
    /* SPI0 transmit dma config */
    dma_deinit(DMA_CH2);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)spi0DmaTxBuffer;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_init_struct.number       = transSize;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA_CH2, &dma_init_struct);
    /* SPI0_Tx DMA channel */
    dma_channel_disable(DMA_CH2);

    /* SPI0 receive dma config */
    dma_deinit(DMA_CH1);
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr = (uint32_t)spi0DmaRxBuffer;
    dma_init_struct.direction   = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.priority    = DMA_PRIORITY_LOW;
    dma_init(DMA_CH1, &dma_init_struct);
    /* SPI0_Rx DMA channel */
    dma_channel_disable(DMA_CH1);

    // nvic_irq_enable(DMA_Channel1_2_IRQn, 0, 0);
    // dma_interrupt_enable(DMA_CH1, DMA_INT_FTF);
    // dma_interrupt_enable(DMA_CH2, DMA_INT_FTF);

    // dma_circulation_enable(DMA_CH1);
    // dma_circulation_enable(DMA_CH2);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spiConfig(void)
{
    spi_parameter_struct spi_init_struct;

    spi_i2s_deinit(SPI0);
    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_HARD;
    spi_init_struct.prescale             = SPI_PSC_4;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);
    /* SPI enable */
    spi_enable(SPI0);
    /* SPI DMA enable */
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI0, SPI_DMA_RECEIVE);
}

void spiRestart(void)
{
    /* DMA channel disable */
    /* SPI0_Tx DMA channel */
    dma_channel_disable(DMA_CH2);
    /* SPI0_Rx DMA channel */
    dma_channel_disable(DMA_CH1);

    dma_transfer_number_config(DMA_CH2, transSize);
    dma_transfer_number_config(DMA_CH1, transSize);

    /* SPI0_Tx DMA channel */
    dma_channel_enable(DMA_CH2);
    /* SPI0_Rx DMA channel */
    dma_channel_enable(DMA_CH1);
}
void spiInit(void)
{
    spiRcuConfig();
    gpioConfig();
    dmaConfig();
    spiConfig();
    spiRestart();
}
