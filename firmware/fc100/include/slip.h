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

#ifndef SLIP_H
#define SLIP_H

#include <types.h>

#ifndef SLIP_CHUNK_SIZE
#define SLIP_CHUNK_SIZE 32
#endif

#ifndef SLIP_DATA_SIZE
#define SLIP_DATA_SIZE 32
#endif

struct slip_socket;

typedef void (* slip_handler_t)(struct slip_socket *socket, const void *data, u32_t size);
typedef u32_t (* slip_read_t)(void *data, u32_t size);
typedef u32_t (* slip_write_t)(const void *data, u32_t size);

struct slip_chunk
{
    u8_t data[SLIP_CHUNK_SIZE];
    u8_t *head;
    u8_t *tail;
};

struct slip_socket
{
    slip_handler_t handler;
    slip_read_t read;
    slip_write_t write;

    struct slip_chunk read_chunk;
    struct slip_chunk write_chunk;

    u32_t size;
    u8_t data[SLIP_DATA_SIZE];
};

void bind_slip_socket(struct slip_socket *socket, slip_handler_t handler, slip_read_t read, slip_write_t write);
void poll_slip_socket(struct slip_socket *socket);

void send_slip_response(struct slip_socket *socket, const void *data, u32_t size);

#endif
