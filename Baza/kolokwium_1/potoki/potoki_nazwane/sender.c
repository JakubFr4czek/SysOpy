#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

int data;

void send(){

    int descriptor = open("potok", O_WRONLY);

    write(descriptor, &data, sizeof(data));

    close(descriptor);

    return;

}

int main(){

    signal(SIGTSTP, send);

    data = 777;

    while(1){


        if(pause() == -1)
            printf(" SIGTSTP sent!\n");

    }

    return 0;

}
