#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        return 1;
    }

    pid_t catcher_pid = atoi(argv[1]); // Catcher process pid

    union sigval value; // Contains value field and pointer field
    value.sival_int = 47;

    // Sending SIGUSR1 signal to the catcher process
    sigqueue(catcher_pid, SIGUSR1, value); // Sending SIGUSR with sigval to catcher process

    return 0;
}
