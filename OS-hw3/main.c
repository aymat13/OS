#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ucontext.h>
#include <stdio.h>
#include <ctype.h>

#define SLEEP_AMNT 100000

int isnumber(char *string) {
  int i = 0;
  int length = strlen(string);
  while(i < length) {
    if(!isdigit(string[i])) return 0;
    i++;
  }
  return 1;
}

void worker_funct(int n, int ID) {
  int i;
  unsigned int usecs = SLEEP_AMNT;
  for(i = 0;i<n;i++) {
    printf("Thread #%d -> %d\n", ID, i);
    usleep(usecs);
  }
  printf("\n");
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
  worker_funct(entered_numbers[0],1);
  worker_funct(entered_numbers[1],2);
  free(entered_numbers);
}
