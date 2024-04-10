#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler for SIGUSR1
void handle_signal(int signum, siginfo_t *info, void *context) {
    printf("Catcher received SIGUSR1 signal from PID %d\n", info->si_pid);

    union sigval value;
    value.sival_int = 0;

    sigqueue(info->si_pid, SIGUSR1, 0);

}

int main() {

    printf("Catcher PID: %d\n", getpid()); 

    // Preparing mask that block every signal except SIGUSR1
    sigset_t mask; // Setting up a mask for sigsuspend
    sigfillset(&mask); // Filling set with all posiible signals
    sigdelset(&mask, SIGUSR1); // Removing SIGUSR1 from set

    // Setting up the signal handler for SIGUSR1
    struct sigaction act;
    act.sa_handler = handle_signal;
    act.sa_mask = mask;
    act.sa_flags = 0;

    sigaction(SIGUSR1, &act, NULL); //Intercepting SIGUSR1 signal

    
    //sigaddset(&mask, SIGUSR1);

    while (1) {
        sigsuspend(&mask); // Wait for SIGUSR1 signal
    }

    return 0;
}
