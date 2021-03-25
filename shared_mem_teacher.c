#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/shm.h> /* for shmget() shmctl() */
#include <signal.h> /* for signal(2) kill(2) */
#include <errno.h> /* for perror */
#include <stdlib.h>



int shmid;/* defined globally so that signal handler can access it */
int *rollno, n;

/* following is a signal handler for the keypress ^C, that is, ctrl-c */
typedef void (*sighandler_t)(int);

void releaseSHM(int signum) {
        int status;
	printf("Attendance\nRoll no\n");

        for(int i=0;i<n;i++){
            if(*(rollno + i) == -1){
                printf("%d -\t Absent\n",i+1);
            }
            else{
                printf("%d -\t Present\n",i+1);
            }
        }
        status = shmctl(shmid, IPC_RMID, NULL); /* IPC_RMID is the command for destroying the shared memory*/
        if (status == 0) {
                fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
        }
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
int main(int argc,char* argv[]) {
        int status;
        pid_t pid=0;
        pid_t p1=0;
        key_t shmkey;
	
        n = atoi(argv[1]); //roll no entered by user
	int i;

        sighandler_t shandler;

        shandler =  signal(SIGINT, releaseSHM);

        shmkey = ftok("/home/linux/Desktop/OSLab/temp", 1);

//        printf("%d\n",(int)shmkey);

        shmid =  shmget(shmkey, 4*sizeof(int), IPC_CREAT | 0777);

        if (shmid == -1) {
            perror("shmget() failed: ");
            exit (1);
        }

        printf("shmget() returns shmid = %d.\n", shmid);

	rollno = shmat(shmid, NULL, 0);
        if (rollno == (void *)-1) {
            perror("shmat() failed");
            exit (1);
        }
        for(i=0; i<n; i++){
            *(rollno+i)=-1;
        }
	while(1);

	shmdt((void*)rollno);
        shmctl(shmid,IPC_RMID,NULL);
        exit(0);
}

