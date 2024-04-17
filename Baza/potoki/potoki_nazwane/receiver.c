#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int data;

int main(){

    mkfifo("potok", 0666); //Należy wywołać tylko raz

    while(1){

        int descriptor = open("potok", O_RDONLY);

        read(descriptor, &data, sizeof(data));

        // NIEZMIERNIE WAŻNE !!!

        printf("From receiver: %d"\n, data);//fflush(stdout); 
        //printf("From receiver: %d", data); fflush(stdout); //ALBO

        close(descriptor);
    }


    return 0;

}
