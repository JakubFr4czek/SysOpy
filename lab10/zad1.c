#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include <unistd.h>

#define REINDEERS 9

int reindeers_waiting = 0;
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

        // Santa sleeps for 2-4 seconds
        int sleep_time = rand() % 3 + 2;
        sleep(sleep_time);

    }

    return NULL;

}

void* reindeer_handler(void* args){

    // Reindeer is on vacation
    int vacation_time = rand() % 6 + 5;
    sleep(vacation_time);

    pthread_mutex_lock(&mutex);

    reindeers_waiting += 1;

    if(reindeers_waiting == REINDEERS){
        pthread_cond_broadcast(&cond);
    }

    pthread_mutex_unlock(&mutex);

    printf("JO CZEKOŁ\n");

    return NULL;

}

int main(){

    pthread_t santa;
    pthread_create(&santa, NULL, santa_handler, NULL);

    pthread_t reindeer[REINDEERS];
    for(int i = 0; i < REINDEERS; i += 1){
        pthread_create(&reindeer[i], NULL, reindeer_handler, NULL);
    }

    while(1){
        
    }


}