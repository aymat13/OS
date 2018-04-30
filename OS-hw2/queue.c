#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void QueueInitialize(queue_t *main_queue, int size) {
  main_queue->memory = malloc(size);
  main_queue->max_size = size;
  main_queue->front = 0;
  main_queue->current_size = 0;
}

void QueueInsert(queue_t *main_queue, int number) {
  if(main_queue->current_size != main_queue->max_size) {
    *(main_queue->memory + (main_queue->current_size) - 1) = number;
    (main_queue->current_size)++;
  } else {
    printf("Error: Inserting to a FULL Queue!\n");
  }
}

int QueueRemove(queue_t *main_queue) {
  if(main_queue->current_size != 0) {
    int temp;
    temp = *(main_queue->memory+(main_queue->front)-1);
    (main_queue->front)++;
    (main_queue->current_size)--;
    return temp;
  } else {
    printf("Error: Deleting from an EMPTY Queue\n");
    return 0;
  }
}

void QueueDestroy(queue_t *main_queue) {
  free(main_queue);
}
