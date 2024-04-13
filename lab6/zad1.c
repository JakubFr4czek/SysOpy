#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 

double f(double x){
    return 4 / (x * x  + 1);
}

double integrate(double (*f)(double), double a, double b, int n) {

    double h = (b - a) / (double)n;
    double res = 0;

    for (int i = 1; i <= n; i++) {

        res += h * f(a + i * h);

    }

    return res;
}

int main(int argc, char* argv[]){

    if(argc != 2) exit(EXIT_FAILURE);

    clock_t begin = clock();

    double a = 0;
    double b = 1;

    int processes_count = atoi(argv[1]);

    int fd[processes_count][2];

    double integral = 0.0;

    for(int i = 0; i < processes_count; i +=1){

        double h = (b - a) / processes_count;

        pipe(fd[i]);

        if(fork() == 0){

            close(fd[i][0]);  //zamknij odczyt

            double result = integrate(f, h * i, h * (i + 1), 1);

            write(fd[i][1], &result, sizeof(double));
            close(fd[i][1]);

            exit(EXIT_SUCCESS);

        }
    }

    for(int i = 0; i < processes_count; i += 1){

        close(fd[i][1]);

        double to_read;

        read(fd[i][0], &to_read, sizeof(double));

        close(fd[i][0]);

        integral += to_read;


    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Calculated integral: %f, time: %f\n", integral, time_spent);

}