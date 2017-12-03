#include <stm32/syscfg.h>
#include <stm32/usart.h>
#include <stm32/flash.h>
#include <stm32/gpio.h>
#include <stm32/rcc.h>
#include <stm32/des.h>
#include <stm32/spi.h>
#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <tools.h>

static void debug_put(void *data, char value)
{
   while (~USART2->ISR & USART_ISR_TXE)
       continue;

   USART2->TDR = value;
}

struct stream debug_stream = {debug_put, 0};

void startup_board(void)
{
   RCC->AHBENR = RCC_AHBENR_SRAMEN | RCC_AHBENR_FLITFEN | RCC_AHBENR_GPIOAEN;
   RCC->APB1ENR = RCC_APB1ENR_USART2EN;
   RCC->APB2ENR = RCC_APB2ENR_SYSCFGCOMPEN | RCC_APB2ENR_SPI1EN;

   SYSCFG->CFGR1 = SYSCFG_CFGR1_PA11_PA12_RMP;

   GPIOA->ODR = GPIO_ODR_ODR7;
   GPIOA->MODER =
           /* CPL_NSS */ GPIO_MODER_GPO(4) |
           /* CPL_SCK */ GPIO_MODER_AFO(5) |
           /* CPL_SDI */ GPIO_MODER_AFO(6) |
           /* CPL_SDO */ GPIO_MODER_AFO(7) |
           /* SYS_TXD */ GPIO_MODER_AFO(14) |
           /* SYS_RXD */ GPIO_MODER_GPI(15);
   GPIOA->AFRL = GPIO_AFRL(4, 0) | GPIO_AFRL(5, 0) | GPIO_AFRL(6, 0);
   GPIOA->AFRH = GPIO_AFRH(14, 1);

   USART2->CR1 = USART_CR1_UE | USART_CR1_TE;
   USART2->CR2 = 0;
   USART2->BRR = 8000000 / 115200 + 1;

   SPI1->CR2 = SPI_CR2_FRXTH | SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;
   SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE | SPI_CR1_BR;

   start_timers_clock(8000);

   debug("hello\n");
   debug("id: %*m flash: %dKbytes\n", sizeof(DES->ID), DES->ID, DES->FSIZE & DES_FSIZE_FSIZE);
}

#define SPI1_DR_8 *(volatile u8_t *)(&SPI1->DR)

u8_t spi_poll(u8_t value)
{
    SPI1_DR_8 = value;
    wait_for(&SPI1->SR, SPI_SR_RXNE, SPI_SR_RXNE);
    return SPI1_DR_8;
}

void main(void)
{
    startup_board();

    while (1)
    {
        u8_t s;
        GPIOA->BSRR = GPIO_BSRR_BR4;
        s = spi_poll(0xA5);
        GPIOA->BSRR = GPIO_BSRR_BS4;

        debug(" -> %x\n", s);
        sleep(50);
    }
}
