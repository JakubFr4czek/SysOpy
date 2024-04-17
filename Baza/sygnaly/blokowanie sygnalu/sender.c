#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if(argc != 2){

        perror("Wrong number of arguments!");
        exit(EXIT_FAILURE);

    }

    kill(atoi(argv[1]), SIGUSR1); //passed

    kill(atoi(argv[1]), SIGUSR2); //blocked

}