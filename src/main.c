#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#include "life.h"

int main(int argc, char *argv[])
{
	struct life *game = init_life();
	load_matrix_file(game);
	
	init_tui();
	
	do {
		output(game);
		update(game);

	} while (game->living_agents > 0 && game->iteration <= 550);

	free(game);
	endwin();

	printf("https://www.youtube.com/watch?v=E8kUJL04ELA\n");

	return 0;
}
