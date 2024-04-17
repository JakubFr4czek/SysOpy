#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, char *argv[])
{
  if (argc!=2) {
     printf("Prawidłowe wywołanie %s liczba\n",argv[0]); 
     exit(EXIT_FAILURE);
  }
  pid_t child;
  int status;
  if((child = fork()) < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if(child == 0) {
    sleep(2);
    exit(EXIT_SUCCESS);
  }
  else {
/* Proces macierzysty usypia na liczbę sekund będącą argumentem programu
 * Proces macierzysty pobiera status  zakończenia potomka child, nie zawieszając swojej pracy. 
 * Jeśli proces się nie zakończył, wysyła do dziecka sygnał zakończenia.
 * Jeśli wysłanie sygnału się nie powiodło, proces zwróci błąd.
 * Jeśli się powiodło, wypisuje komunikat sukcesu zakończenia procesu potomka 
 * z numerem jego PID i sposobem zakończenia (Proces xx zakończony przez exit albo sygnałem). */

  sleep(atoi(argv[1]));


  pid_t res = waitpid(child, &status, WNOHANG);

  if(res == -1){ //błąd waitpid

    perror("Waitpid error!");
    exit(EXIT_FAILURE);

  }else if(res == 0){ //still working

    printf("Sending SIGKILL to %d\n", child);

    int k_val = kill(child, SIGKILL);

    if(k_val == -1){

      perror("Kill error!");
      exit(EXIT_FAILURE);

    }

  } //finished

  wait(NULL);

  if (WIFEXITED(status)) {
    printf("Proces %d zakończony przez exit\n", child);
  } else if (WIFSIGNALED(status)) {
    printf("Proces %d zakończony przez sygnał\n", child);
  }



 
 
 } //else
  return 0;
}
