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

#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <tools.h>
#include <cdc.h>
#include "counter.h"
#include "device.h"
#include "slip.h"

#define V_ON 475
#define V_OFF 465

enum state
{
    IDLE_STATE,
    START_TIME_STATE,
    START_FREQUENCY_STATE,
    START_DUTY_STATE,
    START_GATED_FREQUENCY_STATE,
    DUTY_1_STATE,
    DUTY_2_STATE,
    DUTY_3_STATE,
    BUSY_1_STATE,
    BUSY_2_STATE,
    BUSY_3_STATE,
    CALIBRATION_1_STATE,
    CALIBRATION_2_STATE
};

enum
{
    POLL_COMMAND,
    START_TIME_COMMAND,
    START_FREQUENCY_COMMAND,
    START_DUTY_COMMAND,
    START_GATED_FREQUENCY_COMMAND
};

enum
{
    FAIL_STATUS,
    DONE_STATUS,
    BUSY_STATUS
};

struct parameters
{
    u32_t command;
    u32_t duration;
    u8_t threshold1;
    u8_t threshold2;
    u8_t coupling1;
    u8_t coupling2;
    u8_t ref_source;
    u8_t start_event;
    u8_t stop_event;
    u8_t count_event;
};

struct variables
{
    u32_t status;
    u32_t counter1;
    u32_t timer1;
    u32_t counter2;
    u32_t timer2;
    u8_t tac_start1;
    u8_t tac_stop1;
    u8_t tac_start2;
    u8_t tac_stop2;
    u8_t fs_start;
    u8_t fs_stop;
    u8_t zs_start;
    u8_t zs_stop;
};

static struct parameters params;
static struct variables vars;
static u32_t duration;

static enum state command(enum state state)
{
    switch (params.command)
    {
    case START_TIME_COMMAND:
        state = START_TIME_STATE;
        break;

    case START_FREQUENCY_COMMAND:
        state = START_FREQUENCY_STATE;
        break;

    case START_DUTY_COMMAND:
        state = START_DUTY_STATE;
        break;

    case START_GATED_FREQUENCY_COMMAND:
        state = START_GATED_FREQUENCY_STATE;
        break;

    default:
        break;
    }

    params.command = POLL_COMMAND;
    return state;
}

static enum state control(enum state state, u32_t elapsed, struct counter *regs)
{
    switch (state)
    {
    case IDLE_STATE:
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;
        regs->mode =
                COUNTER_MODE_TMR_BITS(params.ref_source) |
                COUNTER_MODE_CLR;
        regs->ctrl =
                COUNTER_CTRL_HPF_CH1_BITS(params.coupling1) |
                COUNTER_CTRL_HPF_CH2_BITS(params.coupling2) |
                COUNTER_CTRL_CLR;
        break;

    case START_TIME_STATE:
        duration = params.duration;
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;

        regs->mode =
                COUNTER_MODE_STRT_BITS(params.start_event) |
                COUNTER_MODE_STOP_BITS(params.stop_event) |
                COUNTER_MODE_CNT_BITS(params.stop_event) |
                COUNTER_MODE_TMR_BITS(params.ref_source) |
                COUNTER_MODE_CLR;

        regs->ctrl =
                COUNTER_CTRL_HPF_CH1_BITS(params.coupling1) |
                COUNTER_CTRL_HPF_CH2_BITS(params.coupling2) |
                COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;

        return BUSY_1_STATE;

    case START_FREQUENCY_STATE:
        duration = params.duration;
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;

        regs->mode =
                COUNTER_MODE_STRT_BITS(params.count_event) |
                COUNTER_MODE_STOP_BITS(params.count_event) |
                COUNTER_MODE_CNT_BITS(params.count_event) |
                COUNTER_MODE_TMR_BITS(params.ref_source) |
                COUNTER_MODE_CLR;

        regs->ctrl =
                COUNTER_CTRL_HPF_CH1_BITS(params.coupling1) |
                COUNTER_CTRL_HPF_CH2_BITS(params.coupling2) |
                COUNTER_CTRL_STRT;

        return BUSY_1_STATE;

    case START_DUTY_STATE:
        duration = params.duration;
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;

        regs->mode =
                COUNTER_MODE_STRT_BITS(params.count_event) |
                COUNTER_MODE_STOP_BITS(params.count_event) |
                COUNTER_MODE_CNT_BITS(params.count_event) |
                COUNTER_MODE_TMR_BITS(params.ref_source) |
                COUNTER_MODE_CLR;

        regs->ctrl =
                COUNTER_CTRL_HPF_CH1_BITS(params.coupling1) |
                COUNTER_CTRL_HPF_CH2_BITS(params.coupling2) |
                COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;

        return DUTY_1_STATE;

    case START_GATED_FREQUENCY_STATE:
        duration = params.duration;
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;

        regs->mode =
                COUNTER_MODE_STRT_BITS(params.start_event) |
                COUNTER_MODE_STOP_BITS(params.stop_event) |
                COUNTER_MODE_CNT_BITS(params.count_event) |
                COUNTER_MODE_TMR_BITS(params.ref_source) |
                COUNTER_MODE_CLR;

        regs->ctrl =
                COUNTER_CTRL_HPF_CH1_BITS(params.coupling1) |
                COUNTER_CTRL_HPF_CH2_BITS(params.coupling2) |
                COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;

        return BUSY_1_STATE;

    case DUTY_1_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STRT)
        {
            regs->ctrl |= COUNTER_CTRL_TEST;
            return DUTY_2_STATE;
        }

        if (elapsed > duration)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    case DUTY_2_STATE:
        if (regs->ack & COUNTER_ACK_STOP)
        {
            debug("B %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);
            vars.counter2 = regs->cnt;
            vars.timer2 = regs->tmr;
            vars.tac_start2 = regs->tac_strt;
            vars.tac_stop2 = regs->tac_stop;

            regs->mode ^= COUNTER_MODE_STOP_0 | COUNTER_MODE_CNT_0;
            regs->mode |= COUNTER_MODE_CLR;
            return BUSY_1_STATE;
        }

        if (elapsed > duration)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    case BUSY_1_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STRT)
        {
            regs->ctrl |= COUNTER_CTRL_TEST;
            return BUSY_2_STATE;
        }

        if (elapsed > duration)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    case BUSY_2_STATE:
        if (elapsed > duration)
        {
            regs->ctrl |= COUNTER_CTRL_STOP;
            return BUSY_3_STATE;
        }

        break;

    case BUSY_3_STATE:
        if (regs->ack & COUNTER_ACK_STOP)
        {
            debug("R %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);
            vars.counter1 = regs->cnt;
            vars.timer1 = regs->tmr;
            vars.tac_start1 = regs->tac_strt;
            vars.tac_stop1 = regs->tac_stop;

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~(COUNTER_CTRL_STRT | COUNTER_CTRL_STOP);
            regs->ctrl |= COUNTER_CTRL_CLB_FS;
            return CALIBRATION_1_STATE;
        }

        if (elapsed > duration)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    case CALIBRATION_1_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STOP)
        {
            debug("FS %d, %d\n", regs->tac_strt, regs->tac_stop);
            vars.fs_start = regs->tac_strt;
            vars.fs_stop = regs->tac_stop;

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~COUNTER_CTRL_CLB_FS;
            regs->ctrl |= COUNTER_CTRL_CLB_ZS;
            return CALIBRATION_2_STATE;
        }

        if (elapsed > 10)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    case CALIBRATION_2_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STOP)
        {
            debug("ZS %d, %d\n", regs->tac_strt, regs->tac_stop);
            vars.zs_start = regs->tac_strt;
            vars.zs_stop = regs->tac_stop;

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~(COUNTER_CTRL_CLB_ZS | COUNTER_CTRL_TEST);

            vars.status = DONE_STATUS;
            return IDLE_STATE;
        }

        if (elapsed > 10)
        {
            vars.status = FAIL_STATUS;
            return IDLE_STATE;
        }

        break;

    default:
        return IDLE_STATE;
    }

    return state;
}

static void control_handler(void *arg)
{
    WASTE(arg);

    struct timer tm;
    struct counter regs;
    static enum state state = IDLE_STATE;

    sleep(250);
    if (get_device_voltage() > V_ON)
    {
        debug("counter started\n");
        startup_device_counter(250);
        start_timer(&tm, -1);
        while (get_device_voltage() > V_OFF)
        {
            const enum state next = control(command(state), tm.ticks, &regs);
            if (next != state)
            {
                state = next;
                stop_timer(&tm);
                start_timer(&tm, -1);
            }

            write_device_counter(COUNTER_DAC1, &regs.dac1, 4);
            read_device_counter(COUNTER_ID, &regs.id, 12);
        }

        stop_timer(&tm);
        shutdown_device_counter();
        debug("counter stopped\n");
    }
}

static void socket_handler(struct slip_socket *socket, const void *data, u32_t size)
{
    if (size == sizeof(struct parameters))
    {
        copy(&params, data, sizeof(struct parameters));
        if (params.command != POLL_COMMAND)
            vars.status = BUSY_STATUS;

        send_slip_response(socket, &vars, sizeof(struct variables));
    }
}

void main(void)
{
    static struct thread control_thread;
    static u8_t control_stack[256];
    startup_device();
    start_thread(&control_thread, (function_t)control_handler, 0, control_stack, sizeof(control_stack));
    start_cdc_service();
    set_cdc_timeout(10);
    while (1)
    {
        struct slip_socket socket;
        bind_slip_socket(&socket, socket_handler, read_cdc_data, write_cdc_data);

        yield_thread((condition_t)has_cdc_connection, 0);
        debug("cdc connected\n");
        while (has_cdc_connection())
            poll_slip_socket(&socket);

        debug("cdc disconnected\n");
    }
}
