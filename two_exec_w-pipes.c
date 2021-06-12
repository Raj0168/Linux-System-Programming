#include <sys/wait.h> /* wait */
#include <stdio.h>
#include <stdlib.h>   /* exit functions */
#include <unistd.h>   /* read, write, pipe, _exit */
#include <string.h>

#define ReadEnd 0
#define WriteEnd 1

int main(int argc, char *argv[]) {

	int pipeFDs[2];
	if(pipe(pipeFDs) == -1) {
		fprintf(stderr, "pipe creation failed");
		exit(1);
	}
	pid_t pid1=fork();
	if(pid1==-1)
			fprintf(stderr, "fork system\n");

	else if(pid1==0) {
			if(close(pipeFDs[ReadEnd]) == -1) {
				fprintf(stderr, "close function 1 failed\n");
				exit(1);
			}

				if (dup2(pipeFDs[WriteEnd], STDOUT_FILENO) == -1) {
					fprintf(stderr, "dup2 function 1 failed\n");
					exit(1);
				}
				if(close(pipeFDs[WriteEnd]) == -1) {
					fprintf(stderr, "close function 2 failed\n");
					exit(1);
				}

			if(execlp(argv[1], argv[1], (char*) NULL) == -1) {
				fprintf(stderr, "execlp function 1 failed\n");
				exit(1);
			}
	}

        pid_t pid2=fork();
	
	if(pid2==-1)
			fprintf(stderr, "fork failed\n");

	if(pid2==0) {
			if(close(pipeFDs[WriteEnd]) == -1)
			{
				fprintf(stderr, "close function 3 failed\n");
				exit(1);
			}
			if(pipeFDs[ReadEnd] != STDIN_FILENO) {
				if (dup2(pipeFDs[ReadEnd], STDIN_FILENO) == -1) 
				{
					fprintf(stderr, "dup2 function 2 failed\n");
					exit(1);
				}
				if(close(pipeFDs[ReadEnd]) == -1) 
				{
					fprintf(stderr, "close function 4 failed\n");
					exit(1);
				}
			}

			if(execlp(argv[2], argv[2], (char*) NULL) == -1) 
			{
				fprintf(stderr, "execlp function 2 failed\n");
				exit(1);
			}

			_exit(0);
	}

	if(close(pipeFDs[ReadEnd]) == -1) {
		fprintf(stderr, "close function 5 failed\n");
		exit(1);
	}
	if(close(pipeFDs[WriteEnd]) == -1) {
		fprintf(stderr, "close function 6 failed\n");
		exit(1);
	}
	if(wait(NULL) == -1) {
		fprintf(stderr, "wait function 1 failed\n");
	}
	if(wait(NULL) == -1) {
		fprintf(stderr, "wait function 2 failed\n");
	}
	return 0;
}

