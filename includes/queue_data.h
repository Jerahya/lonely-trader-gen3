#ifndef QUEUEDATA_H
#define QUEUEDATA_H

#include "defines.h"

#define QUEUE_SIZE 10

typedef struct {
    uint16_t delay;
    uint8_t size;
    uint8_t pos;
    uint8_t offset;
    uint16_t command;
    uint16_t *data;
    bool sending;
    void (*callBack)(void);
} QueueData;


typedef struct {
    QueueData buffer[QUEUE_SIZE];
    size_t head;
    size_t tail;
    size_t count;
} Queue;

Queue queue_initialize();
QueueData queue_create(uint16_t *data, uint16_t command, uint8_t size, uint16_t delay, void (*callBack)(void));
bool queue_enqueue(Queue *q, QueueData item);
bool queue_dequeue(Queue *q, QueueData *item);
bool queue_not_empty(const Queue *q);
bool queue_is_emplty(const Queue *q);
bool queue_full(const Queue *q);

#endif