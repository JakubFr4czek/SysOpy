#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]){

    if(argc != 2) return 1;

    int local = 0;

    //program prints its name
    printf("Program's name: %s\n", argv[0]);

    pid_t pid = fork();

    if(pid < 0){

        printf("Error creating child process!\n");
        return 2;

    }else if(pid == 0){ //child process

        printf("------- Child process -------\n");

        global += 1;
        local += 1;

        printf("child's pid = %d, parent's pid = %d\n", getpid(), getppid());

        printf("child's local = %d, child's global = %d\n", local, global);

        int error_code = execl("/bin/ls", "ls", argv[1], NULL);

        printf("-----------------------------\n");

        return error_code;

    }else{ //parent process

        int status;
        wait(&status); //waiting for child's process to finish

        printf("------- Parent process -------\n");

        printf("child's pid = %d, parent's pid = %d\n", pid, getpid());

        printf("Child exit code: %d\n", status);

        printf("Parent's local = %d, parent's global = %d\n", local, global);

        printf("------------------------------\n");

    }

}