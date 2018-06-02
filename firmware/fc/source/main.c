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

#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <cdc.h>

#include "device.h"

#define FC_DAC_DAC 0xFF

#define FC_MODE_STRT 0x03
#define FC_MODE_STRT_0 0x01
#define FC_MODE_STRT_1 0x02

#define FC_MODE_STRT_CH1R 0x00
#define FC_MODE_STRT_CH1F 0x01
#define FC_MODE_STRT_CH2R 0x02
#define FC_MODE_STRT_CH2F 0x03

#define FC_MODE_STOP 0x0C
#define FC_MODE_STOP_0 0x04
#define FC_MODE_STOP_1 0x08
#define FC_MODE_STOP_CH1R 0x00
#define FC_MODE_STOP_CH1F 0x04
#define FC_MODE_STOP_CH2R 0x08
#define FC_MODE_STOP_CH2F 0x0C

#define FC_MODE_CNT 0x30
#define FC_MODE_CNT_0 0x10
#define FC_MODE_CNT_1 0x20

#define FC_MODE_CNT_CH1R 0x00
#define FC_MODE_CNT_CH1F 0x10
#define FC_MODE_CNT_CH2R 0x20
#define FC_MODE_CNT_CH2F 0x30

#define FC_MODE_TMR 0x40
#define FC_MODE_TMR_CLK 0x00
#define FC_MODE_TMR_REF 0x40

#define FC_MODE_CLR 0x80

#define FC_CTRL_STRT 0x01
#define FC_CTRL_STOP 0x02
#define FC_CTRL_CLB_ZS 0x04
#define FC_CTRL_CLB_FS 0x08
#define FC_CTRL_HPF_CH1 0x10
#define FC_CTRL_HPF_CH1 0x20
#define FC_CTRL_TEST 0x40
#define FC_CTRL_CLR 0x80

#define FC_TAC_TAC 0xFF

#define FC_ACK_STOP 0x01
#define FC_ACK_STRT 0x02

#define FC_ID_ID 0xFF

#define FC_CNT_CNT 0xFFFFFFFF
#define FC_TMR_TMR 0xFFFFFFFF

struct fc_regs
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

static void dump(const struct fc_regs *regs)
{
    debug("-----------\n");
    debug(" dac1:\t%d\n", regs->dac_1);
    debug(" dac2:\t%d\n", regs->dac_2);
    debug(" mode:\t%x\n", regs->mode);
    debug(" control:\t%x\n", regs->ctrl);
    debug(" strt_tac:\t%d\n", regs->tac_strt);
    debug(" stop_tac:\t%d\n", regs->tac_stop);
    debug(" status:\t%x\n", regs->ack);
    debug(" id:\t%x\n", regs->id);
    debug(" counter:\t%d\n", regs->cnt);
    debug(" timer:\t%d\n", regs->tmr);
}

void read_rr(struct fc_regs *regs)
{
    read_counter(0, (void*)regs, 16);
}

void write_rr(const struct fc_regs *regs)
{
    write_counter(0, (void*)regs, 4);
}

void main(void)
{
    startup_device();
    struct fc_regs regs;

   /* startup_counter();

    sleep(100);

    read_rr(&regs);
    dump(&regs);

    regs.dac1 = 128;
    regs.control = 0x40;
    write_rr(&regs);

    dump(&regs);*/

    while (1)
    {
        debug("begin\n");
        startup_counter();

        sleep(100);

        read_rr(&regs);
        dump(&regs);

        regs.dac_1 = 255;
        write_rr(&regs);
        dump(&regs);

        sleep(100);

        regs.ctrl = FC_CTRL_STRT;
        write_rr(&regs);
        dump(&regs);

        sleep(1000);

        regs.ctrl = FC_CTRL_STRT | FC_CTRL_STOP;
        write_rr(&regs);
        dump(&regs);

        sleep(100);

        read_rr(&regs);
        dump(&regs);

        shutdown_counter();

        sleep(1000);
    }

 /*   u8_t a = 0;
    u8_t c = 0x40;
    write_counter(3, &c, 1);


    u8_t s = 0;
    while (1)
    {
        write_counter(0, &a, 1);
        write_counter(1, &a, 1);

        read_counter(3, &s, 1);
        debug("%x\n", s);

        a++;
        c ^= 0x40;

        sleep(100);
    }*/

    start_cdc_service();
    set_cdc_timeout(10);
    while (1)
    {
        yield_thread((condition_t)has_cdc_connection, 0);
        debug("connected\n");
        while (has_cdc_connection())
        {
            static u8_t buffer[256];
            const u32_t count = read_cdc_data(buffer, sizeof(buffer));
            if (count)
            {
                write_cdc_data(buffer, count);
                debug("%m\n", buffer, count);
            }
        }

        debug("disconnected\n");
    }
}
