#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

//https://stackoverflow.com/questions/25261/set-and-oldset-in-sigprocmask

void handle_signal(int signum) {
    printf("Handling SIGUSR1 with signal function...\n");
}

void sigaction_signal_handler(int signum) {
    printf("Handling with sigaction function...\n");
}

int main(int argc, char *argv[]) {

    // Expecting 1 argument from command line
    if (argc != 2) {
        printf("Wrong number of arguments! Program only accepts one argument!\n");
        return 1;
    }

    if (strcmp(argv[1], "none") == 0) { // signal will be handled in default way
        signal(SIGUSR1, SIG_DFL);
    }
    else if (strcmp(argv[1], "ignore") == 0) { // signal will be ignored
        signal(SIGUSR1, SIG_IGN);
    }
    else if (strcmp(argv[1], "handle") == 0) { // signal will be handled in custom way, by executing handler function
        signal(SIGUSR1, handle_signal);
    }
    else if (strcmp(argv[1], "mask") == 0) { // signal will be masked

        // Block SIGUSR1
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
            perror("sigprocmask");
            return 1;
        }

        raise(SIGUSR1);

        // checks if blocked signal is visible //TODO: nie dziala
        sigset_t sigset;

        if (sigpending(&sigset) != 0) {
            perror("sigpending() error");
        }
        else if (sigismember(&sigset, SIGUSR1)) {
            printf("SIGUSR1 signal is pending\n");
        }
        else {
            printf("no signals are pending\n");
        }
    }
    else if (strcmp(argv[1], "") == 0) {

        signal(SIGUSR1, SIG_IGN);
        printf("No argument provided! You can provide {none, ignore, handle, mask}\n");
    }
    else {

        signal(SIGUSR1, SIG_IGN);
        printf("Wrong argument provided!\n");
    }

    // sends SIGUSR1 to itself
    raise(SIGUSR1);

    return 0;
}
