#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>



int main(){

    //Apparently this wont work:
    //printf("%d", getppid());
    //but this will
    printf("My ppid: %d\n", getppid());



    sigset_t mask;
    sigemptyset(&mask); //clears mask
    sigaddset(&mask, SIGUSR1); //adds SIGUSR1 to mask

    sigsuspend(&mask); //stops program until it recives SIGUSR1

    printf("aha");

    return 0;
}