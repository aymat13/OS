#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include "queue.h"
#include <getopt.h>
#include <string.h>

int isnumber(char *string) {
  int i = 0;
  int length = strlen(string);
  while(i < length) {
    if(!isdigit(string[i])) return 0;
    i++;
  }
  return 1;
}

int printDivisors(int number) {
  printf("Number: %d, Divisors: ", number);
  for(int i=1;i<=number;i++) {
    if(number%i == 0) {
      printf("%d ", i);
    }
  }
  printf("\n");
}



int main(int argc, char **argv) {
  char * t = NULL;
  char * q = NULL;
  char * r = NULL;
  char * m = NULL;
  int c;
  int thread_count = 5;
  int max_size = 10;
  int rand_count = 20;
  int rand_range = 100;

  while((c = getopt(argc, argv, "t:q:r:m:")) != -1) {
    switch(c) {
      case 't':
        if(isnumber(optarg))
          t = optarg;
        else {
          fprintf(stderr, "Please enter a number as an argument, for option %c\n", c);
          return 1;
        }
        break;
      case 'q':
        if(isnumber(optarg))
          q = optarg;
          else {
            fprintf(stderr, "Please enter a number as an argument, for option %c\n", c);
            return 1;
          }
          break;
      case 'r':
        if(isnumber(optarg))
          r = optarg;
          else {
            fprintf(stderr, "Please enter a number as an argument, for option %c\n", c);
            return 1;
          }
          break;
      case 'm':
        if(isnumber(optarg))
          m = optarg;
          else {
            fprintf(stderr, "Please enter a number as an argument, for option %c\n", c);
            return 1;
          }
          break;
      case '?':
        return 1;
      default:
        abort ();
    }
  }
  if (t!= NULL)  thread_count = atoi(t);
  if (q!= NULL)  max_size = atoi(q);
  if (r!= NULL) rand_count = atoi(r);
  if (m!= NULL) rand_range = atoi(m);

  queue_t *main_queue = malloc(sizeof(queue_t));
  QueueInitialize(main_queue, 5);
  printf("Insert: %d\n", QueueInsert(main_queue, 18));
  printf("Insert: %d\n", QueueInsert(main_queue, 11));
  printf("Insert: %d\n", QueueInsert(main_queue, 17));
  printf("Insert: %d\n", QueueInsert(main_queue, 777));
  printf("Insert: %d\n", QueueInsert(main_queue, 121));
  printf("Insert: %d\n", QueueInsert(main_queue, 666));
  for(int i=0; i<3; i++) {
    printf ("Size: %d, ", main_queue->current_size);
    int temp = QueueRemove(main_queue);
    printf("Number: %d\n", temp);
  }
  printf("Insert: %d\n", QueueInsert(main_queue, 13));
  printf("Insert: %d\n", QueueInsert(main_queue, 14));
  printf("Insert: %d\n", QueueInsert(main_queue, 15));
  for(int i=0; i<8; i++) {
    printf ("Size: %d, ", main_queue->current_size);
    int temp = QueueRemove(main_queue);
    printf("Number: %d\n", temp);
  }
  QueueDestroy(main_queue);
  return 0;
}
