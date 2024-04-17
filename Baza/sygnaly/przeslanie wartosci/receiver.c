#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void handler(int sig, siginfo_t *info, void *ucontext){

    printf("Receiver: %d\n", info->si_value.sival_int);

}


int main(){

    printf("Receiver pid: %d\n", getpid());

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);

    pause();

}