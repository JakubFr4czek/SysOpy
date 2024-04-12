#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

double f(double x){
    return 4 / (x * x  + 1);
}

double integrate(double (*f)(double), double a, double b, int i) {

    double h = (b - a) / i;
    double integr = 0;
    double x;

    for (int j = 0; j < i; j++) {

        x = j * h + a;
        integr += h * f(x);

    }

    return integr;
}

int main(int argc, char* argv[]){

    if(argc != 2) exit(EXIT_FAILURE);

    double a = 0;
    double b = 1;

    int processes_count = atoi(argv[1]);

    int fd[processes_count][2];
    //pid_t children[processes_count];

    double integral = 0.0;

    for(int i = 0; i < processes_count; i +=1){

        double h = (b - a) / processes_count;

        pipe(fd[i]);

        if(fork() == 0){

            //printf("pid: %d\n", getpid());

            close(fd[i][0]);  //zamknij odczyt
            //printf("h: %f %f %f\n",h,  h * i, h * (i + 1));
            double result = integrate(f, h * i, h * (i + 1), 1);
            //printf("result: %f", result);
            write(fd[i][1], &result, sizeof(double));
            close(fd[i][1]);

            exit(EXIT_SUCCESS);

        }else{

            close(fd[i][1]);

            double to_read;

            read(fd[i][0], &to_read, sizeof(double));

            integral += to_read;
        }
    }

    //double res = integrate(f, a, b, atoi(argv[1]));

    /*for(int i = 0; i < processes_count; i += 1){
        wait(NULL);
    }*/


    printf("%f\n", integral);

}