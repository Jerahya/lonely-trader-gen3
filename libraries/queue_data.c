#include "pico/stdlib.h"
#include "queue_data.h"

Queue queue_initialize() {
    Queue q;
    q.head      = 0;
    q.tail      = 0;
    q.count     = 0;
    return q;
}

QueueData queue_create(uint16_t *data, uint16_t command, uint8_t size, uint16_t delay, void (*callBack)(void)) {
    QueueData qd;
    qd.data     = data;
    qd.command  = command;
    qd.size     = size;
    qd.pos      = 0;
    qd.delay    = delay;
    qd.offset   = 0;
    qd.sending  = false;
    qd.callBack = callBack;
    return qd;
}

bool queue_enqueue(Queue *q, QueueData item) {
    if (q->count == QUEUE_SIZE) return false;

    q->buffer[q->tail] = item;
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count++;
    return true;
}

bool queue_dequeue(Queue *q, QueueData *item) {
    if (q->count == 0) return false;

    *item = q->buffer[q->head];
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count--;
    return true;
}

bool queue_not_empty(const Queue *q) {
    return q->count > 0;
}

bool queue_is_emplty(const Queue *q) {
    return q->count == 0;
}

bool queue_full(const Queue *q) {
    return q->count == QUEUE_SIZE;
}