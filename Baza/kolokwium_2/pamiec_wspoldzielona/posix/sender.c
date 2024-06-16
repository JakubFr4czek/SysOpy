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
    
   if(argc !=2){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }
    shm_unlink(SHM_NAME);
    int val = atoi(argv[1]);

    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0644);

    // Ustawianie rozmiaru pamięci
    ftruncate(fd, sizeof(int));

    int *addr;

    addr = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    memcpy(addr, &val, sizeof(int));

    //munmap(addr, sizeof(int));

    //shm_unlink(SHM_NAME);

    return 0;
}
