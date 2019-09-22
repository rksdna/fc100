/*
 * Reciprocal counter USB bridge
 * Copyright (c) 2018 rksdna, murych
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stm32/syscfg.h>
#include <stm32/usart.h>
#include <stm32/flash.h>
#include <stm32/gpio.h>
#include <stm32/nvic.h>
#include <stm32/rcc.h>
#include <stm32/des.h>
#include <stm32/spi.h>
#include <stm32/adc.h>
#include <stm32/tim.h>
#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <tools.h>
#include "device.h"

#define V_REF 330

#define SPI1_DR_8 *(volatile u8_t *)(&SPI1->DR)

static s32_t voltage;

static void debug_put(void *data, char value)
{
    while (~USART1->ISR & USART_ISR_TXE)
        continue;

    USART1->TDR = value;
}

struct stream debug_stream = {debug_put, 0};

void startup_device(void)
{
    FLASH->ACR = FLASH_ACR_LATENCY_48MHz | FLASH_ACR_PRFTBE;

    RCC->CR = RCC_CR_HSEON | RCC_CR_HSION;
    wait_for(&RCC->CR, RCC_CR_HSERDY, RCC_CR_HSERDY);

    RCC->CFGR = RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC_HSE_PREDIV;
    RCC->CR = RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_PLLON;
    wait_for(&RCC->CR, RCC_CR_PLLRDY, RCC_CR_PLLRDY);

    RCC->CFGR = RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR_SW_PLL;
    RCC->CFGR2 = 0;
    RCC->CFGR3 = 0;

    RCC->AHBENR = RCC_AHBENR_SRAMEN | RCC_AHBENR_FLITFEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR = RCC_APB1ENR_USBEN | RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR = RCC_APB2ENR_USART1EN | RCC_APB2ENR_TIM17EN | RCC_APB2ENR_ADCEN | RCC_APB2ENR_SPI1EN;

    GPIOA->ODR = GPIO_ODR_ODR3 | GPIO_ODR_ODR4;
    GPIOA->MODER =
            /* NC      */ GPIO_MODER_GPI(0) |
            /* NC      */ GPIO_MODER_GPI(1) |
            /* NC      */ GPIO_MODER_GPI(2) |
            /* MCU_RST */ GPIO_MODER_GPO(3) |
            /* MCU_NSS */ GPIO_MODER_GPO(4) |
            /* MCU_SCK */ GPIO_MODER_AFO(5) |
            /* MCU_SDI */ GPIO_MODER_AFO(6) |
            /* MCU_SDO */ GPIO_MODER_AFO(7) |
            /* NC      */ GPIO_MODER_GPI(8) |
            /* SYS_TXD */ GPIO_MODER_AFO(9) |
            /* SYS_RXD */ GPIO_MODER_GPI(10) |
            /* USB_D-  */ GPIO_MODER_GPI(11) |
            /* USB_D+  */ GPIO_MODER_GPI(12) |
            /* SYS_SWD */ GPIO_MODER_GPI(13) |
            /* SYS_SWC */ GPIO_MODER_GPI(14) |
            /* NC      */ GPIO_MODER_GPI(15);
    GPIOA->AFRL = GPIO_AFRL(4, 0) | GPIO_AFRL(5, 0) | GPIO_AFRL(6, 0) | GPIO_AFRL(7, 0);
    GPIOA->AFRH = GPIO_AFRH(9, 1);

    GPIOB->ODR = 0;
    GPIOB->MODER =
            /* NC      */ GPIO_MODER_GPI(0) |
            /* SUP_2V5 */ GPIO_MODER_ANA(1) |
            /* NC      */ GPIO_MODER_GPI(2) |
            /* NC      */ GPIO_MODER_GPI(3) |
            /* NC      */ GPIO_MODER_GPI(4) |
            /* NC      */ GPIO_MODER_GPI(5) |
            /* NC      */ GPIO_MODER_GPI(6) |
            /* PWM_PP1 */ GPIO_MODER_AFO(7) |
            /* NC      */ GPIO_MODER_GPI(8) |
            /* PWM_PP2 */ GPIO_MODER_AFO(9) |
            /* NC      */ GPIO_MODER_GPI(10) |
            /* NC      */ GPIO_MODER_GPI(11) |
            /* NC      */ GPIO_MODER_GPI(12) |
            /* NC      */ GPIO_MODER_GPI(13) |
            /* NC      */ GPIO_MODER_GPI(14) |
            /* NC      */ GPIO_MODER_GPI(15);
    GPIOB->AFRL = GPIO_AFRL(7, 2);
    GPIOB->AFRH = GPIO_AFRH(9, 2);

    USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
    USART1->CR2 = 0;
    USART1->BRR = 48000000 / 115200 + 1;

    TIM3->PSC = 4800 - 1;
    TIM3->ARR = 200 - 1;
    TIM3->CR2 = TIM_CR2_MMS_1;
    TIM3->CR1 = TIM_CR1_CEN;

    TIM17->PSC = 8 - 1;
    TIM17->ARR = 20 - 1;
    TIM17->CCR1 = 10;
    TIM17->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    TIM17->CCER = TIM_CCER_CC1E | TIM_CCER_CC1NE;
    TIM17->BDTR = 10;
    TIM17->CR1 = TIM_CR1_CEN;

    ADC1->CFGR1 = ADC_CFGR1_EXTEN_0 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_0;
    ADC1->CFGR2 = 0;
    ADC1->SMPR = ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;
    ADC1->CHSELR = ADC_CHSELR_CHSEL9;

    ADC1->CR = ADC_CR_ADCAL;
    wait_for(&ADC1->CR, ADC_CR_ADCAL, 0);

    ADC1->IER = ADC_IER_EOCIE;
    NVIC->ISER = NVIC_ISER_SETENA_12;

    ADC1->CR = ADC_CR_ADEN;
    ADC1->CR = ADC_CR_ADSTART;

    SPI1->CR2 = SPI_CR2_FRXTH | SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE | SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0;

    start_timers_clock(48000);

    debug("device started\n");
    debug("- id: %*m\n", sizeof(DES->ID), DES->ID);
    debug("- flash: %dKbytes\n", DES->FSIZE & DES_FSIZE_FSIZE);
}

void irq12_handler(void)
{
    const s32_t sample = (2 * V_REF * ADC1->DR + 2048) / 4096;
    voltage = (3 * voltage + sample + 2) / 4;
    ADC1->ISR = ADC1->ISR;
}

s32_t get_device_voltage(void)
{
    return voltage;
}

static u8_t spi_poll(u8_t value)
{
    SPI1_DR_8 = value;
    wait_for(&SPI1->SR, SPI_SR_RXNE, SPI_SR_RXNE);

    return SPI1_DR_8;
}

void startup_device_counter(u32_t delay)
{
    TIM17->BDTR |= TIM_BDTR_MOE;
    sleep(delay);
    GPIOA->BSRR = GPIO_BSRR_BR3;
}

void shutdown_device_counter(void)
{
    GPIOA->BSRR = GPIO_BSRR_BS3;
    TIM17->BDTR &= ~TIM_BDTR_MOE;
}

void read_device_counter(u8_t address, void *destination, u32_t size)
{
    u8_t *ptr = (u8_t *)destination;
    GPIOA->BSRR = GPIO_BSRR_BR4;
    yield_thread(0, 0);

    spi_poll(address);
    while (size--)
        *ptr++ = spi_poll(0xFF);

    yield_thread(0, 0);
    GPIOA->BSRR = GPIO_BSRR_BS4;
}

void write_device_counter(u8_t address, const void *source, u32_t size)
{
    const u8_t *ptr = (const u8_t *)source;
    GPIOA->BSRR = GPIO_BSRR_BR4;
    yield_thread(0, 0);

    spi_poll(address | 0x80);
    while (size--)
        spi_poll(*ptr++);

    yield_thread(0, 0);
    GPIOA->BSRR = GPIO_BSRR_BS4;
}

__attribute__((naked))
s32_t __gnu_thumb1_case_uqi()
{
    asm volatile ("mov r12, r1\n"
                  "mov r1, lr\n"
                  "lsr r1, #1\n"
                  "lsl r1, #1\n"
                  "ldrb r1, [r1, r0]\n"
                  "lsl r1, #1\n"
                  "add lr, lr, r1\n"
                  "mov r1, r12\n"
                  "bx lr\n" : : : );
}

__attribute__((naked))
s32_t __gnu_thumb1_case_uhi()
{
    asm volatile ("push {r0, r1}\n"
                  "mov r1, lr\n"
                  "lsr r1, r1, #1\n"
                  "lsl r0, r0, #1\n"
                  "lsl r1, r1, #1\n"
                  "ldrh r1, [r1, r0]\n"
                  "lsl r1, r1, #1\n"
                  "add lr, lr, r1\n"
                  "pop {r0, r1}\n"
                  "bx lr\n" : : : );
}
