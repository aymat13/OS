#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  char *test_directory;
  if(argc!=2) {
    printf("Please enter the testing directory as argument");
    exit(0);
  }
  return(0);
  test_directory = argv[1];
}
