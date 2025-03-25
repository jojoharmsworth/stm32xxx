#include "ringbuffer.h"

RingBuffer *ring_buffer_init(int size)
{
    RingBuffer *rb = (RingBuffer *)malloc(sizeof(RingBuffer));
    rb->buffer     = (uint8_t *)malloc(size * sizeof(uint8_t));
    rb->max        = size;
    rb->head       = 0;
    rb->tail       = 0;
    rb->full       = false;
    return rb;
}

void ring_buffer_free(RingBuffer *rb)
{
    free(rb->buffer);
    free(rb);
}

bool ring_buffer_full(RingBuffer *rb)
{
    return rb->full;
}

bool ring_buffer_empty(RingBuffer *rb)
{
    return (!rb->full && (rb->head == rb->tail));
}

void ring_buffer_reset(RingBuffer *rb)
{
    rb->head = rb->tail;
    rb->full = false;
}

int ring_buffer_size(RingBuffer *rb)
{
    if(rb->full)
    {
        return rb->max;
    }
    if(rb->head >= rb->tail)
    {
        return rb->head - rb->tail;
    }
    return rb->max + rb->head - rb->tail;
}

void ring_buffer_push(RingBuffer *rb, uint8_t data)
{
    rb->buffer[rb->head] = data;
    if(rb->full)
    {
        rb->tail = (rb->tail + 1) % rb->max;
    }
    rb->head = (rb->head + 1) % rb->max;
    rb->full = (rb->head == rb->tail);
}

uint8_t ring_buffer_pop(RingBuffer *rb)
{
    if(ring_buffer_empty(rb))
    {
        return 0;
    }
    uint8_t data = rb->buffer[rb->tail];
    rb->full  = false;
    rb->tail  = (rb->tail + 1) % rb->max;
    return data;
}
