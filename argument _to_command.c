#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>

char **arg2execve(char *arg)
{
	char **passarg;
	int word=1, c=0;
	int i=0, j=0;
	int l=strlen(arg);

	for(i=0; arg[i]!='\0' && arg[i]==' '; i++)
		word++;
	
	passarg = (char**)malloc(word*sizeof(char*));

	for(i=0; i<word; i++)
		passarg[i]=(char*)malloc(20);

	for(i=0; i<=l; i++)
	{
		if(arg[i]==' ')
		{
			passarg[c][j]='\0';
			c++;
			j=0;
		}
		else
		{
			passarg[c][j]=arg[i];
			j++;
		}
	}
	return passarg;
}

void main(int argc,char* argv[])
{
        pid_t pid;
        int n=argc-1;
//	printf("%s \n",*argv);
        for(int i=1; i<=n; i++){
                pid = fork();
                if(pid == 0){
                        char **argvs=arg2execve(argv[i]);
                        char target[]="/bin/";
                        execve(strcat(target,argvs[0]), argvs, NULL);
                        exit(0);
                }
	}

	for(int i=1;i<=n;i++)
		wait(NULL);
}


