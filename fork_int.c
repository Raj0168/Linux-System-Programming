#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(int argc, char *argv[])
{
        pid_t pid;
        int i, j, dn, fact=1;
        printf("argc = %d\n", argc);

        for(int i=0;i<argc;i++)
        {
        pid=fork();
        if(pid == 0)
        {
            printf("[son] pid: %d\n",getpid());
	    printf("[parent] ppid: %d\n",getppid());
            exit(0);
        }
    }

    for(int i=0;i<argc;i++)
        wait(NULL);

    dn=atoi(argv[argc-1]);
    printf("dn=%d\n",dn);

    for (j = 1; j <= dn; j++) {
        fact *= j;
    }
    printf("Factorial of dn = %d\n", fact);

        return 0;
}

