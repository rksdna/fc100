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
#include "threads.h"
#include "counter.h"
#include "device.h"
#include "tools.h"
#include "shp.h"

/*
 * MODE START STOP COUNTER DURATION DISPLAY
 * F    A/B   A/B  A/B     1mS-10S  COUNTER/TIMER, Hz
 * T    A/B   A/B  A/B     1mS-10S  TIMER/COUNTER, S
 * RPM  A/B   A/B  A/B     1mS-10S  COUNTER/TIMER * 60, RPM
 * CNT  A/B   A/B  A/B     inf      COUNTER, 1
 * ARM  B/A  !B/A  A/B     0        COUNTER/TIMER
 * tP   A/B   A/B  A/B     0        TIMER, S
 * tL  !A/B   A/B  A/B     0        TIMER, S
 * tH   A/B  !A/B  A/B     0        TIMER, S
 * tD   A/B   B/A  A/B     0        TIMER, S
 */

#define V_ON 485
#define V_OFF 475
#define TIMER_PERIOD 10

enum counter_state
{
    OFF_STATE,
    ENTER_STARTUP_STATE,
    STARTUP_STATE,
    ENTER_IDLE_STATE,
    IDLE_STATE,
    ENTER_ZS_CALIBRATION_STATE,
    ZS_CALIBRATION_STATE,
    ENTER_FS_CALIBRATION_STATE,
    FS_CALIBRATION_STATE,

    ENTER_START_STATE,
    START_STATE,

    ENTER_FAILURE_STATE,
    FAILURE_STATE,

};

struct counter_context
{
    struct counter regs;
};

static void update_regs(struct counter *regs)
{
    write_device_counter(0, regs, 4);
    read_device_counter(0, regs, 16);
}

enum counter_state process(struct counter_context *context, enum counter_state state, u32_t elapsed)
{
    const s32_t voltage = get_device_voltage();

    if (state > OFF_STATE && state < ENTER_FAILURE_STATE && voltage < V_OFF)
        return ENTER_FAILURE_STATE;

    if (state > ENTER_IDLE_STATE && state < ENTER_FAILURE_STATE && context->regs.id != 0xAA)
        return ENTER_FAILURE_STATE;

    switch (state)
    {
    case OFF_STATE:
        if (voltage > V_ON)
            return ENTER_STARTUP_STATE;

        break;

    case ENTER_STARTUP_STATE:
        switch_on_device_counter();

    case STARTUP_STATE:
        if (elapsed > 25)
            return ENTER_IDLE_STATE;
        break;

    case ENTER_IDLE_STATE:
        debug("idle\n");
        startup_device_counter();
        context->regs.mode = COUNTER_MODE_CLR;
        context->regs.ctrl = COUNTER_CTRL_CLR;

    case IDLE_STATE:
        update_regs(&context->regs);
        if (elapsed > 500)
            return ENTER_ZS_CALIBRATION_STATE;
        return IDLE_STATE;

    case ENTER_ZS_CALIBRATION_STATE:
        context->regs.mode = COUNTER_MODE_CLR;
        context->regs.ctrl = COUNTER_CTRL_CLB_ZS | COUNTER_CTRL_TEST;

    case ZS_CALIBRATION_STATE:
        update_regs(&context->regs);
        if ((context->regs.ack & COUNTER_ACK_STRT) && (context->regs.ack & COUNTER_ACK_STOP))
        {
            debug("zs %d %d\n", context->regs.tac_stop, context->regs.tac_strt);
            return ENTER_FS_CALIBRATION_STATE;
        }

        return ZS_CALIBRATION_STATE;

    case ENTER_FS_CALIBRATION_STATE:
        context->regs.mode = COUNTER_MODE_CLR;
        context->regs.ctrl = COUNTER_CTRL_CLB_FS | COUNTER_CTRL_TEST;

    case FS_CALIBRATION_STATE:
        update_regs(&context->regs);
        if ((context->regs.ack & COUNTER_ACK_STRT) && (context->regs.ack & COUNTER_ACK_STOP))
        {
            debug("fs %d %d\n", context->regs.tac_stop, context->regs.tac_strt);
            return ENTER_IDLE_STATE;
        }

        return FS_CALIBRATION_STATE;

    case ENTER_START_STATE:
        context->regs.mode = COUNTER_MODE_CLR;
        context->regs.ctrl = COUNTER_CTRL_STRT | COUNTER_CTRL_TEST;
        return START_STATE;

    case START_STATE:
        update_regs(&context->regs);
        break;


    case ENTER_FAILURE_STATE:
        shutdown_device_counter();
    case FAILURE_STATE:
        return FAILURE_STATE;

    default:
        return ENTER_FAILURE_STATE;
    }

    return state;
}

static void control(void *arg)
{
    WASTE(arg);

    u32_t elapsed = 0;
    struct counter_context context = {{0, 0, 0, 0}};
    enum counter_state state = OFF_STATE;

    struct timer tm;
    start_timer(&tm, TIMER_PERIOD);

    while (1)
    {
        const enum counter_state next = process(&context, state, elapsed);
        if (state != next)
        {
            state = next;
            elapsed = 0;
        }
        else
        {
            wait_timer(&tm);
            elapsed += TIMER_PERIOD;
        }
    }
}

static void handler(struct shp_socket *socket, const void *data, u32_t size)
{
    send_shp_response(socket, data, size);
}

void main(void)
{
    static struct thread control_thread;
    static u8_t control_stack[256];

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
