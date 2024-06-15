#include <stdio.h>
#include <dlfcn.h>

int main(){

    void *handle = dlopen("./libbibl.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    int (*add)(int, int);
    add = (int (*)(int, int))dlsym(handle, "add");

    const char *dlsym_error = dlerror();
    if (dlsym_error != NULL) {
        fprintf(stderr, "Error: %s\n", dlsym_error);
        dlclose(handle);
        return 1;
    }

    printf("Wynik dodawania: %d\n", (*add)(2, 3));


}