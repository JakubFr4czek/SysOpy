#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int fd[2];

int main(){

    

    //fd[0] - reading
    //fd[1] - writing

    pipe(fd); //Tworzenie potoku

    pid_t child = fork();

    if(child == 0){ //child

        int data;

        close(fd[1]); //Important, child will only read

        read(fd[0], &data, sizeof(data));

        printf("Child: %d\n", data); fflush(stdin);
            
        close(fd[0]);
        return 0;

    }else{ //parent

        close(fd[0]);

        int data = 777;

        write(fd[1], &data, sizeof(data));

        close(fd[1]);
        
    }

    
    wait(NULL);

    return 0;

}