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

#ifndef SHP_H
#define SHP_H

#include <types.h>

#ifndef SHP_CHUNK_SIZE
#define SHP_CHUNK_SIZE 64
#endif

#ifndef SHP_DATA_SIZE
#define SHP_DATA_SIZE 32
#endif

struct shp_socket;
enum shp_state;

typedef void (* shp_handler_t)(struct shp_socket *socket, const void *data, u32_t size);
typedef u32_t (* shp_read_t)(void *data, u32_t size);
typedef u32_t (* shp_write_t)(const void *data, u32_t size);

enum shp_state
{
    SHP_IDLE,
    SHP_EVEN_DIGIT,
    SHP_ODD_DIGIT,
    SHP_CARRIAGE_RETURN
};

struct shp_chunk
{
    u8_t data[SHP_CHUNK_SIZE];
    u8_t *head;
    u8_t *tail;
};

struct shp_socket
{
    shp_handler_t handler;
    shp_read_t read;
    shp_write_t write;
    struct shp_chunk read_chunk;
    struct shp_chunk write_chunk;
    enum shp_state state;
    u8_t data[SHP_DATA_SIZE];
    u32_t size;
};

void bind_shp_socket(struct shp_socket *socket, shp_handler_t handler, shp_read_t read, shp_write_t write);
void poll_shp_socket(struct shp_socket *socket);

void send_shp_response(struct shp_socket *socket, const void *data, u32_t size);

#endif
