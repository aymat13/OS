#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ucontext.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#define SLEEP_AMOUNT 100000
#define STACK_SIZE 64000
#define READY 0
#define RUNNING 1
#define FINISHED 2
#define EMPTY 3

typedef struct ThreadInfo {
  ucontext_t context;
  int state;
}threadinfo_t;

threadinfo_t global_array[5];
int next_index = 0;
int prev_index = 1;
int no_more_args = 0;

int isnumber(char *string) {
  int i = 0;
  int length = strlen(string);
  while(i < length) {
    if(!isdigit(string[i])) return 0;
    i++;
  }
  return 1;
}

void worker_funct(int n, int ID, int index) {
  int i;
  unsigned int usecs = SLEEP_AMOUNT;
  for(i = 0;i<n;i++) {
    printf("Thread #%d -> %d\n", ID, i);
    usleep(usecs);
  }
  printf("\n");
  global_array[index].state = FINISHED;
  alarm(0);
  raise(SIGALRM);
}

int scheduler() {
  alarm(1);
  int empty_count=0;
  int temp_index;
  if(no_more_args!=1) {
    for(int i=1;i<5;i++) {
      if(global_array[i].state == EMPTY) {
        swapcontext(&(global_array[prev_index].context),&(global_array[0].context));
        return 0;
      }
    }
  } else {
    for(int i=1;i<5;i++) {
      if(global_array[i].state == EMPTY) {
        empty_count++;
      }
    }
    if(empty_count == 4) {
      printf("3. cikis\n");
      exit(3);
    }
  }
  //If prev_thread is finished then free its stack and make its space empty.
  if(global_array[prev_index].state == FINISHED) {
    printf("Is finished? \n");
    free(global_array[prev_index].context.uc_stack.ss_sp);
    global_array[prev_index].state == EMPTY;
  } //If it is not finished just make it ready.
  else if(global_array[prev_index].state == RUNNING) {
    printf("Or was it running?\n");
    global_array[prev_index].state = READY;
  }
  if(next_index == 4) next_index = 1;
  else next_index++;
  int k;
  for(k=1;k<5;k++) { //Search for next available thread to switch
    if(global_array[next_index].state == READY)
      break; //After finding the first next ready thread index, break from loop.
    else {
      if(next_index!=4) next_index++; //Otherwise go through the whole array.
      else next_index = 1;
    }
  }
  if(k==5 && no_more_args==1) {
    printf("4. cikis\n");
    exit(4); //If Above loop could not found any READY
  }
  //thread and there are no more threads to be scheduled then exit.
  else if(k==5 && no_more_args==0) next_index = 0; //If there are threads -->
  //to be scheduled go to the main function to schedule them.

  printf("Prev_index: %d\n", prev_index);
  printf("Next_index: %d\n", next_index);

  temp_index = prev_index;
  prev_index = next_index;
  global_array[next_index].state = RUNNING;
  swapcontext(&(global_array[temp_index].context),&(global_array[next_index].context));
  prev_index = next_index;
}

int main(int argc, char** argv) {
  if(argc<2) {
    printf("Error: Program Need Arguments!\n");
    exit(0);
  }
  for(int i=1;i<argc;i++) {
    if(!isnumber(argv[i])) {
      printf("Error: All arguments must be numbers!\n");
      exit(1);
    }
  }
  int *entered_numbers;
  entered_numbers = malloc(sizeof(int) * (argc - 1));
  for(int i=1;i<argc;i++) {
    if(argv[i]!=NULL) entered_numbers[i-1] = atoi(argv[i]);
    else break;
  }

  signal (SIGALRM, (void (*)(int))scheduler);
  for(int i=0;i<5;i++) {
    global_array[i].state = EMPTY;
  }

  int j=1;
  while(argc!=1) {
    for(int i=1;i<5;i++) {
      if(global_array[i].state == EMPTY) {
        getcontext(&(global_array[i].context));
        global_array[i].context.uc_link = 0;
        global_array[i].context.uc_stack.ss_sp = malloc(STACK_SIZE);
        global_array[i].context.uc_stack.ss_size = STACK_SIZE;
        global_array[i].context.uc_stack.ss_flags = 0;
        makecontext(&(global_array[i].context), (void (*)(void))worker_funct,
          3, entered_numbers[j-1],j,i);
        global_array[i].state = READY;
        argc--;
        j++;
      }
    }
    prev_index = 0;
    alarm(0);
    getcontext(&(global_array[0].context));
    global_array[0].state = 5;
    printf("kek\n");
    raise(SIGALRM);
    printf("lel\n");
  }
  no_more_args = 1;
  free(entered_numbers);
  prev_index = 0;
  alarm(0);
  getcontext(&(global_array[0].context));
  global_array[0].state = 5;
  raise(SIGALRM);
  while(1) {

  }
}
