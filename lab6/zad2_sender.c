#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){

    if(argc != 4) exit(EXIT_FAILURE);

    //fifo path
    char* fifo_in = "fifo_in";
    char* fifo_out = "fifo_out";
    char* catcher = "zad2_catcher";


    mkfifo(fifo_in, 0666);
    mkfifo(fifo_out, 0666); 

    pid_t pid = fork();

    if (pid == 0) {
        execl(catcher, catcher, NULL);
        exit(EXIT_SUCCESS);
    }else if (pid < 0) {
        perror("Failed to fork");
        exit(1);
    }

    int fd_in = open(fifo_in, O_WRONLY);

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = atoi(argv[3]);

    write(fd_in, &a, sizeof(double));
    write(fd_in, &b, sizeof(double));
    write(fd_in, &n, sizeof(int));

    close(fd_in);

    int fd_out = open(fifo_out, O_RDONLY);

    double integral;

    read(fd_out, &integral, sizeof(double));

    close(fd_out);

    wait(NULL);

    printf("Integral: %f\n", integral);
}