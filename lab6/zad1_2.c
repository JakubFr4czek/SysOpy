#include <stdio.h>
#include <stdlib.h>
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

    double integral = integrate(f, 0, 1, atoi(argv[1]));

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Integral: %f, Time: %f\n", integral, time_spent);

}