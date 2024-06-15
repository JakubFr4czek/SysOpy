#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){

    int n = 10;

    for(int i = 0; i < n; i += 1){

        pid_t child = fork();

        if(child == 0){ // child

            printf("Child pid %d\n", getpid());

            exit(EXIT_SUCCESS);

        }

        
    }

    for(int i = 0; i < n; i += 1){
        wait(NULL);
    }

    printf("Parent pid %d\n", getpid());

}