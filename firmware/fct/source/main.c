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
#define CHECK_VOLTAGE if (voltage <= V_OFF) { shutdown_device_counter(); return FAILURE_STATE; }
#define CHECK_ID if (regs.id != 0xAA) { shutdown_device_counter(); return FAILURE_STATE; }

enum counter_state
{
    SHUTDOWN_STATE,
    PREPARE_STATE,
    IDLE_STATE,
    ZS_CALIBRATION_STATE,
    FS_CALIBRATION_STATE,
    START_STATE,
    MEASURE_STATE,
    STOP_STATE,
    FAILURE_STATE
};

enum counter_coupling
{
    DC_COUPLING,
    AC_COUPLING
};

enum counter_edge
{
    CH1_RISING_EDGE,
    CH1_FALLING_EDGE,
    CH2_RISING_EDGE,
    CH2_FALLING_EDGE
};

enum counter_clock
{
    INTERNAL_CLOCK,
    EXTERNAL_CLOCK
};

struct shp_request
{
    u32_t command;
    u32_t duration;
    s8_t dac1;
    s8_t dac2;
    u8_t ch1_coupling;
    u8_t ch2_coupling;
    u8_t start_edge;
    u8_t stop_edge;
    u8_t counter_edge;
    u8_t timer_clock;
};

struct shp_response
{
    u32_t status;
    u32_t counter;
    u32_t timer;
    u8_t tac_start;
    u8_t tac_stop;
    u8_t ref_start[2];
    u8_t ref_stop[2];
    u8_t padding[2];
};

struct counter regs;
struct shp_request request;
struct shp_response response;

u32_t duration = 0;

static void poll_counter(void)
{
    write_device_counter(0, &regs, 4);
    read_device_counter(0, &regs, 16);
}

static void control_counter(u8_t mode, u8_t ctrl)
{
    regs.dac1 = request.dac1 ^ 0x80;
    regs.dac2 = request.dac2 ^ 0x80;
    regs.mode =
            ((request.start_edge) & COUNTER_MODE_STRT) |
            ((request.stop_edge << 2) & COUNTER_MODE_STOP) |
            ((request.counter_edge << 4) & COUNTER_MODE_CNT) |
            ((request.timer_clock << 6) & COUNTER_MODE_TMR) |
            mode;
    regs.ctrl =
            ((request.ch1_coupling << 4) & COUNTER_MODE_STOP) |
            ((request.ch2_coupling << 5) & COUNTER_MODE_CNT) |
            ctrl;
}

enum counter_state process(enum counter_state state, u32_t elapsed)
{
    const s32_t voltage = get_device_voltage();

    switch (state)
    {
    case SHUTDOWN_STATE:
        if (voltage >= V_ON)
        {
            prepare_device_counter();
            return PREPARE_STATE;
        }

        break;

    case PREPARE_STATE:
        CHECK_VOLTAGE;
        if (elapsed > 25)
        {
            startup_device_counter();
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
            return IDLE_STATE;
        }

        break;

    case IDLE_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;
        if (elapsed > 500)
        {
            debug("begin\n");
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLB_ZS | COUNTER_CTRL_TEST);
            return ZS_CALIBRATION_STATE;
        }

        break;

    case ZS_CALIBRATION_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;
        if (regs.ack & COUNTER_ACK_STOP)
        {
            debug("zs %d %d\n", regs.tac_stop, regs.tac_strt);
            response.ref_start[0] = regs.tac_strt;
            response.ref_stop[0] = regs.tac_stop;
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLB_FS | COUNTER_CTRL_TEST);
            return FS_CALIBRATION_STATE;
        }

        break;

    case FS_CALIBRATION_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;
        if (regs.ack & COUNTER_ACK_STOP)
        {
            debug("fs %d %d\n", regs.tac_stop, regs.tac_strt);
            response.ref_start[1] = regs.tac_strt;
            response.ref_stop[1] = regs.tac_stop;
            control_counter(COUNTER_MODE_CLR, duration ? COUNTER_CTRL_STRT | COUNTER_CTRL_TEST : COUNTER_CTRL_STRT | COUNTER_CTRL_STOP | COUNTER_CTRL_TEST);
            return START_STATE;
        }

        break;

    case START_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;
        if (regs.ack & COUNTER_ACK_STRT)
        {
            debug("started\n");

            return MEASURE_STATE;
        }

        if (elapsed >= duration)
        {
            debug("timeout 1\n");
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
            return IDLE_STATE;
        }

        break;

    case MEASURE_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;

        if (elapsed >= duration)
        {
            control_counter(0, COUNTER_CTRL_STRT | COUNTER_CTRL_STOP | COUNTER_CTRL_TEST);
            return STOP_STATE;
        }

        break;

    case STOP_STATE:
        CHECK_VOLTAGE;
        poll_counter();
        CHECK_ID;

        if (regs.ack & COUNTER_ACK_STOP)
        {
            debug("stopped %d %d\n", regs.cnt, regs.tmr);
            response.counter = regs.cnt;
            response.timer = regs.tmr;
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
            return IDLE_STATE;
        }

        if (elapsed >= duration)
        {
            debug("timeout 2\n");
            control_counter(COUNTER_MODE_CLR, COUNTER_CTRL_CLR);
            return IDLE_STATE;
        }

        break;

    case FAILURE_STATE:
    default:
        break;
    }

    return state;
}

static void control(void *arg)
{
    WASTE(arg);
    u32_t elapsed = 0;
    enum counter_state state = SHUTDOWN_STATE;
    struct timer tm;
    start_timer(&tm, TIMER_PERIOD);
    while (1)
    {
        const enum counter_state next = process(state, elapsed);
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
    if (size == sizeof(struct shp_request))
    {
        copy(&request, data, size);
        send_shp_response(socket, &response, sizeof(struct shp_response));
    }
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
