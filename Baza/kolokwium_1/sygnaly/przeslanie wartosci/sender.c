#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

    if(argc != 2){
        perror("Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    union sigval value;
    value.sival_int = 777;

    sigqueue(atoi(argv[1]), SIGUSR1, value);

}