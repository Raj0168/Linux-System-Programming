#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/sem.h> /* for semget(2) semop(2) semctl(2) */
#include <signal.h> /* for signal(2) kill(2) */
#include <errno.h> /* for perror */
#include <stdlib.h>
#include <semaphore.h>

typedef void (*sighandler_t)(int);

#define NO_SEM	1

#define P(s) semop(s, &Pop, 1);
#define V(s) semop(s, &Vop, 1);

struct sembuf Pop;
struct sembuf Vop;

int semid;

union semun {
        int val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
} setvalArg;

void releaseSHM(int signum)
{
        if(semctl(semid, 0, IPC_RMID, setvalArg)==0) // if semaphore is deleted/killed
                printf("Semaphore Deleted\n");
        kill(0, SIGKILL);
}

int main(int argc,char* argv[])
{
        signal(SIGINT, releaseSHM);
        key_t mykey;
//      pid_t pid;

        int status;

        // struct sembuf has the following fields 
        //unsigned short sem_num;  semaphore number 
        //short          sem_op;   semaphore operation 
        //short          sem_flg;  operation flags

        Pop.sem_num = 0;
        Pop.sem_op = -1;
        Pop.sem_flg = SEM_UNDO;

        Vop.sem_num = 0;
        Vop.sem_op = 1;
        Vop.sem_flg = SEM_UNDO;

        mykey = ftok(".", 1);
        if(mykey == -1) {
                perror("ftok() failed");
                exit(1);
        }

        //       int semget(key_t key, int nsems, int semflg);
        semid = semget(mykey, NO_SEM, IPC_CREAT | 0777);

        if(semid == -1) {
                perror("semget() failed");
                exit(1);
        }

        // int semctl(int semid, int semnum, int cmd, ...);
        status = semctl(semid, 0, SETVAL, setvalArg); //sets the values of the semaphores semid
        if(status == -1) {
                perror("semctl() failed");
                exit(1);
        }

/*      pid = fork();
        if (pid == -1) {
                perror("fork() failed");
                exit(1);
        }
*/

        for(;;)
        {
                if(semctl(semid, 0, GETVAL)==0) {
                        printf("I am consumer\n");
                        sleep(1);
                        V(semid);
                }
                if(semctl(semid, 0, GETVAL)==-1)
                        releaseSHM(1);
        }
        return 0;
}

