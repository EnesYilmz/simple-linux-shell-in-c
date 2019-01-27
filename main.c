#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LENGTH 128

int main() {
	char line[MAX_LENGTH];
	char *token;
	char *argv[100];
	int argc;

	while (1) {
		printf("> ");
    	fflush(stdout); //Print buffer clear

    	if (!fgets(line, MAX_LENGTH, stdin)) break; //User input

		int length = strlen(line);
		if (line[length - 1] == '\n') //Delete new line
    		line[length - 1] = '\0';

    	token = strtok(line," ");
    	int i=0;
    	while(token!=NULL){ //Splitting the input
        	argv[i]=token;      
        	token = strtok(NULL," ");
        	i++;
    }
    argv[i]=NULL;

	if (strcmp(argv[0], "quit") == 0){ //Exit
    	exit(1);
	}

	int pid= fork();

    if(pid==0){ //Child process
    	argc = i;
    	if(argc == 4){
    		if(strcmp(argv[0], "cat") == 0 && strcmp(argv[2], ">") == 0){ //Output routing
    			int fd = open(argv[1],O_RDONLY,0);
    			if(fd >= 0){
				dup2(fd,STDIN_FILENO);
				close(fd);

				fd = creat(argv[3],0664);
				dup2(fd,STDOUT_FILENO);
				argv[2] = NULL;
				argv[3] = NULL;
				execvp(argv[0],argv);
				close(fd);
				}else{
                printf("%s input file not found.\n",argv[1]);
            	}
    		}
    	}
    	else if(argc == 3){
    		if(strcmp(argv[0], "cat") == 0 && strcmp(argv[1], "<") == 0){ //Input routing
    			int fd = open(argv[2],O_RDONLY,0);
    			if(fd >= 0){
				dup2(fd,STDIN_FILENO);
				argv[1] = argv[2];
				argv[2] = NULL;
				execvp(argv[0],argv);
				close(fd);
				}else{
                printf("%s input file not found.\n",argv[2]);
            	}
    		}
    	}
    	else{
    		execvp(argv[0],argv); //Single Commands
    	}
        return 0;
    }
    else{ //Parent process
        wait(NULL);
        printf("\n");
    }
  }

	return 0;
}