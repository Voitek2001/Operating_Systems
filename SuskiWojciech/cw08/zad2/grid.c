#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)


const int grid_width = 30;
const int grid_height = 30;
pthread_t* threads;
struct thread_info* t_info;
char* bg;
char* fg;

typedef struct thread_info {
	int curr_index;		/* index specifing cell in our grid */
	int block_size;		/* how many cell (to the right) belong to this thread */
} thread_info;

char *create_grid()
{
	return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}



void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid)
{
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}


void signal_handler(int sig_no) {}


void *cell(void* arg) {
	
	thread_info* t_info2 = arg;
	signal(SIGUSR1, signal_handler);
	while(1) {
		int j = t_info2->curr_index + t_info2->block_size;
		for (int i = t_info2->curr_index; i < j; i++) {
			fg[i] = is_alive(i / grid_width, i % grid_height, bg);
		}
		pause();
		
	}

}


void update_grid_with_multithread(char *src, char *dst, int n_threads) {
	threads = malloc(n_threads * sizeof(pthread_t));
	bg = src;
	fg = dst;
	int block_size;
	int reminder;

	block_size = grid_width * grid_height / (n_threads);
	reminder = (grid_width * grid_height) % (n_threads);
		
	for(int i = 0; i < n_threads; i++) {
		thread_info* t_info = malloc(sizeof(thread_info));
		if (t_info == NULL) {
			handle_error("malloc");
		}

		t_info->block_size = block_size;
		t_info->curr_index = i*block_size;
		int s = pthread_create(threads + i, NULL, cell, (void*) t_info);
		if(s != 0) {
			handle_error_en(s, "creating");
		}

	}
	if (reminder != 0) {
		thread_info* t_info = malloc(sizeof(thread_info));
		t_info->curr_index = block_size * (n_threads - 1);
		t_info->block_size = block_size + reminder;
		pthread_create(threads + n_threads-1, NULL, cell, (void*) t_info);
	}
}

void update_grid(char* src, char* dst, int n_threads) {	
	bg = src;
	fg = dst;
	for(int i = 0; i < n_threads; i++) {
		pthread_kill(threads[i], SIGUSR1);	
	}

}

void free_threads() {
	free(threads);
}
