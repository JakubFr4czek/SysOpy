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
    if (argc != 2)
    {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    int val = atoi(argv[1]);

    // Getting the shared memory segment
    int shmid = shmget(SHM_KEY, sizeof(int), 0644 | IPC_CREAT);

    // Attaching to the shared memory
    int *addr = (int *)shmat(shmid, NULL, 0);

    // Copy the value to the shared memory
    memcpy(addr, &val, sizeof(int));

    // Detaching from the shared memory
    shmdt(addr);

    return 0;
}
