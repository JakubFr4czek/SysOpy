#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void sighandler(int signo, siginfo_t *info, void *context) {
    printf("Signal %d received from parent with value %d\n", signo, info->si_value.sival_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }


    int child = fork();
    if(child == 0) {

        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGINT);

        sigprocmask(SIG_SETMASK, &mask, NULL);

        struct sigaction action;
        action.sa_sigaction = &sighandler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_SIGINFO;

        sigaction(SIGUSR1, &action, NULL);

        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        pause();
        return 0;
    }
    else {
        sleep(1);
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
        union sigval value;
        value.sival_int = atoi(argv[1]);
        sigqueue(child, SIGUSR1, value);

    }

    wait(NULL);

    return 0;
}
