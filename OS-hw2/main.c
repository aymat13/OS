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
#include <time.h>

sem_t mutex;
sem_t print_mutex;

struct thread_args {
  queue_t *main_queue;
  int rand_count;
  int rand_range;
};

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
  unsigned int tid = pthread_self();
  sem_wait(&mutex);
  //while(1) {
  while(main_queue->current_size != 0) {
    number = QueueRemove(main_queue);
    sem_post(&mutex);
    unsigned int usecs = (((rand()%10)+1) * 100000);
    usleep(usecs);
    sem_wait(&print_mutex);
    printf("Thread ID: %u, Number: %d, Divisors: ", tid, number);
    for(int i=1;i<=number;i++) {
      if(number%i == 0) {
        printf("%d ", i);
      }
    }
    printf("\n");
    sem_post(&print_mutex);
    sem_wait(&mutex);
  }
  sem_post(&mutex);
}

void *Generator(void *ptr) {
  struct thread_args *args = (struct thread_args*)ptr;
  queue_t *main_queue = args->main_queue;
  int rand_count = args->rand_count;
  int rand_range = args->rand_range;
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
  sem_init(&print_mutex,0,1);
  pthread_t thread_gen;
  pthread_t *thread_work = malloc(sizeof(pthread_t) * thread_count);
  queue_t *main_queue = malloc(sizeof(queue_t));
  QueueInitialize(main_queue, max_size);
  struct thread_args *ptr = malloc(sizeof(struct thread_args));
  ptr->main_queue = main_queue;
  ptr->rand_count = rand_count;
  ptr->rand_range = rand_range;
  pthread_create(&thread_gen,NULL,Generator,(void*)ptr);
  while(j<thread_count) {
    pthread_create(&(thread_work[j]),NULL,printDivisors,(void*)main_queue);
    j++;
  }
  j = 0;
  for(j=0;j<thread_count;j++) {
    pthread_join(thread_work[j],NULL);
  }
  pthread_join(thread_gen,NULL);

  QueueDestroy(main_queue);
  free(thread_work);
  free(ptr);
  return 0;
}
