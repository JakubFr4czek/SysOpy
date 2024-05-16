#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include<semaphore.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SEMAPHORE_NAME "sem_1"

int main(){

    int n = 0; // number of users
    int m = 0; // number of printers

    sem_t sem_addr = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1);

    printf("Udalo sie\n");


}