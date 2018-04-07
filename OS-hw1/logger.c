#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  char *dir_name;
  long int loc;
  long int prev_loc;
  int dead_end;
  struct node *next;
}node_t;

void AppendNode(node_t *head, char *name, long int loc, long int prev_loc,
        int dead_end) {
      node_t *current = head;
      while(current->next != NULL) {
        current = current->next;
      }
      current->next = malloc(sizeof(node_t));
      current->next->dir_name = name;
      current->next->loc = loc;
      current->next->prev_loc = prev_loc;
      current->next->dead_end = dead_end;
      current->next->next = NULL;
    }

void RemoveList(node_t *head) {
  node_t *current = head;
  node_t *temp=head;
  while(current->next != NULL) {
    temp = current->next;
    free(current);
    current=temp;
  }
  free(temp);
  free(current);
}

int SearchList(node_t *head, long int loc) {
  node_t *current = head;
  while(current->next != NULL) {
    if(current->loc == loc)
      return 1;
    else
      current = current->next;
  }
  return 0;
}

node_t FindNode(node_t *head, long int loc) {
  node_t *current = head;
  while(current->next != NULL) {
    if(current->loc == loc)
      return current;
    else
      current = current->next;
  }
  return NULL;
}

void PrintList(node_t *head) {
  node_t *current = head;
  while(current !=NULL) {
    printf("Name: %s\n", current->dir_name);
    printf("Location: %ld\n", current->loc);
    printf("")
  }
}

int main(int argc, char *argv[]) {
  if(argc!=2) {
    printf("Please enter the testing directory as argument");
    exit(0);
  }
  const char *test_directory;
  DIR *d;
  //Create the head of our list.
  node_t *head = NULL;
  head = malloc(sizeof(node_t));
  head->dir_name="Parent";
  head->loc=0;
  head->prev_loc=0;
  head->next=NULL;
  node_t *temp_node;
  long int loc = 0;
  long int prev_loc = 0;
  int count=0;
  int dead_end=0;
  struct dirent *dir, *temp_dir;
  test_directory = argv[1];
  d = opendir(test_directory);
  if(d) { //if d is a correct directory
    while((dir=readdir(d)) != NULL) { //To find the entry count in the main directory
      if(strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..")) {
        count++;
      }
    }
    if(count==0) {
      printf("Empty Directory.\n");
    }
    rewinddir(d); //Start over
    while(1) {
      if((dir=readdir(d)) != NULL) {
        loc = telldir(d);
        if(SearchLish(loc)) {
          continue;
        }
        else {
          seekdir(d,loc);
          if((temp_dir=readdir(d)) != NULL) {
            dead_end=0;
          }
          else {
            dead_end=1;
          }
          AppendNode(head,dir->d_name,loc,prev_loc,dead_end);
          if(dead_end==0) {
            prev_loc=loc;
            seekdir(d,loc);
          }
          else { //We have encountered a deadend go back.
            seekdir(d,prev_loc);
          }
        }
      }//END OF if(dir=readdir(d) != NULL)
      else {
        temp_node = FindNode(head,prev_loc);
        if(temp_node->prev_loc != 0){
          temp_node = FindNode(head,prev_loc);
          prev_loc = temp_node->prev_loc;
          //If the previous node is not the parent just go back.
        }
        else { //If it is the parent, then rewind and decrement count
          prev_loc = 0;
          rewind(d);
          count--;
          if(count==0)
            break;
        }
      }
    }
  } //end of if(d)
  else {
    printf("Please Enter a Correct Directory");
    RemoveList(head);
    exit(0);
  }
  closedir(d);
  RemoveList(head);
  return(0);
}
