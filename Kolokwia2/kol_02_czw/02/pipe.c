#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#define BUFFER_SIZE 10

int main(int argc, char const *argv[])
{
	int fd[...];
	pid_t pid;
	char sendBuffer	[BUFFER_SIZE]="welcome!";
	char receiveBuff[BUFFER_SIZE];
	// Wyzeruj bufor receiveBuff
    ...

	//Utwórz anonimowy potok
	if (...==-1)
	{
		fprintf(stderr, "... :%s\n",strerror(errno));
		exit(1);
	}

	//Utwórz proces potomny
	if ... <0)
	{
		fprintf(stderr, "... :%s\n",strerror(errno));
		exit(1);
	}else if (pid>0)	// proces nadrzędny
	{
        ...
        ...		
		printf("parent process %d send:%s\n", ..., ...);
	}else				// proces potomny
	{
		...
		...
		printf("child process %d receive:%s\n", ..., ...);
	}
	return 0;
}

/*
gcc -o pipe pipe.c 
./pipe 
parent process send:welcome!
child  process receive:welcome
*/
