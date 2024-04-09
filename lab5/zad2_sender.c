#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    pid_t catcher_pid = (pid_t)atoi(argv[1]);

    union sigval value;
    value.sival_int = 47;

    sigqueue(catcher_pid, SIGUSR1, value);
    

    return 0;
}