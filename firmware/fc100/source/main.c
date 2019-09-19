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

/*#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <cdc.h>
#include "threads.h"
#include "counter.h"
#include "device.h"
#include "tools.h"
#include "shp.h"

enum
{
    IDLE_STATE,
    READY_STATE,
    TRIGGER_STATE,
    BUSY_1_STATE,
    BUSY_2_STATE,
    BUSY_3_STATE,
    CALIBRATION_1_STATE,
    CALIBRATION_2_STATE
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
    u8_t threshold1;
    u8_t threshold2;
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
    u32_t duration;
    u32_t burst;

    u8_t dac1;
    u8_t dac2;
    u8_t coupling1;
    u8_t coupling2;
    u8_t start_edge;
    u8_t stop_edge;
    u8_t counter_edge;
    u8_t timer_clock;

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

static u32_t update(u32_t state, u8_t mode, u8_t ctrl)
{
    regs.dac1 = 255 - params.dac1;
    regs.dac2 = 255 - params.dac2;

    regs.ctrl = ctrl |
            ((params.coupling1 << 4) & COUNTER_CTRL_HPF_CH1) |
            ((params.coupling2 << 5) & COUNTER_CTRL_HPF_CH2);

    regs.mode = mode |
            ((params.start_edge) & COUNTER_MODE_STRT) |
            ((params.stop_edge << 2) & COUNTER_MODE_STOP) |
            ((params.counter_edge << 4) & COUNTER_MODE_CNT) |
            ((params.timer_clock << 6) & COUNTER_MODE_TMR);

    return state;
}

static u32_t process_counter(u32_t state, u32_t elapsed)
{
    const u8_t ctrl = params.burst ? COUNTER_CTRL_STRT : COUNTER_CTRL_STRT | COUNTER_CTRL_STOP;

    const u32_t is_started = regs.ack & COUNTER_ACK_STRT;
    const u32_t is_stopped = regs.ack & COUNTER_ACK_STOP;
    const u32_t is_ready = is_started && is_stopped;

    switch (state)
    {
    case IDLE_STATE:
    case READY_STATE:
        return update(state, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

    case TRIGGER_STATE:
        return update(BUSY_1_STATE, COUNTER_MODE_CLR, ctrl);

    case BUSY_1_STATE:
        if (elapsed >= params.duration)
            return update(IDLE_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

        if (!is_started)
            return update(BUSY_1_STATE, 0, ctrl);

        return update(BUSY_2_STATE, 0, ctrl | COUNTER_CTRL_TEST);

    case BUSY_2_STATE:
        if (elapsed < params.duration && !is_ready)
            return update(BUSY_2_STATE, 0, ctrl | COUNTER_CTRL_TEST);

        return update(BUSY_3_STATE, 0, COUNTER_CTRL_STRT | COUNTER_CTRL_STOP | COUNTER_CTRL_TEST);

    case BUSY_3_STATE:
        if (elapsed >= params.duration)
            return update(IDLE_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

        if (!is_ready)
            return update(BUSY_3_STATE, 0, COUNTER_CTRL_STRT | COUNTER_CTRL_STOP | COUNTER_CTRL_TEST);

        params.counter = regs.cnt;
        params.timer = regs.tmr;
        params.start = regs.tac_strt;
        params.stop = regs.tac_stop;
        return update(CALIBRATION_1_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLB_ZS);

    case CALIBRATION_1_STATE:
        if (!is_ready)
            return update(IDLE_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

        params.start_p0 = regs.tac_strt;
        params.stop_p0 = regs.tac_stop;
        return update(CALIBRATION_2_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLB_FS);

    case CALIBRATION_2_STATE:
        if (!is_ready)
            return update(IDLE_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

        params.start_p1 = regs.tac_strt;
        params.stop_p1 = regs.tac_stop;
        return update(READY_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);

    default:
        break;
    }

    return update(IDLE_STATE, COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
}

static void control_handler(void *arg)
{
    WASTE(arg);
    struct timer tm;
    start_timer(&tm, -1);

    u32_t time = 0;
    while (1)
    {
        const u32_t state = process_counter(params.state, tm.ticks - time);
        if (state != params.state)
        {
            time = tm.ticks;
            params.state = state;
        }

        write_device_counter(COUNTER_DAC1, &regs.dac1, 4);
        read_device_counter(COUNTER_ID, &regs.id, 12);
    }

    stop_timer(&tm);
}

static void socket_handler(struct shp_socket *socket, const void *data, u32_t size)
{
    if (size == sizeof(struct shp_short_request))
    {
        const struct shp_short_request * const request = (const struct shp_short_request *)data;
        params.dac1 = request->threshold1;
        params.dac2 = request->threshold2;
        params.coupling1 = request->coupling1;
        params.coupling2 = request->coupling2;
    }
    else if (size == sizeof(struct shp_long_request))
    {
        const struct shp_long_request * const request = (const struct shp_long_request *)data;
        params.state = TRIGGER_STATE;
        params.burst = request->burst;
        params.duration = request->duration;

        params.dac1 = request->threshold1;
        params.dac2 = request->threshold2;
        params.coupling1 = request->coupling1;
        params.coupling2 = request->coupling2;

        params.start_edge =request->start_edge;
        params.stop_edge = request->stop_edge;
        params.counter_edge = request->counter_edge;
        params.timer_clock = request->timer_clock;
    }
    else
    {
        return;
    }

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
}*/

/*#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <cdc.h>
#include "device.h"
#include "shp.h"

static void socket_handler(struct shp_socket *socket, const void *data, u32_t size)
{
    const u8_t response[12];
    write_device_counter(0, data, size);
    read_device_counter(size, response, sizeof(response));
    send_shp_response(socket, &response, sizeof(response));
}

void main(void)
{
    startup_device();
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
}*/

#include <threads.h>
#include <timers.h>
#include <debug.h>
#include <tools.h>
#include <cdc.h>
#include "device.h"
#include "counter.h"
#include "shp.h"

#define V_ON 475
#define V_OFF 465

enum state
{
    IDLE_STATE,
    START_TIME_STATE,
    START_FREQUENCY_STATE,
    START_DUTY_STATE,
    START_GATED_FREQUENCY_STATE,
    Ps2State,
    Ps3State,
    Ps4State,
    BUSY_1_STATE,
    BUSY_2_STATE,
    BUSY_3_STATE,
    CALIBRATION_1_STATE,
    CALIBRATION_2_STATE,
};

struct
{
    u8_t threshold1;
    u8_t threshold2;
    u8_t coupling1;
    u8_t coupling2;
    u8_t ref_source;
    u8_t start_event;
    u8_t stop_event;
    u8_t count_event;
    u32_t duration;
} params;

struct output
{
    u32_t counter;
    u32_t timer;
} out;

u32_t duration;


char tx[256];
u32_t tx_size;

static void put_cdc(void *data, char value)
{
    if (tx_size < sizeof(tx))
        tx[tx_size++] = value;

    if (value == '\n')
    {
        if (has_cdc_connection())
            write_cdc_data(tx, tx_size);

        tx_size = 0;
    }
}

struct stream cdc_stream = {put_cdc, 0};

static enum state control(enum state state, u32_t elapsed, struct counter *regs)
{
    switch (state)
    {
    case IDLE_STATE:
        regs->dac1 = 255 - params.threshold1;
        regs->dac2 = 255 - params.threshold2;
        regs->mode =
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

        return Ps2State;

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
                COUNTER_CTRL_STRT;

        return BUSY_1_STATE;

    case Ps2State:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STRT)
        {
            regs->ctrl |= COUNTER_CTRL_TEST;
            return Ps3State;
        }

        if (elapsed > duration)
            return IDLE_STATE;

        break;

    case Ps3State:
        if (regs->ack & COUNTER_ACK_STOP)
        {
            print(&cdc_stream, "B %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);

            debug("B %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);

            regs->mode ^= COUNTER_MODE_STOP_0 | COUNTER_MODE_CNT_0;
            regs->mode |= COUNTER_MODE_CLR;
            return BUSY_1_STATE;
        }

        if (elapsed > duration)
        {

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
            print(&cdc_stream, "R %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);

            debug("R %d, %d, %d, %d\n", regs->tac_strt, regs->tac_stop, regs->cnt, regs->tmr);

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~(COUNTER_CTRL_STRT | COUNTER_CTRL_STOP);
            regs->ctrl |= COUNTER_CTRL_CLB_FS;
            return CALIBRATION_1_STATE;
        }

        if (elapsed > duration)
        {
            return IDLE_STATE;
        }

        break;

    case CALIBRATION_1_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STOP)
        {
            print(&cdc_stream, "FS %d, %d\n", regs->tac_strt, regs->tac_stop);

            debug("FS %d, %d\n", regs->tac_strt, regs->tac_stop);

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~COUNTER_CTRL_CLB_FS;
            regs->ctrl |= COUNTER_CTRL_CLB_ZS;
            return CALIBRATION_2_STATE;
        }

        break;

    case CALIBRATION_2_STATE:
        regs->mode &= ~COUNTER_MODE_CLR;
        if (regs->ack & COUNTER_ACK_STOP)
        {
            print(&cdc_stream, "ZS %d, %d\n", regs->tac_strt, regs->tac_stop);


            debug("ZS %d, %d\n", regs->tac_strt, regs->tac_stop);

            regs->mode |= COUNTER_MODE_CLR;
            regs->ctrl &= ~(COUNTER_CTRL_CLB_ZS | COUNTER_CTRL_TEST);
            return START_DUTY_STATE;
        }

        break;

    default:
        break;
    }

    return state;
}

static void control_handler(void *arg)
{
    WASTE(arg);
    enum state state = START_DUTY_STATE;
    struct timer tm;
    struct counter regs;

    sleep(250);
    if (get_device_voltage() > V_ON)
    {
        debug("control started\n");
        startup_device_counter(250);
        start_timer(&tm, -1);
        while (get_device_voltage() > V_OFF)
        {
            const enum state next = control(state, tm.ticks, &regs);
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
        debug("control stopped\n");
    }
}

static void socket_handler(struct shp_socket *socket, const void *data, u32_t size)
{
    const u8_t response[12];
    write_device_counter(0, data, size);
    read_device_counter(size, response, sizeof(response));
    send_shp_response(socket, &response, sizeof(response));
}


char *test = "a=332.2,b=qwerty";

char gett(void *data)
{
    return *test++;
}

struct source src = {gett, 0};

void main(void)
{
    static struct thread control_thread;
    static u8_t control_stack[256];

    s32_t x;
    char y[256];

    static char rx[256];
    static char tx[256];

    params.threshold1 = 128;
    params.threshold2 = 128;
    params.duration = 500;

    startup_device();


    u32_t r = scan(&src, "a=%1.5d,b=%s", &x, y);
    debug("%d, %d, %s", r, x, y);



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
        {
            poll_shp_socket(&socket);
        }

        debug("cdc disconnected\n");
    }
}

