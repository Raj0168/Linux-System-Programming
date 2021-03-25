#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/shm.h> /* for shmget() shmctl() */
#include <signal.h> /* for signal(2) kill(2) */
#include <errno.h> /* for perror */
#include <stdlib.h>

int shmid;
typedef void (*sighandler_t)(int);

int main(int argc,char* argv[]) {
        int status;
        pid_t pid=0;
        pid_t cpid=0;
        key_t shmkey;
	int *rollno;
        int n = atoi(argv[1]);
	int i;

        //sighandler_t shandler;
        //shandler =  signal(SIGINT, releaseSHM); 
	
        shmkey = ftok("/home/linux/Desktop/OSLab/temp/", 1);

        shmid =  shmget(shmkey , n*sizeof(int), IPC_CREAT | 0777);

        if (shmid == -1) { /* shmget() failed() */
            perror("shmget() failed: ");
            exit (1);
        }

        printf("shmget() returns shmid = %d.\n", shmid);


        rollno = shmat(shmid, NULL, 0);
        if (rollno == (void *)-1) {
		perror("shmat() failed");
		exit (1);
        }

        *(rollno+n-1)=n;


}

