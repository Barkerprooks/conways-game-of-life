#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#include "life.h"

void usage(char c, char *a[])
{
	if(c > 2)
	{
		if(strcmp("--help", a[1]) == 0 || strcmp("-h", a[1]) == 0)
		{	
			printf("help: coming soon\n");
		}
		else
			printf("usage: %s -h or --help for options\n", a[0]);
		
		exit(0);	
	}
}

int main(int argc, char *argv[])
{
	struct life *game = init_life();
	load_matrix_file(game);
	
	usage(argc, argv);
	init_tui();
	
	do {
		output(game);
		update(game);

	} while (game->living_agents > 0);

	free(game);
	endwin();

	printf("https://www.youtube.com/watch?v=E8kUJL04ELA\n");

	return 0;
}
