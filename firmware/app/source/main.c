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

   GPIOA->ODR = GPIO_ODR_ODR4;
   GPIOA->MODER =
           /* CPL_RST */ GPIO_MODER_GPO(3) |
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
   SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE | SPI_CR1_BR_2;

   start_timers_clock(8000);

   debug("hello\n");
   debug("id: %*m flash: %dKbytes\n", sizeof(DES->ID), DES->ID, DES->FSIZE & DES_FSIZE_FSIZE);
}

struct fc_tx_frame
{
    u8_t bis;
    u8_t eis;
    u8_t xis;
    u8_t ris;

    u32_t dummy;
    u8_t cr;
};

struct fc_rx_frame
{
    u32_t cnr;
    u32_t cnx;
    u8_t sr;
};

static struct fc_tx_frame fc_tx = {0x00, 0x00, 0x00, 0x00, 0xFFFFFFFF, 0x00};
static struct fc_rx_frame fc_rx = {0x00000000, 0x00000000, 0x00};

#define SPI1_DR_8 *(volatile u8_t *)(&SPI1->DR)

void fc_poll(void)
{
    const u8_t * txp = (void *)&fc_tx;
    u8_t * rxp = (void *)&fc_rx;
    u32_t n = 9;

    GPIOA->BSRR = GPIO_BSRR_BR4;

    while (n--)
    {
        SPI1_DR_8 = txp[n];
        wait_for(&SPI1->SR, SPI_SR_RXNE, SPI_SR_RXNE);
        rxp[n] = SPI1_DR_8;
    }

    GPIOA->BSRR = GPIO_BSRR_BS4;

    debug("%*m > %*m\n", 9, (void *)&fc_tx, 9, (void *)&fc_rx);
}

void fx_m(void)
{
    GPIOA->BSRR = GPIO_BSRR_BS3;

    fc_tx.bis = 0x00;
    fc_tx.eis = 0x00;
    fc_tx.xis = 0x00;
    fc_tx.ris = 0x00;
    fc_tx.cr = 0x80;
    fc_poll();

    fc_tx.cr = 0x81;
    fc_poll();

    sleep(1000);

    fc_tx.cr = 0x83;
    fc_poll();

    while (1)
    {
        fc_poll();
        if (fc_rx.sr == 0xAB)
        {
            debug("%d / %d \n", fc_rx.cnx, fc_rx.cnr);
            break;
        }
        sleep(10);
    }

    GPIOA->BSRR = GPIO_BSRR_BR3;
}

static u8_t espi(u8_t value)
{
    SPI1_DR_8 = value;
    wait_for(&SPI1->SR, SPI_SR_RXNE, SPI_SR_RXNE);
    return SPI1_DR_8;
}

static void fc_read(u32_t address, u8_t *data, u32_t count)
{
    u32_t n = count;

    GPIOA->BSRR = GPIO_BSRR_BR4;

    espi(address & 0x0F);
    while (count--)
        *data++ = espi(0x00);

    GPIOA->BSRR = GPIO_BSRR_BS4;

    debug("r %x %*m\n", address, n, data - n);
}

static void fc_write(u32_t address, const u8_t *data, u32_t count)
{
    u32_t n = count;

    GPIOA->BSRR = GPIO_BSRR_BR4;

    espi(0x80 | (address & 0x0F));
    while (count--)
        espi(*data++);

    GPIOA->BSRR = GPIO_BSRR_BS4;

    debug("w %x %*m\n", address, n, data - n);
}

void main(void)
{
    u8_t buf[16];
    startup_board();

    while (1)
    {
        GPIOA->BSRR = GPIO_BSRR_BS3;

        fc_read(0, buf, 16);

        GPIOA->BSRR = GPIO_BSRR_BR3;
        sleep(250);
    }

}
