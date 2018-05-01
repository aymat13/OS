#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue{
  int *memory;
  int max_size;
  int current_size;
  int front;
  int rear;
}queue_t;

void QueueInitialize(queue_t *main_queue, int size);
int QueueInsert(queue_t *main_queue, int number);
int QueueRemove(queue_t *main_queue);
void QueueDestroy(queue_t *main_queue);

#endif
