#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>

void* hello(void* arg){
        
        sleep(1);
        while(1){
                printf("Hello world from thread number %d\n", *(int*)arg);
		/****************************************************
			przerwij dzialanie watku jesli bylo takie zadanie
		*****************************************************/     
                pthread_exit(0);           
                printf("Hello again world from thread number %d\n", *(int*)arg);
                sleep(2);
        }
        return NULL;
}


int main(int argc, char** args){

       if(argc !=3){
	    printf("Not a suitable number of program parameters\n");
    	return(1);
   	}

        int n = atoi(args[1]);

	/**************************************************
	    Utworz n watkow realizujacych funkcje hello
	**************************************************/

        pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * n);

        int *values = (int*)malloc(sizeof(int) * n);

        for(int i = 0; i < n; i += 1){

                memcpy(&values[i], &i, sizeof(int));
                pthread_create(&threads[i], NULL, hello, (void*)&values[i]);

        } 
        

        int i = 0;
        while(i++ < atoi(args[2])) {
                printf("Hello from main %d\n",i);
                sleep(2);
        }
        
        i = 0;

	/*******************************************
	    "Skasuj" wszystke uruchomione watki i poczekaj na ich zakonczenie
	*******************************************/
        printf("DONE");

        for(int i = 0; i < n; i += 1){

                pthread_join(threads[i], NULL);

        }

        free(threads);

        return 0;
}

