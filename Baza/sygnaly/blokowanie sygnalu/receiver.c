#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void handle1(int signum){

    printf("Got %d\n", signum);

}

void handle2(int signum){ //should not be called

    printf("Got %d\n", signum);

}

int main(){

    printf("Receiver pid: %d\n", getpid());

    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGINT); // so you could stop process from terminal

    sigprocmask(SIG_SETMASK, &mask, NULL);

    signal(SIGUSR1, handle1);

    signal(SIGUSR2, handle2);

    //Two signals will be sent

    pause();
    pause(); //should not intercep singal because of sigprocmask

}