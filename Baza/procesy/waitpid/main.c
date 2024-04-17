#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

    pid_t child = fork();

    if(child == 0){

        //sleep(5);
        exit(EXIT_SUCCESS);

    }else{
        
        sleep(2);

        int status;

        pid_t res = waitpid(child, &status, WNOHANG);

        if(res == 1){

            perror("Błąd waitpid\n");
            exit(EXIT_FAILURE);

        }else if(res == 0){

            printf("Proces potomny nadal pracuje\n");

        }else{

            printf("Proces potomny zakończył działanie\n");

        }

    }

    return 0;

}