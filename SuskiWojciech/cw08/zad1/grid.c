#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while(0)

const int grid_width = 30;
const int grid_height = 30;
pthread_t* threads;
struct thread_info* t_info;
char* bg;
char* fg;

struct thread_info {
	int row;			/* row specifing current cell */
	int col;			/* col specifing current cell */
	char *background;	/* grid from previous step */
	char *foreground; 	/* grid to draw */
};

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
//void signal_handler(int sig_no, siginfo_t* info, void* context) {}


void *cell(void* arg) {
	
	struct thread_info* t_info2 = arg;
	signal(SIGUSR1, signal_handler);
	while(1) {
		fg[t_info2->row * grid_width + t_info2->col] = is_alive(t_info2->row, t_info2->col, bg);
		pause();
		
		//char *tmp = bg;
		//bg = fg;
		//fg = tmp;
	}

}


void update_grid_with_multithread(char *src, char *dst) {
	threads = malloc(grid_width * grid_height * sizeof(pthread_t));
	//struct sigaction action;
	//sigemptyset(&action.sa_mask);
	//action.sa_sigaction = signal_handler;
	//sigaction(SIGUSR1, &action, NULL);
	t_info = malloc(sizeof(t_info));
	if (t_info == NULL) {
		handle_error("malloc");
	}
	bg = src;
	fg = dst;
	for (int i = 0; i < grid_height; ++i)
    {
		t_info->row = i;
        for (int j = 0; j < grid_width; ++j)
        {
			t_info->col = j;
			pthread_create(threads + i*grid_width + j, NULL, cell, (void*) t_info);
        }
    }

}

void update_grid(char* src, char* dst) {	
	bg = src;
	fg = dst;
	for(int i = 0; i < grid_width * grid_height; i++) {
		t_info->row = i / grid_width;
		t_info->col = i % grid_height;	
		pthread_kill(threads[i], SIGUSR1);
	}
	src = fg;
	dst = bg;

}

void free_threads() {
	free(threads);
}
