﻿#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>


#define MaxItems 5 		// Maksymalna liczba elementow, które producent może wyprodukować lub konsument może skonsumować
#define BufferSize 5	// Rozmiar bufora

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{   
    int item;
    for(int i = 0; i < MaxItems; i++) {
        item = rand(); 			// Wygeneruj losowy element
        ...
        ...
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BufferSize;
        ...
        ...		
    }
}

void *consumer(void *cno)
{   
    for(int i = 0; i < MaxItems; i++) {
        ...
        ...
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BufferSize;
        ...
        ...
    }
}

int main()
{   

    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[5] = {1,2,3,4,5}; 

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
         ...
    }
    for(int i = 0; i < 5; i++) {
        ...
    }

    ...
    ...
    ...	

    return 0;
    
}