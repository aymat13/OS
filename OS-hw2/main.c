#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int printDivisors(int number) {
  printf("Number: %d, Divisors: ", number);
  for(int i=1;i<=number;i++) {
    if(number%i == 0) {
      printf("%d ", i);
    }
  }
  printf("\n");
}

int main() {
  queue_t *main_queue = malloc(sizeof(queue_t));
  QueueInitialize(main_queue, 5);
  QueueInsert(main_queue, 18);
  QueueInsert(main_queue, 11);
  QueueInsert(main_queue, 13);
  QueueInsert(main_queue, 51);
  QueueInsert(main_queue, 777);
  QueueInsert(main_queue, 121);
  for(int i=0; i<6; i++) {
    printf ("Size: %d, ", main_queue->current_size);
    int temp = QueueRemove(main_queue);
    printf("Number: %d\n", temp);
  }
  QueueDestroy(main_queue);
  return 0;
}
