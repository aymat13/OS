#include <stdlib.h>
#include <stdio.h>


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
  int n = 999;
  printDivisors(n);
  n = 867;
  printDivisors(n);
  return 0;
}
