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
#include "counter.h"
#include "device.h"
#include "shp.h"

/*
 * MODE START STOP COUNTER DURATION DISPLAY
 * F    A/B   A/B  A/B     1mS-10S  COUNTER/TIMER, Hz
 * T    A/B   A/B  A/B     1mS-10S  TIMER/COUNTER, S
 * RPM  A/B   A/B  A/B     1mS-10S  COUNTER/TIMER * 60, RPM
 * CNT  A/B   A/B  A/B     inf      COUNTER, 1
 * tP   A/B   A/B  A/B     0        TIMER, S
 * tL  !A/B   A/B  A/B     0        TIMER, S
 * tH   A/B  !A/B  A/B     0        TIMER, S
 * tD   A/B   B/A  A/B     0        TIMER, S
 * ARM  B/A  !B/A  A/B     0        COUNTER/TIMER, Hz,
 */

/*static struct counter cnt;

static void update(struct counter *regs)
{
    write_counter(0, (void*)regs, 4);
    read_counter(0, (void*)regs, 16);
}

static void dump(const struct counter *regs)
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
}*/

enum control_state
{
    CONTROL_IDLE_WO,
    CONTROL_IDLE,
};

static enum control_state state;
static struct thread counter_thread;
static u8_t counter_stack[256];

static void counter(void *data)
{
    switch (state)
    {
    case CONTROL_IDLE_WO:
        static struct counter regs;

    case CONTROL_IDLE:

    }

}

static void handler(struct shp_socket *socket, void *data, u32_t size)
{
    send_shp_response(socket, data, size);
}

void main(void)
{
    startup_device();
    start_thread(&control_thread, (function_t)control, 0, control_stack, sizeof(control_stack));
    start_cdc_service();
    set_cdc_timeout(10);
    while (1)
    {
        struct shp_socket socket;
        bind_shp_socket(&socket, handler, read_cdc_data, write_cdc_data);

        yield_thread((condition_t)has_cdc_connection, 0);
        debug("connected\n");
        while (has_cdc_connection())
            poll_shp_socket(&socket);

        debug("disconnected\n");
    }
}
