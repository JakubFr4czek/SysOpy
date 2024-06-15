#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int signum){

    printf("Received %d\n", signum);

}

int main(){

    printf("Receiver pid: %d\n", getpid());

    signal(SIGUSR1, handler);

    pause();

}