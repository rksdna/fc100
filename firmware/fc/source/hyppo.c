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

#include "hyppo.h"

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

static void reset(struct hyppo_socket *socket)
{
    socket->state = IDLE_STATE;
}

static void begin(struct hyppo_socket *socket)
{
    socket->state = DIGIT_0_STATE;
    socket->size = 0;
}

static void complete(struct hyppo_socket *socket)
{
    if (socket->state == DIGIT_0_STATE)
    {
        debug("> %*m\n", socket->size, socket->buffer);
    }

    reset(socket);
}

static void consume(struct hyppo_socket *socket, u8_t value)
{
    if (socket->size < sizeof(socket->buffer))
    {
        if (socket->state == DIGIT_0_STATE)
        {
            socket->buffer[socket->size] = value << 4;
            socket->state = DIGIT_1_STATE;
            return;
        }

        if (socket->state == DIGIT_1_STATE)
        {
            socket->buffer[socket->size++] += value;
            socket->state = DIGIT_0_STATE;
            return;
        }
    }

    reset(socket);
}

void poll_hyppo(struct hyppo_socket *socket)
{
    char buffer[2];
    const char *ptr = buffer;
    u32_t size = socket->read(buffer, sizeof(buffer));
    while (size--)
    {
        const char ch = *ptr++;
        switch (ch)
        {
        case '\r':
            complete(socket);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            consume(socket, ch - '0');
            break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            consume(socket, ch - 'A' + 10);
            break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            consume(socket, ch - 'a' + 10);
            break;

        case ':':
            begin(socket);
            break;

        default:
            reset(socket);
            break;
        }
    }
}
