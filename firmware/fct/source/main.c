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

#define V_ON 485
#define V_OFF 475

enum
{
    OFF_STATE,
    IDLE_STATE,
    READY_STATE,
    TRIGGER_STATE,
    TRIGGER_BURST_STATE,
    WAIT_START_STATE,
    MEASURE_STATE,
    WAIT_STOP_STATE,
    FETCH_RESULT,
    CALIBRATION_1_STATE,
    CALIBRATION_0_STATE
};

struct shp_short_request
{
    s8_t threshold1;
    s8_t threshold2;
    u8_t coupling1;
    u8_t coupling2;
};

struct shp_long_request
{
    s8_t threshold1;
    s8_t threshold2;
    u8_t coupling1;
    u8_t coupling2;

    u16_t burst;
    u16_t duration;
    u8_t counter_edge;
    u8_t timer_clock;
    u8_t start_edge;
    u8_t stop_edge;
};

struct shp_short_response
{
    u16_t state;
    u16_t voltage;
};

struct shp_long_response
{
    u16_t state;
    u16_t voltage;

    u32_t counter;
    u32_t timer;
    u8_t start_divident;
    u8_t start_divider;
    u8_t stop_divident;
    u8_t stop_divider;
};

struct counter_params
{
    u32_t state;

    s8_t threshold1;
    s8_t threshold2;
    u8_t coupling1;
    u8_t coupling2;

    u32_t duration;
    u8_t counter_edge;
    u8_t timer_clock;
    u8_t start_edge;
    u8_t stop_edge;

    u32_t counter;
    u32_t timer;
    u8_t start;
    u8_t start_p0;
    u8_t start_p1;
    u8_t stop;
    u8_t stop_p0;
    u8_t stop_p1;
};

static struct counter regs;
static struct counter_params params;

static void set_regs(u8_t mode, u8_t ctrl)
{
    regs.dac1 = params.threshold1 ^ 0x80;
    regs.dac2 = params.threshold2 ^ 0x80;
    regs.mode =
            ((params.start_edge) & COUNTER_MODE_STRT) |
            ((params.stop_edge << 2) & COUNTER_MODE_STOP) |
            ((params.counter_edge << 4) & COUNTER_MODE_CNT) |
            ((params.timer_clock << 6) & COUNTER_MODE_TMR) |
            mode;
    regs.ctrl =
            ((params.coupling1 << 4) &  COUNTER_CTRL_HPF_CH1) |
            ((params.coupling2 << 4) &  COUNTER_CTRL_HPF_CH2) |
            ctrl;
}

static u32_t process_counter(u32_t state, u32_t elapsed)
{
    switch (state)
    {
    case OFF_STATE:
        set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
        return IDLE_STATE;

    case IDLE_STATE:
    case READY_STATE:
        set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
        break;

    case TRIGGER_STATE:
        set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_STRT | COUNTER_CTRL_STOP);
        return WAIT_START_STATE;

    case TRIGGER_BURST_STATE:
        set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_STRT);
        return WAIT_START_STATE;

    case WAIT_START_STATE:
        if (regs.ack & COUNTER_ACK_STRT)
        {
            regs.ctrl |= COUNTER_CTRL_TEST;
            return MEASURE_STATE;
        }

        if (elapsed >= params.duration)
            return IDLE_STATE;

        break;

    case MEASURE_STATE:
        if (elapsed >= params.duration)
        {
            regs.ctrl |= COUNTER_CTRL_STOP;
            return WAIT_STOP_STATE;
        }

        break;

    case WAIT_STOP_STATE:
        if (regs.ack & COUNTER_ACK_STOP)
            return FETCH_RESULT;

        if (elapsed >= params.duration)
            return IDLE_STATE;

        break;

    case FETCH_RESULT:
        params.counter = regs.cnt;
        params.timer = regs.tmr;
        params.start = regs.tac_strt;
        params.stop = regs.tac_stop;
        set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_CLB_FS);
        return CALIBRATION_1_STATE;

    case CALIBRATION_1_STATE:
        if ((regs.ack & COUNTER_ACK_STRT) && (regs.ack & COUNTER_ACK_STOP))
        {
            params.start_p1 = regs.tac_strt;
            params.stop_p1 = regs.tac_stop;

            set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_CLB_ZS);
            return CALIBRATION_0_STATE;
        }

        if (elapsed)
            return IDLE_STATE;

        break;

    case CALIBRATION_0_STATE:
        if ((regs.ack & COUNTER_ACK_STRT) && (regs.ack & COUNTER_ACK_STOP))
        {
            params.start_p0 = regs.tac_strt;
            params.stop_p0 = regs.tac_stop;

            debug(" %d \n", regs.tac_stop);

            set_regs(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
            return READY_STATE;
        }

        if (elapsed)
            return IDLE_STATE;

        break;

    default:
        break;
    }

    return state;
}

static void control_handler(void *arg)
{
    WASTE(arg);

    while (get_device_voltage() < V_ON)
        sleep(5);

    prepare_device_counter();
    sleep(5);

    if (get_device_voltage() > V_OFF)
    {
        debug("counter started\n");
        startup_device_counter();

        struct timer tm;
        start_timer(&tm, -1);
        do
        {
            const u32_t state = process_counter(params.state, tm.ticks);
            if (state != params.state)
            {
                params.state = state;
                tm.ticks = 0;
            }

            write_device_counter(0, &regs, 4);
            read_device_counter(0, &regs, 16);
        }
        while (get_device_voltage() > V_OFF && regs.id == 0xAA);
    }

    debug("counter stopped\n");
    shutdown_device_counter();
    params.state = OFF_STATE;
}

static void socket_handler(struct shp_socket *socket, const void *data, u32_t size)
{
    if (size == sizeof(struct shp_short_request))
    {
        const struct shp_short_request * const request = (const struct shp_short_request *)data;
        params.threshold1 = request->threshold1;
        params.threshold2 = request->threshold2;
        params.coupling1 = request->coupling1;
        params.coupling2 = request->coupling2;
    }
    else if (size == sizeof(struct shp_long_request))
    {
        const struct shp_long_request * const request = (const struct shp_long_request *)data;
        params.threshold1 = request->threshold1;
        params.threshold2 = request->threshold2;
        params.coupling1 = request->coupling1;
        params.coupling2 = request->coupling2;
        params.duration = request->duration;
        params.state = request->burst ? TRIGGER_BURST_STATE : TRIGGER_STATE;
        params.counter_edge = request->counter_edge;
        params.timer_clock = request->timer_clock;
        params.start_edge = request->start_edge;
        params.stop_edge = request->stop_edge;
    }
    else
        return;

    if (params.state == READY_STATE)
    {
        struct shp_long_response response;
        response.state = params.state;
        response.voltage = get_device_voltage();

        response.counter = params.counter;
        response.timer = params.timer;
        response.start_divident = params.start - params.start_p0;
        response.start_divider = params.start_p1 - params.start_p0;
        response.stop_divident = params.stop - params.stop_p0;
        response.stop_divider = params.stop_p1 - params.stop_p0;

        send_shp_response(socket, &response, sizeof(struct shp_long_response));
    }
    else
    {
        struct shp_short_response response;
        response.state = params.state;
        response.voltage = get_device_voltage();

        send_shp_response(socket, &response, sizeof(struct shp_short_response));
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
        struct shp_socket socket;
        bind_shp_socket(&socket, socket_handler, read_cdc_data, write_cdc_data);

        yield_thread((condition_t)has_cdc_connection, 0);
        debug("cdc connected\n");
        while (has_cdc_connection())
            poll_shp_socket(&socket);

        debug("cdc disconnected\n");
    }
}
