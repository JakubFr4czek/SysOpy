#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
    char * fifo_in = "fifo_in";
    char * fifo_out = "fifo_out";

    int fd_in = open(fifo_in, O_RDONLY);
    int fd_out = open(fifo_out, O_WRONLY);

    double a;
    double b;
    int n;

    read(fd_in, &a, sizeof(double));
    read(fd_in, &b, sizeof(double));
    read(fd_in, &n, sizeof(int));

    close(fd_in);

    printf("%f %f %d\n", a, b, n);

    double integral = integrate(f, a, b, n);

    write(fd_out, &integral, sizeof(double));

    close(fd_out);

    //printf("Integral: %f\n", integral);

}