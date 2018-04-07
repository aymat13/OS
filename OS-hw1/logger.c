#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct node {
  char *dir_name;
  char *prev_name;
  int dead_end;
  struct node *next;
}node_t;

void AppendNode(node_t *head, char *name, char *prev_name, int dead_end) {
      node_t *current = head;
      while(current->next != NULL) {
        current = current->next;
      }
      current->next = malloc(sizeof(node_t));
      current->next->dir_name =  malloc(strlen(name)+1);
      strcpy(current->next->dir_name,name);
      current->next->prev_name = malloc(strlen(prev_name)+1);
      strcpy(current->next->prev_name,prev_name);
      current->next->dead_end = dead_end;
      current->next->next = NULL;
    }

void RemoveList(node_t *head) {
  node_t *current = head;
  node_t *temp=head;
  while(current != NULL) {
    temp = current->next;
    free(current);
    current=temp;
  }
}

int SearchList(node_t *head, char *name, char *prev_name) {
  node_t *current = head;
  while(current->next != NULL) {
    if((current->dir_name == name) && (current->prev_name==prev_name))
      return 1;
    else
      current = current->next;
  }
  return 0;
}

void PrintList(node_t *head) {
  node_t *current = head;
  while(current !=NULL) {
    printf("Name: %s\n", current->dir_name);
    printf("Prev_Name: %s\n", current->prev_name);
    printf("Deadend?? %d\n\n\n", current->dead_end);
    current=current->next;
  }
}

void recursive(node_t **head, char *directory, char *additional) {
  int dead_end=0;
  char *new_name;
  char *dir_name,*test_direct;
  char *prev_name;
  struct dirent *dir, *temp_dir;
  DIR *d;
  d = opendir(directory);
  if(d) { //if d is a correct directory
      while((dir=readdir(d)) != NULL) {
        if(strcmp(dir->d_name, ".")==0 || strcmp(dir->d_name, "..")==0) {
          continue;
        }
        dir_name = dir->d_name;
        if(additional==NULL) {
          test_direct = malloc(strlen(dir_name)+2);
          strcpy(test_direct,dir_name);
        }
        else {
          test_direct = malloc(strlen(additional)+strlen(dir_name)+2);
          strcpy(test_direct,additional);
          strcat(test_direct,"/");
          strcat(test_direct,dir_name);
        }
        new_name = malloc(strlen(directory)+strlen(dir_name)+2);
        strcpy(new_name,directory);
        strcat(new_name,"/");
        strcat(new_name,dir_name);
        if(dir->d_type != DT_DIR){ //We have encountered a deadend go back.
          printf("New Name: %s\n",new_name);
          printf("dir_name: %s\n", dir_name);
          printf("prev_name: %s\n", prev_name);
          AppendNode(*head,dir_name,test_direct,dead_end);
          free(new_name);
          free(test_direct);
        }
        else {
          recursive(head, new_name, test_direct);
          free(new_name);
          free(test_direct);
        }
      }
      closedir(d);//END OF WHILE
  } //end of if(d)
  else {
    printf("Please Enter a Correct Directory\n");
    RemoveList(*head);
    exit(0);
  }
}

int main(int argc, char *argv[]) {
  if(argc!=2) {
    printf("Please enter the testing directory as argument\n");
    exit(0);
  }
  char buffer[1024];
  char *test_directory;
  char *addit = NULL;
  //Create the head of our list.
  node_t *head = NULL;
  int opened_in_wb=0;
  test_directory = argv[1];
  head = malloc(sizeof(node_t));
  head->dir_name=test_directory;
  head->prev_name=test_directory;
  head->dead_end=0;
  head->next=NULL;
  recursive(&head,test_directory,addit);
  PrintList(head); /*
  FILE *fp = fopen("test.list","a+");
  fgets(buffer,1024,(FILE*)fp);
  if(strlen(buffer)!=0) {
    if(strcmp(buffer,"2030005")) {
      fgets(buffer,1024,(FILE*)fp);
      if(buffer,
    }
  }*/
  RemoveList(head);
  return(0);
}
