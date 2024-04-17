#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

static void handle_signal(int signum, siginfo_t *info, void *context){

    printf("Wartość: %d", info->si_value.sival_int);
    exit(EXIT_SUCCESS);

}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    //struct sigaction action;
    //action.sa_sigaction = &sighandler;
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    //memset(&act, 0, sizeof(act));
    act.sa_sigaction = &handle_signal;
    sigemptyset(&act.sa_mask);
    

    //..........


    int child = fork();
    if(child == 0) {
        //printf("Skubana pompuj rap\n");
        sigset_t mask; 
        sigfillset(&mask); 
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGINT); // So ctr+c would work
        sigprocmask(SIG_SETMASK, &mask, NULL);

        sigaction(SIGUSR1, &act, NULL); //Intercepting SIGUSR1 signal

        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        //exit(EXIT_SUCCESS);

        sigsuspend(&mask); // Wait for SIGUSR1 signal

    }

    else {

        union sigval value; // Contains value field and pointer field
        value.sival_int = atoi(argv[1]);
        sleep(1);
        sigqueue(child, SIGUSR1, value);

        /*if (strcmp(argv[2], "SIGUSR1") == 0){
            if(sigqueue(child, SIGUSR1, value) == -1){
                perror("Sigqueue error");
                exit(EXIT_FAILURE);
            }
        }else{
            perror("Wrong signal");
            exit(EXIT_FAILURE);
        }*/
        //printf("potem pierwszy raz\n");
        
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
