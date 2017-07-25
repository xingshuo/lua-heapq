#ifndef _HEAPQ_H
#define _HEAPQ_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    int session;
    double priority;
} HeapNode;

typedef struct {
    size_t size;
    size_t capacity;
    HeapNode** data;
} HeapQueue;

HeapQueue* create_queue(size_t capacity);
void reset_queue(HeapQueue*);
void delete_queue(HeapQueue*);
size_t queue_size(HeapQueue*);
bool queue_push(HeapQueue*, HeapNode*);
HeapNode* queue_pop(HeapQueue*);
HeapNode* queue_pushpop(HeapQueue*, HeapNode*);
HeapNode* queue_cmpltepop(HeapQueue*, double);
HeapNode* queue_top(HeapQueue*);

#endif