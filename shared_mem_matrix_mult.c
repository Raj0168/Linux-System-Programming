#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/shm.h> /* for shmget() shmctl() */
#include <signal.h> /* for signal(2) kill(2) */
#include <errno.h> /* for perror */
#include <stdlib.h>

#define N 4

int shmid; /* defined globally so that signal handler can access it */

/* following is a signal handler for the keypress ^C, that is, ctrl-c */
typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        // int shmctl(int shmid, int cmd, struct shmid_ds *buf);
        status = shmctl(shmid, IPC_RMID, NULL); /* IPC_RMID is the command for destroying the shared memory*/
        if (status == 0) {
                fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
        }

        // int kill(pid_t pid, int sig);
        status = kill(0, SIGKILL);
        if (status == 0) {
                fprintf(stderr, "kill susccesful.\n"); /* this line may not be executed :P WHY?*/
        } else if (status == -1) {
                perror("kill failed.\n");
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "kill(2) returned wrong value.\n");
        }
}


int mata[4][4], matb[4][4];
typedef void (*sighandler_t)(int);

int main()
{
	pid_t status;
	pid_t pid=0;
	pid_t cpid=0;
	int ra=4, rb=4, rc, ca=4, cb=4, cc;
	int i, j, k;
	rc=ra;
	cc=cb;
	int *shm_ptr=0;
	sighandler_t shandler;
	int rows=10, columns=10;
	shandler=signal(SIGINT, releaseSHM);
	shmid=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0777);

	int mata[N][N]= { {1, 2, 3, 4 },
                        { 3, 7, 2, 1 },
                        { 6, 1, 0, 5 },
                        { 10, 0, 8, 3 } };
	
	int matb[N][N]={ {7, 4, 1, 0 },
                        { 3, 9, 10, 2 },
                        { 4, 9, 7, 1 },
                        { 7, 5, 2, 0 } };

	for(int i=0;i<rc;i++)
	{
		for(int j=0;j<cc;j++)
                {
			pid=fork();
			if(pid==0)
			{
				printf("Child process id:%d\n",getpid());

                                shm_ptr = (int*)shmat(shmid,NULL,0);
                                if(shm_ptr == (void*)-1)
                                {
					perror("shmat");
                                        exit(1);
                                }

                                for(k=0;k<ca;k++)
					shm_ptr[i*columns+j]=shm_ptr[i*columns+j]+mata[i][k]*matb[k][j];
				exit(0);
                        }
		}
	}

	while(cpid=wait(&status)!=-1);

        shm_ptr = shmat(shmid,NULL,0);  //shm_ptr char *ptr=NULL flag=0
        if(shm_ptr==(void*)-1)
        {
		perror("shmat");
                exit(1);
	}
	
	printf("Matrix C:\n");
	for(i=0; i<rc ;i++)
        {
		for(j=0; j<cc; j++)
			printf("%d   ",shm_ptr[i*columns+j]);
                printf("\n");
	}
	
	printf("pid1 = %d status = %d\n", cpid, status);
	
	wait(&status);
	shmdt((void*)shm_ptr);
        shmctl(shmid,IPC_RMID,NULL);

        exit(0);
}

