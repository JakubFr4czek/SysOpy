#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <signal.h>
#include <string.h>

// Tile coordinates
typedef struct{
    
    int i;
    int j;

} Tile;

// Thread arguments
typedef struct{

    char *src;
    char *dst;
    Tile* tiles;
    int number_of_tiles;

} Thread_args;

// Function that updates the grid
void* thread_update_grid(void *args_void){

	Thread_args* args = (Thread_args*)args_void;

    char *src = args->src;
    char *dst = args->dst;

	while(1){

		pause(); // Waiting for signal from main thread

		for(int i = 0; i < args->number_of_tiles; i += 1){

			int row = args->tiles[i].i;
			int col = args->tiles[i].j;

			dst[row * GRID_WIDTH + col] = is_alive(row, col, src);

		}

		//char *tmp = src;
		//src = dst;
		//dst = tmp;

	}

	return NULL;

}

// Function that initializes worker threads
pthread_t* init_worker_threads(int n, char* src, char* dst, pthread_t* threads, Thread_args* args){
    
    // Assigning foreground and background grids to the arguments
    for (int i = 0; i < n; ++i){

        args[i].src = src;
        args[i].dst = dst;
        args[i].tiles = malloc(sizeof(Tile) * (int)ceil(GRID_HEIGHT * GRID_WIDTH / n));
        args[i].number_of_tiles = 0;

    }

    //Dividing work among all threads
    int current_arg = 0;

    for (int i = 0; i < GRID_HEIGHT; ++i){

        for (int j = 0; j < GRID_WIDTH; ++j){
			
            Tile tile = {i, j};

			args[current_arg].tiles[args[current_arg].number_of_tiles] = tile;
			args[current_arg].number_of_tiles++;

			current_arg = (current_arg + 1) % n;

        }
    }

	for (int i = 0; i < n; ++i){

		pthread_create(&threads[i], NULL, thread_update_grid, &args[i]);

	}

	return threads;

}

// Function that frees thread arguments
void free_thread_args(Thread_args* args, int n){

	for(int i = 0; i < n; i += 1){

		free(args[i].tiles);

	}

}

// Function that kills threads
void kill_threads(pthread_t* threads, int n){

	for(int i = 0; i < n; i += 1){

		pthread_cancel(threads[i]);

	}

}

// Function that handles signals
void handle_signal(int signum){

	// Do nothing
}

int main(int argc, char** argv){

	// Checking the number of arguments
	if(argc != 2){
		printf("Usage: ./life <number of threads>\n");
		exit(EXIT_FAILURE);
	}

	// Number of threads
	int n = atoi(argv[1]);
	
	// Setting up ncurses
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	// Creating grids
	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp = create_grid();

	// Initial state of the grid
	init_grid(foreground);

	// Creating threads and their arguments
	pthread_t threads[n];
    Thread_args args[n];

	// Initializing threads
	init_worker_threads(n, foreground, background, threads, args);

	// Handling signals
	signal(SIGUSR1, handle_signal);

	while (true){

		// Drawing the grid
		draw_grid(foreground);

		// Notyfing threads to update the grid
		for(int i = 0; i < n; i += 1){
			pthread_kill(threads[i], SIGUSR1);
		}

		// Waiting for threads to finish
		usleep(500 * 1000);

		// Swapping foreground and background
		memcpy(tmp, foreground, sizeof(char) * GRID_WIDTH * GRID_HEIGHT);
		memcpy(foreground, background, sizeof(char) * GRID_WIDTH * GRID_HEIGHT);
		memcpy(background, tmp, sizeof(char) * GRID_WIDTH * GRID_HEIGHT);

	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);
	destroy_grid(tmp);
	free_thread_args(args, n);
	kill_threads(threads, n);

	return 0;
}
