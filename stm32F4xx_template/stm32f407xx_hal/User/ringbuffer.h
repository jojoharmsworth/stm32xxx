#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t *buffer;    // 缓冲区
    int      head;      // 头指针
    int      tail;      // 尾指针
    int      max;       // 最大长度
    bool     full;      // 是否满
} RingBuffer;

RingBuffer *ring_buffer_init(int size);
void        ring_buffer_free(RingBuffer *rb);
bool        ring_buffer_full(RingBuffer *rb);
bool        ring_buffer_empty(RingBuffer *rb);
int         ring_buffer_size(RingBuffer *rb);
void        ring_buffer_reset(RingBuffer *rb);
void        ring_buffer_push(RingBuffer *rb, uint8_t data);
uint8_t     ring_buffer_pop(RingBuffer *rb);

#endif
