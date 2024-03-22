#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    
    if(argc != 2) return 1;

    int procesess_count = atoi(argv[1]);

    for(int i = 0; i < procesess_count; i += 1){
        
        if(fork() == 0){ //fork's return should always be 0 for parent process

            printf("Child's pid %d, parent's pid %d\n",getpid(),getppid());
            exit(0);

        }

    }

    for(int i = 0; i < procesess_count; i += 1){
         wait(NULL);
    }

    printf("Argv[1]: %s\n", argv[1]);

    return 0;

}
