#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void QueueInitialize(queue_t *main_queue, int size) {
  main_queue->memory = malloc(sizeof(int) * size);
  main_queue->max_size = size;
  main_queue->front = 0;
  main_queue->rear = 0;
  main_queue->current_size = 0;
}

int QueueInsert(queue_t *main_queue, int number) {
  //Check if the queue is full or not, if not put the Number--
  //--at the rear end.
  if(main_queue->current_size != main_queue->max_size) {
    main_queue->memory[main_queue->rear] = number;
    (main_queue->current_size)++;
    (main_queue->rear)++;
    //If the rear is now at the end of the queue. Reset rear.
    if(main_queue->rear == main_queue->max_size)
      main_queue->rear = 0;
    //If inserting is successful return 1
    return 1;
  } else {
    return 0;
  }
}

int QueueRemove(queue_t *main_queue) {
  if(main_queue->current_size != 0) {
    int temp;
    temp = main_queue->memory[(main_queue->front)];
    (main_queue->front)++;
    //If front is at the end of the queue reset it.
    if(main_queue->front == main_queue->max_size)
      main_queue->front = 0;
    (main_queue->current_size)--;
    return temp;
  } else {
    //If Removing is unsuccessful return -1
    return -1;
  }
}

void QueueDestroy(queue_t *main_queue) {
  free(main_queue->memory);
  free(main_queue);
}
