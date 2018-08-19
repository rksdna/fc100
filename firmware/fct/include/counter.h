/*
 * Reciprocal counter with vernier and serial peripheral interface
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

#ifndef COUNTER
#define COUNTER

#include <types.h>

struct counter
{
    u8_t dac_1;
    u8_t dac_2;
    u8_t mode;
    u8_t ctrl;
    u8_t tac_strt;
    u8_t tac_stop;
    u8_t ack;
    u8_t id;
    u32_t cnt;
    u32_t tmr;
};

#define COUNTER_DAC_DAC ((u8_t)0xFF)

#define COUNTER_MODE_STRT ((u8_t)0x03)
#define COUNTER_MODE_STRT_0 ((u8_t)0x01)
#define COUNTER_MODE_STRT_1 ((u8_t)0x02)
#define COUNTER_MODE_STRT_CH1R ((u8_t)0x00)
#define COUNTER_MODE_STRT_CH1F ((u8_t)0x01)
#define COUNTER_MODE_STRT_CH2R ((u8_t)0x02)
#define COUNTER_MODE_STRT_CH2F ((u8_t)0x03)

#define COUNTER_MODE_STOP ((u8_t)0x0C)
#define COUNTER_MODE_STOP_0 ((u8_t)0x04)
#define COUNTER_MODE_STOP_1 ((u8_t)0x08)
#define COUNTER_MODE_STOP_CH1R ((u8_t)0x00)
#define COUNTER_MODE_STOP_CH1F ((u8_t)0x04)
#define COUNTER_MODE_STOP_CH2R ((u8_t)0x08)
#define COUNTER_MODE_STOP_CH2F ((u8_t)0x0C)

#define COUNTER_MODE_CNT ((u8_t)0x30)
#define COUNTER_MODE_CNT_0 ((u8_t)0x10)
#define COUNTER_MODE_CNT_1 ((u8_t)0x20)
#define COUNTER_MODE_CNT_CH1R ((u8_t)0x00)
#define COUNTER_MODE_CNT_CH1F ((u8_t)0x10)
#define COUNTER_MODE_CNT_CH2R ((u8_t)0x20)
#define COUNTER_MODE_CNT_CH2F ((u8_t)0x30)

#define COUNTER_MODE_TMR ((u8_t)0x40)
#define COUNTER_MODE_TMR_CLK ((u8_t)0x00)
#define COUNTER_MODE_TMR_REF ((u8_t)0x40)

#define COUNTER_MODE_CLR ((u8_t)0x80)

#define COUNTER_CTRL_STRT ((u8_t)0x01)
#define COUNTER_CTRL_STOP ((u8_t)0x02)
#define COUNTER_CTRL_CLB_ZS ((u8_t)0x04)
#define COUNTER_CTRL_CLB_FS ((u8_t)0x08)
#define COUNTER_CTRL_HPF_CH1 ((u8_t)0x10)
#define COUNTER_CTRL_HPF_CH2 ((u8_t)0x20)
#define COUNTER_CTRL_TEST ((u8_t)0x40)
#define COUNTER_CTRL_CLR ((u8_t)0x80)

#define COUNTER_TAC_TAC ((u8_t)0xFF)

#define COUNTER_ACK_STOP ((u8_t)0x01)
#define COUNTER_ACK_STRT ((u8_t)0x02)

#define COUNTER_ID_ID ((u8_t)0xFF)

#define COUNTER_CNT_CNT ((u32_t)0xFFFFFFFF)
#define COUNTER_TMR_TMR ((u32_t)0xFFFFFFFF)

#endif

