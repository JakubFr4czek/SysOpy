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

    
    int descriptor = open("example_file", O_WRONLY | O_TRUNC);

    dup2(descriptor, STDOUT_FILENO);

    execlp("grep", "grep", "kot", "dup2_write.c", NULL);

    close(descriptor);

    return 0;

}