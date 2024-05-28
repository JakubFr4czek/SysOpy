#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/wait.h>

#define QUEUE_SIZE 20
#define TEXT_LENGTH 3

#define SEMAPHORE_SIZE "LAB8_semaphore_queue_space"
#define SEMAPHORE_TASKS "LAB8_SEMAPHORE_TASKS"

#define SHARED_MEMORY_PATH "LAB8_SHARED_MEMORY"
#define SHARED_MEMORY_SIZE QUEUE_SIZE * TEXT_LENGTH * sizeof(char)

#define DESCRIPTIVE_PRINTER false


char* sample_text();
void print_text(char*, int, int);
void handle_sigint();

typedef struct{

    char* text[QUEUE_SIZE][TEXT_LENGTH];
    int head;
    int tail;
    int users[QUEUE_SIZE];

}Queue;

int n = -1; // number of users
int m = -1; // number of printers
sem_t *semaphore_queue_space;
sem_t *semaphore_tasks;
Queue* queue;

int main(int argc, char** argv){

    if(argc != 3){
        printf("Usage: ./zad1 <number of users> <number of printers>\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    m = atoi(argv[2]);

    srand(time(NULL));

    // Creating shared memory segment
    int memory_descriptor = shm_open(SHARED_MEMORY_PATH, O_RDWR | O_CREAT, 0);

    // Could not create shared memory
    if(memory_descriptor == -1){
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Setting shared memory size
    if(ftruncate(memory_descriptor, SHARED_MEMORY_SIZE) == -1){
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Joining shared memory
    queue = (Queue*)mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memory_descriptor, 0);
    queue->head = 0;
    queue->tail = 0;

    semaphore_queue_space = sem_open(SEMAPHORE_SIZE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, QUEUE_SIZE);
    semaphore_tasks = sem_open(SEMAPHORE_TASKS, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 0);

    // Setting semaphore size to QUEUE_SIZE
    if(sem_init(semaphore_queue_space, 1, QUEUE_SIZE) == -1){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Setting semaphore size to 0
    if(sem_init(semaphore_tasks, 1, 0) == -1){
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Creating printers
    for(int i = 0; i < m; i += 1){

        if(fork() == 0){ // in child process
            
            while(1){
                
                // int value;
                // sem_getvalue(semaphore_tasks, &value);
                // printf("Semaphore printer: %d\n", value);

                // Waiting for tasks
                if(sem_wait(semaphore_tasks) == -1){
                    perror("sem_wait");
                    exit(EXIT_FAILURE);
                }

                char* text = (char*)malloc(sizeof(char) * TEXT_LENGTH);
                memcpy(text, queue->text[queue->head], TEXT_LENGTH);
                queue->head = (queue->head + 1) % QUEUE_SIZE;

                print_text(text, i, queue->users[queue->head]);
                free(text);
                
                // Increasing queue space
                if(sem_post(semaphore_queue_space) == - 1){
                    perror("sem_wait");
                    exit(EXIT_FAILURE);
                }
            }

            exit(EXIT_SUCCESS);

        }

    } 

    // Creating clients
    for(int i = 0; i < n; i += 1){

        if(fork() == 0){

            while(1){

                // int value;
                // sem_getvalue(semaphore_tasks, &value);
                // printf("Semaphore client: %d\n", value);

                if(sem_wait(semaphore_queue_space) == -1){
                    perror("sem_wait");
                    exit(EXIT_FAILURE);
                }
                
                srand(time(NULL) ^ (getpid() << 16));
                char* text = sample_text();

                memcpy(queue->text[queue->tail], text, TEXT_LENGTH);
                queue->users[queue->tail] = i;
                queue->tail = (queue->tail + 1) % QUEUE_SIZE;

                free(text);

                // Increasing number of tasks
                if(sem_post(semaphore_tasks) == -1){
                    perror("sem_post");
                    exit(EXIT_FAILURE);
                }

                // Waiting for next task
                sleep(rand()%5 + 1);

            }

            exit(EXIT_SUCCESS);

        }
        
    }

    // ON PROGRAM EXIT
    signal(SIGINT, handle_sigint);

    pause();

}

// Creates sample text with size of TEXT_LENGTH
char* sample_text(){

    char* text = (char*)malloc(sizeof(char) * TEXT_LENGTH);
    for(int i = 0; i < TEXT_LENGTH; i += 1){
        text[i] = 97 + rand() % 26;
    }
    return text;

}

// Prints text character by character with 1 second delay
void print_text(char* text, int printer_id, int client_id){
    
    for(int i = 0; i < TEXT_LENGTH; i += 1){
        
        if(DESCRIPTIVE_PRINTER)
            printf("Printer %d printing for client %d: %c (Position in text: %d)\n", printer_id, client_id, text[i], i);
        else
            printf("Printer %d printing for client %d: %c\n", printer_id, client_id, text[i]);
        sleep(1);

    }

    //printf("Printer %d finished printind for printed for client %d\n", printer_id, client_id);

}

// Closes semaphores and shared memory
void handle_sigint(){

    // waiting for all children
    for(int i = 0; i < m + n; i += 1){
        wait(NULL);
    }

    //closing semaphore responsible for queue size
    if(sem_close(semaphore_queue_space) == -1){
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    //closing semaphore responsible for number of tasks
    if(sem_close(semaphore_tasks) == -1){
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    // Closing shared memory
    munmap(queue, SHARED_MEMORY_SIZE);

    if(shm_unlink(SHARED_MEMORY_PATH) == -1){
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}