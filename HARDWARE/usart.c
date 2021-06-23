#include "usart.h"
#include "gd32f1x0.h"
#include <stdarg.h>
#include <stdio.h>

#define BPS 9600

#define USART0_TDATA_ADDRESS ((uint32_t)0x40013828) /* 130_150 device */
#define USART0_RDATA_ADDRESS ((uint32_t)0x40013824) /* 130_150 device */
/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/

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
    /* configure led GPIO port */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    RE485;
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
void uart1_init(uint16_t baudrate)  //???????
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

    /* configure DMA mode */
    dma_circulation_disable(DMA_CH3);
    dma_memory_to_memory_disable(DMA_CH3);
    /* USART interrupt configuration */
    nvic_irq_enable(DMA_Channel3_4_IRQn, 0, 0);
    usart_interrupt_disable(USART1, USART_INT_RBNE);
    dma_interrupt_enable(DMA_CH3, DMA_INT_FTF);

    TE485;
    /* enable DMA channel3 */
    dma_channel_enable(DMA_CH3);
    /* USART DMA enable for transmission and reception */
    usart_dma_transmit_config(USART1, USART_DENT_ENABLE);
}
