/***** LAB3 base code *****/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


// ** TA Added includes to get source code to compile:
#include <unistd.h>



char gpath[128];    // hold token strings 
char *arg[64];      // token string pointers
int  n;             // number of token strings

char dpath[128];    // hold dir strings in PATH
char *dir[64];      // dir string pointers
int  ndir;          // number of dirs   

//for pipes
int pd[2];
char *head[64];
char *tail[64];

int tokenize(char *pathname) // YOU have done this in LAB2
{                            // YOU better know how to apply it from now on
  char *s;
  strcpy(gpath, pathname);   // copy into global gpath[]
  s = strtok(gpath, " ");    
  n = 0;
  while(s){
    arg[n++] = s;           // token string pointers   
    s = strtok(0, " ");
  }
  arg[n] =0;                // arg[n] = NULL pointer 
  return 0;
}

int checkPipes(char *env[ ]){	
	for(int i = 0; i < n; i++){
		if(strcmp(arg[i], "|") == 0){				//if we are at the pipe
			printf("FOUND PIPE\n");
			for(int x = i+1; x < n; x++){			//copy the rest of the args to tail
				tail[x-(i+1)] = arg[x];
			}
			
			//create the paths for execve
			char headcmd[128] = "/"; strcat(headcmd, head[0]);
			char tailcmd[128] = "/"; strcat(tailcmd, tail[0]);
			
			//remove the command to create the args for execve
			int j = 1;
			while(head[j] != 0){
				head[j-1] = head[j];
				j++;
			}
			head[j-1] = 0;
			j = 1;
			while(tail[j] != 0){
				tail[j-1] = tail[j];
				j++;
			}
			tail[j-1] = 0;
			
			//print the head and tail paths
			printf("HEAD PATH: %s\n", headcmd);
			printf("TAIL PATH: %s\n", tailcmd);
			
			//print the head and tail arguments
			j = 0;
			printf("HEAD ARGS: ");
			while(head[j] != 0){
				printf("%s ", head[j]);
				j++;
			}
			printf("\n");
			j = 0;
			printf("TAIL ARGS: ");
			while(tail[j] != 0){
				printf("%s ", tail[j]);
				j++;
			}
			printf("\n");
			
			pipe(pd);									//create pipe
			int pid = fork();							//fork child
			
			if(pid){										//parent as pipe WRITER
				close(pd[0]);
				close(1);
				dup(pd[1]);								//replace 1 with pd[1]
				close(pd[1]);							//close pd[1]
				int r = execve(headcmd, head, env);
				printf("execve failed r = %d\n", r);
				exit(1);
			}
			else{											//child as pipe READER
				close(pd[1]);
				close(0);
				dup(pd[0]);								//replace 0 with pd[0]
				close(pd[0]);							//close pd[0]
				int r = execve(tailcmd, tail, env);
				printf("execve failed r = %d\n", r);
				exit(1);
			}
			return 1;									//pipe found
		}
		else{												//we are before the pipe
			head[i] = arg[i];							//copy from before pipe into head
		}
	}
	printf("NO PIPE\n");
	return 0;											//pipe not found
}

int main(int argc, char *argv[ ], char *env[ ])
{
	int i;
	int pid, status;
	char *cmd;
	char line[28];

	// The base code assume only ONE dir[0] -> "/bin"
	// YOU do the general case of many dirs from PATH !!!!
	dir[0] = "/bin";
	ndir   = 1;

	// show dirs
	for(i=0; i<ndir; i++)
	printf("dir[%d] = %s\n", i, dir[i]);

	while(1){
		printf("sh %d running\n", getpid());
		printf("enter a command line : ");
		fgets(line, 128, stdin);
		line[strlen(line) - 1] = 0; 
		if (line[0]==0)
			continue;

		tokenize(line);

		for (i=0; i<n; i++){  // show token strings   
		  printf("arg[%d] = %s\n", i, arg[i]);
		}
		
		cmd = arg[0];         // line = arg0 arg1 arg2 ... 

		if (strcmp(cmd, "cd")==0){
			chdir(arg[1]);
			continue;
		}
		if (strcmp(cmd, "exit")==0)
			exit(0); 
		
		//only do the regular stuff if there's not any pipes
		if(!checkPipes(env)){
			pid = fork();
			
			if (pid){
				printf("sh %d forked a child sh %d\n", getpid(), pid);
				printf("sh %d wait for child sh %d to terminate\n", getpid(), pid);
				pid = wait(&status);
				printf("ZOMBIE child=%d exitStatus=%x\n", pid, status); 
				printf("main sh %d repeat loop\n", getpid());
			}
			else{
				printf("child sh %d running\n", getpid());
					
				if(strcmp(cmd, "./a.out") == 0){
					//make a cmd line for execve() if we're running a.out
					strcpy(line, ".//"); strcat(line, cmd);
				}
				else{
					// make a cmd line = dir[0]/cmd for execve()
					strcpy(line, dir[0]); strcat(line, "/"); strcat(line, cmd);
				}
				
				printf("line = %s\n", line);

				//check for input/output redirection
				for(i=0; i<n; i++){
					if(strcmp(arg[i], ">") == 0){					//output overwrite
						arg[i] = 0;
						close(1);
						open(arg[i+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
						break;
					}
					else if(strcmp(arg[i], ">>") == 0){			//output append
						arg[i] = 0;
						close(1);
						open(arg[i+1], O_WRONLY|O_CREAT|O_APPEND, 0644);
						break;
					}
					else if(strcmp(arg[i], "<") == 0){			//input overwrite
						arg[i] = 0;
						close(0);
						open(arg[i+1], O_RDONLY, 0644);
						break;
					}
				}
				
				int r = execve(line, arg, env);

				printf("execve failed r = %d\n", r);
				exit(1);
			}
		}
	}
}

/********************* YOU DO ***********************
1. I/O redirections:

Example: line = arg0 arg1 ... > argn-1

  check each arg[i]:
  if arg[i] = ">" {
     arg[i] = 0; // null terminated arg[ ] array 
     // do output redirection to arg[i+1] as in Page 131 of BOOK
  }
  Then execve() to change image


2. Pipes:

Single pipe   : cmd1 | cmd2 :  Chapter 3.10.3, 3.11.2

Multiple pipes: Chapter 3.11.2
****************************************************/

    
