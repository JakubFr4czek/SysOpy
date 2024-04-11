#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile signal_counter = 0; // In order for int to not be optimized to const
// Signal handler for SIGUSR1
void handle_signal(int signum, siginfo_t *info, void *context) {

    //printf("Catcher received SIGUSR1 signal from PID %d\n", info->si_pid);

    union sigval value;
    value.sival_int = 0; //0 signal stands for confirmation 
    sigqueue(info->si_pid, SIGUSR1, value);

    if(info->si_value.sival_int == 1){ //print numbers from 1 to 100

        signal_counter += 1;

        for(int i = 1; i <= 100; i += 1){
            printf("%d\n", i);
        }

    }else if(info->si_value.sival_int == 2){

        signal_counter += 1;

        printf("%d\n", signal_counter);

    }else if(info->si_value.sival_int == 3){

        signal_counter += 1;

        exit(EXIT_SUCCESS);

    }else if(info->si_value.sival_int == 4){

        printf("Invalid command!");

        exit(EXIT_FAILURE);

    }


}

int main() {

    printf("Catcher PID: %d\n", getpid()); 

    // Preparing a mask that block every signal except SIGUSR1
    sigset_t mask; // Setting up a mask for sigsuspend
    sigfillset(&mask); // Filling set with all posiible signals
    sigdelset(&mask, SIGUSR1); // Removing SIGUSR1 from set
    sigdelset(&mask, SIGINT); // So ctr+c would work

    // Setting up the signal handler for SIGUSR1
    struct sigaction act;
    act.sa_handler = handle_signal;
    act.sa_mask = mask;
    act.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &act, NULL); //Intercepting SIGUSR1 signal


    while (1) {
        sigsuspend(&mask); // Wait for SIGUSR1 signal
    }

    return 0;
}
