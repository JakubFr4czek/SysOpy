#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/*
1) program tworzy dwa procesy potomne. Nastepnie proces macierzysty co sekund�
wysy�a SIGUSR1 do procesu potomnego 1. Proces potomny 1 po otrzymaniu sygna�u
przesy�a kolejn� liczb� (rozpoczynajac od zera) przez potok nienazwany do
procesu potomnego 2, kt�ry wyswietla te liczbe.

2) Po wcisnieciu ctrl-c (SIGINT) powinno nastapic przerwanie wysy�ania sygnalow.
Powtorne wcisniecie ctrl-c powinno wznowic wysylanie sygnalow*/

int fd[2];
int nums = 0;

void handle_sigusr1(){

  write(fd[1], &nums, sizeof(nums));
  nums += 1;  

}

int main (int lpar, char *tab[]){
 
  pipe(fd);

  pid_t child_1 = fork();
  
  if(child_1 == 0){ //child_1

    //zamykam odczyt z potoku
    close(fd[0]);

    signal(SIGUSR1, handle_sigusr1);

    //Trzeba otrzymać sygnał
    while(1){
      
      pause();

    }

    close(fd[1]);

    return 0;

  }else{ //parent

    pid_t child_2 = fork();

    if(child_2 == 0){ //child_2

      //Zamykam zapis do potoku
      close(fd[1]);

      int liczba;

      while(1){
      read(fd[0], &liczba, sizeof(liczba));
      printf("child_2: %d\n", liczba);
      }

      close(fd[0]);

      return 0;

    }

  }

  //macierzysty
  
  //zamykam potok
  close(fd[0]);
  close(fd[1]);

  while(1){

    kill(child_1, SIGUSR1);
    printf("wyslano SIGUSR1\n");
    sleep(1);

  }

}