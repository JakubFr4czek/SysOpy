#include "collatz.h"

int collatz_conjecture(int input){

    if(input % 2 == 0)
        return input / 2;
    else
        return 3 * input + 1;

}

int test_collatz_convergence(int input, int max_iter){

    int cnt = 0;

    while(max_iter > 0 && input != 1){

        input = collatz_conjecture(input);
        max_iter -= 1;
        cnt += 1;

    }

    return input == 1?cnt:-1; 

}