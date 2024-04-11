#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Signal handler for SIGUSR1
void handle_signal(int signum, siginfo_t *info, void *context) {
    //printf("Sender received SIGUSR1 signal from PID %d\n", info->si_pid);

    if(info->si_value.sival_int == 0){ //0 stands for confirmations (catcher recived a command)

        printf("Catcher recived a command!\n");

    }else{

        exit(EXIT_FAILURE);
    }

}


int main(int argc, char *argv[]) {

    if (argc != 3) {
        return 1;
    }

    // Preparing a mask that block every signal except SIGUSR1
    sigset_t mask; // Setting up a mask for sigsuspend
    sigfillset(&mask); // Filling set with all posiible signals
    sigdelset(&mask, SIGUSR1); // Removing SIGUSR1 from set
    sigdelset(&mask, SIGINT); // So ctr+c would work

    // Setting up the signal handler for SIGUSR1
    struct sigaction act;
    act.sa_handler = handle_signal;
    act.sa_mask = mask;
    act.sa_flags = SA_SIGINFO; // important in order to make siginfo work

    sigaction(SIGUSR1, &act, NULL); //Intercepting SIGUSR1 signal

    pid_t catcher_pid = atoi(argv[1]); // Catcher process pid

    // --- Initial handshake ---

    union sigval value; // Contains value field and pointer field
    value.sival_int = atoi(argv[2]);

    // Sending SIGUSR1 signal to the catcher process
    sigqueue(catcher_pid, SIGUSR1, value); // Sending SIGUSR with sigval to catcher process

    sigsuspend(&mask);

    return 0;
}
