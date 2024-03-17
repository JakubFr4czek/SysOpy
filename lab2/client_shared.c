#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include"collatz.h"

int main(){

    srand(time(NULL)); 

    for(int i = 0; i < 3; i += 1){

        int number = rand();

        int iterations = test_collatz_convergence(number, 10000);

        printf("Liczba %d zbiega do 1 po %d iteracjach\n", number, iterations);

    }

}