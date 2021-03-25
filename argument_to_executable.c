#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc,char **argv){

        id_t pid;
	int i;
//	char* argvs[] = { "./hello ./no ./yes", NULL };
	
//	printf("%s",argv[2]);

        for(i=1; i<argc; i++)
	{
		pid=fork();

                if(pid == 0)
		{
                        if(i==1)
                                execve(argv[1],argv,NULL);
                        if(i==2)
                                execve(argv[2],argv,NULL);
			if(i==3)
				execve(argv[3],argv,NULL);
                }
                wait(NULL);
        }
        return 0;
}

