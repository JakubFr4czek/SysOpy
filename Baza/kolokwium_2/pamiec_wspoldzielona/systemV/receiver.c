#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 0x1234
#define MAX_SIZE 1024

int main(int argc, char **argv)
{
    sleep(1);
    int val = 0;

    // Getting the shared memory segment
    int shmid = shmget(SHM_KEY, sizeof(int), 0644 | IPC_CREAT);

    // Attaching to the shared memory
    int *addr = (int *)shmat(shmid, NULL, 0);

    // Copy the value from the shared memory to the variable
    memcpy(&val, addr, sizeof(int));

    // Detaching from the shared memory
    shmdt(addr);

    // Mark the segment to be destroyed
    shmctl(shmid, IPC_RMID, NULL);

    printf("%d square is: %d \n", val, val * val);
    return 0;
}
