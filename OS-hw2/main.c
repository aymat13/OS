#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include "queue.h"
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t mutex;

int isnumber(char *string) {
  int i = 0;
  int length = strlen(string);
  while(i < length) {
    if(!isdigit(string[i])) return 0;
    i++;
  }
  return 1;
}

void *printDivisors(void *main_queue_ptr) {
  queue_t *main_queue = (queue_t *) main_queue_ptr;
  int number;
  sem_wait(&mutex);
  while(main_queue->current_size != 0) {
    number = QueueRemove(main_queue);
    sem_post(&mutex);
    nanosleep(((((rand()%10)+1) * 100000000) - 1),NULL);
    printf("Number: %d, Divisors: ", number);
    for(int i=1;i<=number;i++) {
      if(number%i == 0) {
        printf("%d ", i);
      }
    }
    printf("\n");
    sem_wait(&mutex);
  }
  sem_post(&mutex);
}

void *Generator(queue_t *main_queue, void *rand_count_ptr_void, void *rand_range_ptr_void) {
  int *rand_count_ptr = (int *) rand_count_ptr_void;
  int *rand_range_ptr = (int *) rand_range_ptr_void;
  int rand_count = *rand_count_ptr;
  int rand_range = *rand_range_ptr;
  int rdm_number=0;
  int i = 0;
  while(i<rand_count) {
    rdm_number = rand() % (rand_range+1);
    sem_wait(&mutex);
    if(QueueInsert(main_queue,rdm_number)){
    sem_post(&mutex);
      i++;
    } else {
    sem_post(&mutex);
      continue;
    }
  }
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
  int j = 0;
  sem_init(&mutex,0,1);
  pthread_t thread_gen;
  pthread_t *thread_work = malloc(sizeof(pthread_t) * thread_count);
  queue_t *main_queue = malloc(sizeof(queue_t));
  QueueInitialize(main_queue, max_size);
  pthread_create(&thread_gen,NULL,Generator,(void*)main_queue,(void*)&rand_count,(void*)&rand_range);
  while(j<thread_count) {
    pthread_create(&(thread_work[j]),NULL,printDivisors,(void*)main_queue);
    j++;
  }

  //Generator(main_queue,rand_count,rand_range);
  QueueDestroy(main_queue);
  free(thread_work);
  return 0;
}
