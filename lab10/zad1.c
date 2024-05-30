#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include <unistd.h>
#include<stdbool.h>

#define REINDEERS 9
#define ROUNDS 4

int reindeers_waiting = 0;
int rounds = 1;
bool santa_sleeping = true;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;


void* santa_handler(void* args){

    while(1){

        printf("Mikołaj: Zasypiam\n");

        pthread_mutex_lock(&mutex);

        while(reindeers_waiting < REINDEERS){
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        printf("Mikołaj: Budzę się\n");
        printf("Mikołaj: Dostarczam zabawki\n");  

        reindeers_waiting = 0;

        santa_sleeping = false;
        pthread_cond_broadcast(&cond);

        pthread_mutex_unlock(&mutex);  

        // Santa sleeps for 2-4 seconds
        int sleep_time = rand() % 3 + 2;
        sleep(sleep_time);

        pthread_mutex_lock(&mutex);

        santa_sleeping = true;
        pthread_cond_broadcast(&cond);

        rounds += 1;

        pthread_mutex_unlock(&mutex);

        if(rounds == ROUNDS) break;

    }

    pthread_exit(0);

}

void* reindeer_handler(void* args){

    int id = *(int*)args;

    while(1){

        printf("Renifer %d: Lecę na wakacje\n", id);

        // Reindeer is on vacation
        int vacation_time = rand() % 6 + 5;
        sleep(vacation_time);

        pthread_mutex_lock(&mutex);

        reindeers_waiting += 1;
         printf("Renifer %d: Czekam na mikołaja, w sumie czeka %d reniferów\n", id, reindeers_waiting    );

        if(reindeers_waiting == REINDEERS){

            printf("Renifer %d: Wybudzam Mikołaja\n", id);
            pthread_cond_broadcast(&cond);
            
        }

        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);

        while(santa_sleeping){
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        printf("Renifer %d: Dostarczam zabawki\n", id);

        pthread_mutex_lock(&mutex);

        while(!santa_sleeping){
            pthread_cond_wait(&cond, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        if (rounds == ROUNDS) break;

    }

    pthread_exit(0);

}

int main(){

    pthread_t santa;
    pthread_create(&santa, NULL, santa_handler, NULL);

    pthread_t reindeer[REINDEERS];
    for(int i = 0; i < REINDEERS; i += 1){
        int* id = (int*)malloc(sizeof(int));
        *id = i;
        pthread_create(&reindeer[i], NULL, reindeer_handler, (void*)id);
    }

    pthread_join(santa, NULL);
    for(int i = 0; i < REINDEERS; i += 1){
        pthread_join(reindeer[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond); 

}