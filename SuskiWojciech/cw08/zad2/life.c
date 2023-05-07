#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
	
	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments\nUsage: ./life NUMBER_OF_THREADS\n");
		exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode
	char *foreground = create_grid();
	char *background = create_grid();
	char *tmp;
	int n_threads = atoi(argv[1]);

	init_grid(foreground);
	update_grid_with_multithread(foreground, background, n_threads);

	while (true)
	{
		draw_grid(foreground);
		usleep(500 * 1000);

		// Step simulation
		update_grid(foreground, background, n_threads);
		tmp = foreground;
		foreground = background;
		background = tmp;
	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);

	return 0;
}
