#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <sys/mman.h>

#define SHM_NAME "/kol_shm"
#define MAX_SIZE 1024

int main(int argc, char **argv)
{

    sleep(1);
    int val =0;

    // Tworzenie segmentu pamięci wspólnej
    int fd = shm_open(SHM_NAME, O_RDWR, 0644);

    // Ustawianie rozmiaru pamięci
    ftruncate(fd, sizeof(int));

    int *addr;

    addr = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    memcpy(&val, addr, sizeof(int));

    munmap(addr, sizeof(int));

    shm_unlink(SHM_NAME);

	printf("%d square is: %d \n",val, val*val);
    return 0;
}
