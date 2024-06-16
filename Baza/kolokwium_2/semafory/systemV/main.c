#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>

#define FILE_NAME "common.txt"
#define SEM_KEY 1234  // Klucz semafora

// Funkcja pomocnicza do operacji na semaforach
void sem_op(int semid, int op) {
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main(int argc, char** args){

    if (argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return(1);
    }

    /**************************************************
    Stworz semafor systemu V
    Ustaw jego wartosc na 1
    ***************************************************/

    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }
    
    // Ustaw wartość semafora na 1
    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl");
        exit(1);
    }
     
    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    int parentLoopCounter = atoi(args[1]);
    int childLoopCounter = atoi(args[2]);
        
    char buf[100];
    pid_t childPid;
    int max_sleep_time = atoi(args[3]);
     
    if ((childPid = fork())) {
        int status = 0;
        srand((unsigned)time(0)); 

        while (parentLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);    
            
            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/
            sem_op(semid, -1);  // Zablokuj semafor
        
            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));
            
            sem_op(semid, 1);  // Odblokuj semafor
            /*********************************
            Koniec sekcji krytycznej
            **********************************/

        }
        waitpid(childPid, &status, 0);
    } else {
        srand((unsigned)time(0)); 
        while (childLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);                
            
            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/
            sem_op(semid, -1);  // Zablokuj semafor
            
            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            sem_op(semid, 1);  // Odblokuj semafor
            /*********************************
            Koniec sekcji krytycznej
            **********************************/

        }
        _exit(0);
    }
     
    /*****************************
    posprzataj semafor
    ******************************/
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }

    close(fd);
    return 0;
}
