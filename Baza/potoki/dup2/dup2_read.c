#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

    int fd[2];

    pipe(fd);

    pid_t child = fork(); //child process which will run grep

    if(child == 0){

        close(fd[1]);

        dup2(fd[0], STDIN_FILENO);

        execlp("grep", "grep", "SECRET", NULL);

        // No need to close fd[0] here as execlp replaces the current process image
        // with a new process image, so all file descriptors are automatically closed

    }else{ // Send data to grep

        close(fd[0]);

        char* sc = "SECRET: Earth is actually round!"; //You cant put filename here, because grep only knows input from this pipe

        write(fd[1], sc, sizeof(sc) - 1);

        close(fd[1]);

    }

    wait(NULL); // Wait for child process to finish

    return 0;
}
