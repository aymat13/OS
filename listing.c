#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void writeInitialPath(char *inputDirectory, FILE *fptr){
	char dir[1024];
	fputs("2030112\n", fptr);
	if(getcwd(dir, sizeof(dir)) != NULL){
		sprintf(dir,"%s/%s\n",dir,(inputDirectory+2));
		fputs(dir, fptr);
	}
	else{
		printf("error at getcwd\n");
		exit(3);
	}	
}

void listing(char *inputDirectory, char *startDirectory, FILE *fptr){
	DIR *d;
	struct dirent *dir;
	d = opendir(inputDirectory);
	char tmp[1024];
	if(d == NULL) return;


		while((dir = readdir(d)) != NULL){
			if(dir->d_type != DT_DIR){
				if(startDirectory == NULL){
					sprintf(tmp,"%s\n",dir->d_name);
					fputs(tmp, fptr);
				}
				else{
					sprintf(tmp,"%s/%s\n",startDirectory,dir->d_name);
					fputs(tmp, fptr);
				}

				
			}
			else{
				if(dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0){
					//printf("%s\n",dir->d_name);
					char d_path[255];
					char write_path[255];
					if(startDirectory == NULL)
						sprintf(write_path,"%s",dir->d_name);
					else
						sprintf(write_path,"%s/%s",startDirectory,dir->d_name);
					sprintf(d_path,"%s/%s",inputDirectory,dir->d_name);
					//			printf("%s\n",d_path);
					listing(d_path, write_path, fptr);
				}
			}
		}
	closedir(d);
}

int main(int argc, char **argv){
	if(argc < 2){
		printf("Too few argumants\n");
		exit(1);
	}


	FILE *fptr;
	char file[1024];
	if(getcwd(file, sizeof(file)) != NULL){
		sprintf(file,"%s/example.list",file);
			fptr = fopen(file,"a+");
			if(fptr == NULL){
			perror("fopen error\n");
			exit(2);
			}		
	}
	else{
		printf("error at getcwd\n");
		exit(3);
	}	

	writeInitialPath(argv[1],fptr);
	listing(argv[1],NULL,fptr);
	fclose(fptr);

	return 0;
}