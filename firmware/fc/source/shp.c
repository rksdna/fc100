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

#include "debug.h"
#include "shp.h"

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

static void clear_chunk(struct shp_chunk *chunk)
{
    chunk->head = chunk->data;
    chunk->tail = chunk->data;
}

static void read_socket_chunk(struct shp_socket *socket)
{
    while (socket->read_chunk.head == socket->read_chunk.tail)
    {
        clear_chunk(&socket->read_chunk);
        socket->read_chunk.tail += socket->read(socket->read_chunk.head, sizeof(socket->read_chunk.data));
    }
}

static void write_socket_chunk(struct shp_socket *socket)
{
    while (socket->write_chunk.head != socket->write_chunk.tail)
        socket->write_chunk.head += socket->write(socket->write_chunk.head, socket->write_chunk.tail - socket->write_chunk.head);

    clear_chunk(&socket->write_chunk);
}

static u8_t get_socket_byte(struct shp_socket *socket)
{
    read_socket_chunk(socket);
    return *socket->read_chunk.head++;
}

static void put_socket_byte(struct shp_socket *socket, u8_t value)
{
    if (socket->write_chunk.tail >= socket->write_chunk.data + sizeof(socket->write_chunk.data))
        write_socket_chunk(socket);

    *socket->write_chunk.tail++ = value;
}

u8_t shp_checksum(const void *data, u32_t size)
{
    // CRC-8
    // Poly 0x07
    // Init 0x00
    // RefIn false
    // RefOut false
    // Xor 0x00

    static const u8_t table[] =
    {
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
        0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
        0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
        0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
        0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
        0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
        0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
        0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
        0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
        0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
        0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
        0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
        0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
        0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
        0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
        0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
        0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
        0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
        0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
        0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
    };

    const u8_t *ptr = (const u8_t *)data;
    u8_t crc = 0x00;
    while (size--)
        crc = table[crc ^ *ptr++];

    return crc;

    /*const u8_t *ptr = (const u8_t *)data;
    u8_t lcr = 0;
    while (size--)
        lcr += *ptr++;

    return -lcr;*/
}

static void begin_message(struct shp_socket *socket)
{
    socket->state = SHP_EVEN_DIGIT;
    socket->size = 0;
}

static void process_message(struct shp_socket *socket, u8_t value)
{
    if (socket->size < sizeof(socket->data))
    {
        if (socket->state == SHP_EVEN_DIGIT)
        {
            socket->data[socket->size] = value << 4;
            socket->state = SHP_ODD_DIGIT;
            return;
        }

        if (socket->state == SHP_ODD_DIGIT)
        {
            socket->data[socket->size++] += value;
            socket->state = SHP_EVEN_DIGIT;
            return;
        }
    }

    socket->state = SHP_IDLE;
}

static void complete_message(struct shp_socket *socket)
{
    if (socket->state == SHP_EVEN_DIGIT)
    {
        if (socket->size > 1 && shp_checksum(socket->data, socket->size) == 0)
            socket->handler(socket, socket->data, socket->size - 1);
    }

    socket->state = SHP_IDLE;
}

static void drop_message(struct shp_socket *socket)
{
    socket->state = SHP_IDLE;
}

void bind_shp_socket(struct shp_socket *socket, shp_handler_t handler, shp_read_t read, shp_write_t write)
{
    socket->size = 0;
    socket->handler = handler;
    socket->read = read;
    socket->write = write;
    clear_chunk(&socket->read_chunk);
    clear_chunk(&socket->write_chunk);
    socket->state = SHP_IDLE;
}

void poll_shp_socket(struct shp_socket *socket)
{
    const char ch = get_socket_byte(socket);
    switch (ch)
    {
    case '\r':
        complete_message(socket);
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
        process_message(socket, ch - '0');
        break;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        process_message(socket, ch - 'A' + 10);
        break;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
        process_message(socket, ch - 'a' + 10);
        break;

    case ':':
        begin_message(socket);
        break;

    default:
        drop_message(socket);
        break;
    }
}

void send_shp_response(struct shp_socket *socket, const void *data, u32_t size)
{
    static const char digits[] = "0123456789ABCDEF";
    const u8_t *ptr = (const u8_t *)data;
    const u8_t checksum = shp_checksum(data, size);

    put_socket_byte(socket, ':');

    while (size--)
    {
        const u8_t value = *ptr++;
        put_socket_byte(socket, digits[(value >> 4) & 0x0F]);
        put_socket_byte(socket, digits[value & 0x0F]);
    }

    put_socket_byte(socket, digits[(checksum >> 4) & 0x0F]);
    put_socket_byte(socket, digits[checksum & 0x0F]);

    put_socket_byte(socket, '\r');

    write_socket_chunk(socket);
}
