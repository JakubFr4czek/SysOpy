#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv){
    if (argc == 2){
        char* filename1 = argv[1];
        int fd[2];
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
        //  zamknij deskryptor do zapisu i wykonaj program sort na filename1
        //  w przypadku błędu zwróć 3
        close(fd[1]);
        if(execl("/usr/bin/sort","sort", "file2", "-o", "file2", NULL)){
            return 3;
        }
		  
        //  koniec
        }else{
            close(fd[0]);
        }
    }
    else if (argc==3){
        char* filename1 = argv[1];
        char* filename2 = argv[2];
        int fd[2];
        //  otwórz plik filename2 z prawami dostępu rwx_r--_r--, 
        //  jeśli plik istnieje otwórz go i usuń jego zawartość
        
        mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH;
        int file = open(filename2, O_WRONLY | O_TRUNC, mode);

        //  koniec
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0){
        //  zamknij deskryptor do zapisu,
        //  przekieruj deskryptor standardowego wyjścia na deskryptor pliku filename2 i zamknij plik,
        //  wykonaj program sort na filename1
        //  w przypadku błędu zwróć 3.

        close(fd[1]);

        dup2(file, STDOUT_FILENO);

        close(file);

        if(execl("/usr/bin/sort","sort", filename1, NULL)){
            return 3;
        }

        //  koniec
        }else{
            close(fd[0]);
        }
    }
    else
        printf("Wrong number of args! \n");

    return 0;
}