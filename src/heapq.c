#include "heapq.h"

#define HEAPQ_MAX_CAP 0x7fffffff-1

static inline void
swap_elements(HeapQueue* q, int first, int second) {
  void* temp = q->data[first];
  q->data[first] = q->data[second];
  q->data[second] = temp;
}

static inline int
cmp_elements(HeapNode* a, HeapNode* b) {
    if (a->priority == b->priority) {
        return 0;
    }else {
        return (a->priority < b->priority)?-1:1;
    }
}

static void
shift_up(HeapQueue* q, int hole, HeapNode* n) {
    int up_hole = hole/2;
    while (up_hole >= 1 && cmp_elements(n,q->data[up_hole]) < 0) {
        swap_elements(q, hole, up_hole);
        hole = up_hole;
        up_hole = hole/2;
    }
    q->data[hole] = n;
}

static void
shift_down(HeapQueue* q, int hole) {
    int down_hole = hole*2;
    while (down_hole <= q->size) {
        if (down_hole < q->size && cmp_elements(q->data[down_hole+1],q->data[down_hole]) < 0) {
            down_hole++;
        }
        if (cmp_elements(q->data[hole],q->data[down_hole]) >= 0) {
            swap_elements(q, hole, down_hole);
            hole = down_hole;
            down_hole = hole*2;
        }else{
            break;
        }
    }
}


HeapQueue*
create_queue(size_t capacity) {
    assert(capacity > 0);
    HeapQueue* q = malloc(sizeof(*q));
    q->data = calloc(capacity+1, sizeof(*(q->data)));
    q->capacity = capacity;
    q->size = 0;
    return q;
}

void
reset_queue(HeapQueue* q) {
    if(!q) {
        return;
    }
    int i;
    for (i=1; i<=q->size; i++) {
        if (q->data[i]) {
            free(q->data[i]);
        }
    }
    q->size = 0;
}

void
delete_queue(HeapQueue* q) {
    if (!q) {
        return;
    }
    int i;
    for (i=1; i<=q->size; i++) {
        if (q->data[i]) {
            free(q->data[i]);
        }
    }
    free(q->data);
    free(q);
}

size_t
queue_size(HeapQueue* q) {
    if (!q) {
        return 0;
    }
    return q->size;
}

bool
queue_push(HeapQueue* q, HeapNode* n) {
    if(!q) {
        return false;
    }
    if (q->size >= q->capacity) {
        size_t old_cap = q->capacity;
        if (2*old_cap > HEAPQ_MAX_CAP) {
            return false;
        }
        q->capacity = old_cap*2;
        HeapNode** newdata = calloc(q->capacity+1,sizeof(*(q->data)));
        int i;
        for (i=1; i<=old_cap; i++) {
            newdata[i] = q->data[i];
        }
        free(q->data);
        q->data = newdata;
    }
    int hole = ++(q->size);
    shift_up(q, hole, n);
    return true;
}

HeapNode*
queue_pop(HeapQueue* q) {
    if(!q) {
        return NULL;
    }
    if (q->size <= 0) {
        return NULL;
    }
    HeapNode* top = q->data[1];
    q->data[1] = q->data[q->size--];
    shift_down(q, 1);
    return top;
}

HeapNode*
queue_pushpop(HeapQueue* q, HeapNode* n) {
    if(!q) {
        return NULL;
    }
    if (q->size <= 0) {
        return n;
    }
    HeapNode* top = q->data[1];
    if (cmp_elements(n, top) <= 0) {
        return n;
    }
    q->data[1] = n;
    shift_down(q, 1);
    return top;
}

HeapNode*
queue_cmpltepop(HeapQueue* q, double cmp_val) {
    if(!q) {
        return NULL;
    }
    if (q->size <= 0) {
        return NULL;
    }
    HeapNode* top = q->data[1];
    if (top->priority > cmp_val) {
        return NULL;
    }
    q->data[1] = q->data[q->size--];
    shift_down(q, 1);
    return top;
}

HeapNode*
queue_top(HeapQueue* q) {
    if(!q) {
        return NULL;
    }
    if (q->size <= 0) {
        return NULL;
    }
    return q->data[1];
}