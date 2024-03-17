#include <stdio.h>
#include<time.h>
#include <stdlib.h>
#include <dlfcn.h>

#ifndef DYNAMIC
#error DYNAMIC not defined
#endif

int main() {

    void *handle = dlopen("./libcollatz_shared.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    int (*test_collatz_convergence)(int, int);
    test_collatz_convergence = (int (*)(int, int))dlsym(handle, "test_collatz_convergence");

    const char *dlsym_error = dlerror();
    if (dlsym_error != NULL) {
        fprintf(stderr, "Error: %s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    srand(time(NULL)); 

    for(int i = 0; i < 3; i += 1){

        int number = rand();

        int iterations = (*test_collatz_convergence)(number, 10000);

        printf("Liczba %d zbiega do 1 po %d iteracjach\n", number, iterations);

    }

    dlclose(handle);

    return 0;
}
