#define 	W 		50
#define 	H 		W/2
#define		MAX_AGENTS 	200

struct point { int8_t x, y; };

struct life
{
	struct point agents[MAX_AGENTS];
	uint8_t living_agents;
	uint16_t iteration;
	bool matrix[W * H];
};

struct life *init_life()
{
	struct life *game = (struct life *) malloc(sizeof(struct life));
	struct point null;
	null.x = -1; 
	null.y = -1;

	memset(game->matrix, false, W * H);

	for(uint8_t i = 0; i < MAX_AGENTS; i++)
		game->agents[i] = null;

	game->living_agents = 0;
	game->iteration = 0;
}

void init_tui()
{
	initscr();
	cbreak();
	noecho();
	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	curs_set(0);
}

uint8_t in_proximity(struct life *game, struct point at)
{
	uint8_t found = 0;
	
	for(int8_t at_y = at.y - 1; at_y <= at.y + 1; at_y++)
	{
		if(at_y < 0 || at_y >= H)
			continue;	

		for(int8_t at_x = at.x - 1; at_x <= at.x + 1; at_x++)
		{
			if((at_x < 0 || at_x >= W) || (at_x == at.x && at_y == at.y))
				continue;
				
			if(game->matrix[at_x + W * at_y] == true)
				found++;
		}
	}
	
	return found;
}

bool in_list(struct life *game, struct point at)
{
	for(uint8_t i = 0; i < MAX_AGENTS; i++)
	{	
		if(game->agents[i].x == at.x && game->agents[i].y == at.y)
			
			return true;
	}

	return false;
}

void sort_list(struct life *game)
{
	struct point copy[MAX_AGENTS];
	struct point null;
	uint8_t cidx = 0;
	
	null.x = -1;
	null.y = -1;

	for(uint8_t i = 0; i < MAX_AGENTS; i++)
	{
		if(!(game->agents[i].x == -1 && game->agents[i].y == -1))
			copy[cidx++] = game->agents[i];
	}

	for(uint8_t i = cidx; i < MAX_AGENTS; i++)
		copy[i] = null;

	for(uint8_t i = 0; i < MAX_AGENTS; i++)
		game->agents[i] = copy[i];
}

void add_agent(struct life *game, struct point at)
{
	if(game->living_agents >= MAX_AGENTS)
		return;

	struct point alive;
	alive.x = at.x;
	alive.y = at.y;

	game->agents[game->living_agents++] = alive;
	game->matrix[at.x + W * at.y] = true;
}

void del_agent(struct life *game, struct point at)
{
	struct point null;
	null.x = -1;
	null.y = -1;

	for(uint8_t i = 0; i < MAX_AGENTS; i++)
	{	
		if(game->agents[i].x == at.x && game->agents[i].y == at.y)
		{
			game->matrix[at.x + W * at.y] = false;
			game->agents[i] = null;
			game->living_agents--;
			sort_list(game);
		}
	}
}

void load_matrix_file(struct life *game)
{
	struct point add;
	FILE *m_file;

	char fchr;
	char xstr[3],
	     ystr[3];

	uint8_t idx = 0, mode = 1;

	m_file = fopen("data/matrix", "r");
	
	while((fchr = fgetc(m_file)) != EOF)
	{		
		if(fchr == ',')
		{
			ystr[idx] = '\0';
			add.y = atoi(ystr);
				
			idx = 0;
			mode = 2;
			continue;
		
		} else if(fchr == '\n') {
			
			xstr[idx] = '\0';
			add.x = atoi(xstr);
			
			add_agent(game, add);
			
			idx = 0;	
			mode = 1;
			continue;
		}
		
		if(fchr >= '0' && fchr <= '9')
		{
			if(mode == 1)
			
				ystr[idx++] = fchr;
		
			else if(mode == 2)

				xstr[idx++] = fchr;
		}
	}

	fclose(m_file);
}

void update(struct life *game)
{
	struct point dying[80], 
		     alive[80];

	uint8_t aidx, didx = 0;

	for(int8_t y = 0; y < H; y++)
	{
		for(int8_t x = 0; x < W; x++)
		{
			struct point at;
			at.x = x;
			at.y = y;

			switch(in_proximity(game, at))
			{
				case 0:
				case 1:
					if(in_list(game, at))
						
						dying[didx++] = at;
					
					break;
				
				case 2:
					break;
				
				case 3:
					if(!in_list(game, at))
						
						alive[aidx++] = at;
					
					break;
				
				default:
					if(in_list(game, at))
						
						dying[didx++] = at;
					
					break;
			}
		}
	}
	
	for(uint8_t i = 0; i < didx; i++)
	
		del_agent(game, dying[i]);
	
	for(uint8_t i = 0; i < aidx; i++)

		add_agent(game, alive[i]);

	game->iteration++;
}

void output(struct life *game)
{
	clear();
	
	char mat_buff[2048], 
	     mat_info[1024];
	
	memset(mat_buff, 0, 2048);	
	memset(mat_info, 0, 1024);	

	for(uint8_t y = 0; y < H; y++)
	{
		for(uint8_t x = 0; x < W; x++)
		{
			if(game->matrix[x + W * y] == true)
				
				strcat(mat_buff, "*");
			
			else
					
				strcat(mat_buff, ".");	
		}
		
		strcat(mat_buff, "\n");
	}

	sprintf(mat_info, "\nday: %u / 550\nalive: %u\n\nPress ctrl+c to exit.", game->iteration, game->living_agents);
	
	addstr(mat_buff);
	addstr(mat_info);

	refresh();
	usleep(10000);
}
