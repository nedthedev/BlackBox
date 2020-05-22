#include <iostream>
#include <stdlib.h>
#include <ncurses.h>

#include "grid.h"

int grid::count = 0;

int error_found(grid *g)
{
	for (int y = 1; y <= SIZE * 4; y++)
	{
		if (check_ray(g, y) == -20)
		{
			return 1;
		}
	}
	return 0;
}

grid::grid(time_t &seed) : works(1), games(0), points(15), total_points(0), total_moves(0), wins(0), moves(0), playing(1), balls_placed(0)
{
	this->seed = seed;
	for (int i = 0; i < SIZE * 4 * 2; i++)
	{
		guesses[i] = 0;
	}
	for (int i = 0; i < SIZE + 2; i++)
	{
		for (int t = 0; t < SIZE + 2; t++)
		{
			user_map[i][t] = 0;
			ball_map[i][t] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int x, y;
		do
		{
			x = 1 + rand() % SIZE;
			y = 1 + rand() % SIZE;
		} while (ball_map[x][y] == -1);
		ball_map[x][y] = -1;
	}
	for (int i = 0; i < 8; i++)
	{
		user_map[i + 1][0] = i + 1;
		ball_map[i + 1][0] = i + 1;
	}
	int count = 9;
	for (int i = 0; i < 8; i++)
	{
		user_map[9][i + 1] = count;
		ball_map[9][i + 1] = count;
		count++;
	}
	for (int i = 8; i > 0; i--)
	{
		user_map[i][9] = count;
		ball_map[i][9] = count;
		count++;
	}
	for (int i = 8; i > 0; i--)
	{
		user_map[0][i] = count;
		ball_map[0][i] = count;
		count++;
	}
}

void grid::reset_grid()
{
	count = 0;
	points = 15;
	playing = 1;
	balls_placed = 0;
	moves = 0;
	for (int i = 0; i < SIZE * 4 * 2; i++)
	{
		guesses[i] = 0;
	}
	for (int i = 0; i < SIZE + 2; i++)
	{
		for (int t = 0; t < SIZE + 2; t++)
		{
			user_map[i][t] = 0;
			ball_map[i][t] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int x, y;
		do
		{
			x = 1 + rand() % SIZE;
			y = 1 + rand() % SIZE;
		} while (ball_map[x][y] == -1);
		ball_map[x][y] = -1;
	}
	for (int i = 0; i < 8; i++)
	{
		user_map[i + 1][0] = i + 1;
		ball_map[i + 1][0] = i + 1;
	}
	int count = 9;
	for (int i = 0; i < 8; i++)
	{
		user_map[9][i + 1] = count;
		ball_map[9][i + 1] = count;
		count++;
	}
	for (int i = 8; i > 0; i--)
	{
		user_map[i][9] = count;
		ball_map[i][9] = count;
		count++;
	}
	for (int i = 8; i > 0; i--)
	{
		user_map[0][i] = count;
		ball_map[0][i] = count;
		count++;
	}
	while (error_found(this))
	{
		reset_grid();
	}
}

void grid::draw_map()
{
	attroff(COLOR_PAIR(5));
	// attron(COLOR_PAIR(8));
	// attron(A_BOLD);
	mvprintw(Y_DOWN - 4, 34, "THE BLACK BOX");
	// attroff(A_BOLD);
	// attroff(COLOR_PAIR(8));
	attron(COLOR_PAIR(5));
	// mvprintw(0,0,"MOVES: %d", moves);
	if (points <= 5)
	{
		attron(COLOR_PAIR(2));
		mvprintw(0, 0, "GUESSES REMAINING: %d", points);
		attroff(COLOR_PAIR(2));
	}
	else
	{
		mvprintw(0, 0, "GUESSES REMAINING: %d", points);
	}
	if (games > 0)
	{
		mvprintw(1, 0, "TOTAL POINTS: %d", total_points);
		// mvprintw(0,10,"TOTAL MOVES: %d", total_moves);
		mvprintw(2, 0, "SESSION RECORD: %d-%d", wins, games - wins);
	}
	else
	{
		mvprintw(1, 0, "SESSION RECORD: %d-%d", wins, games - wins);
	}
	for (int i = 0; i < SIZE + 2; i++)
	{
		for (int t = 0; t < SIZE + 2; t++)
		{
			if (i == 0 || i == 9 || t == 0 || t == 9)
			{
				if (user_map[i][t] != 0)
				{
					if (guesses[(user_map[i][t] - 1) * 2])
					{
						if ((user_map[i][t] - 1) * 2 < SIZE * 2)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 5, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 4)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 6)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
					}
					if ((user_map[i][t] - 1) * 2 < SIZE * 6)
					{
						mvprintw(Y_DOWN + i * 1, X_OVER + t * 5, "%d", user_map[i][t]);
					}
					else
					{
						mvprintw(Y_DOWN + i - 1, X_OVER + t * 5, "%d", user_map[i][t]);
					}
				}
			}
			else
			{
				mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 1, "|");
				mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 2, "|");
				if (i == 1)
				{
					mvprintw(Y_DOWN + i - 1, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i - 1, X_OVER + (t * 5) + 1, "_");
					mvprintw(Y_DOWN + i, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 1, "_");
				}
				if (i != 8)
				{
					mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5) + 1, "_");
				}
				if (user_map[i][t] == -1)
				{
					attroff(COLOR_PAIR(5));
					attron(COLOR_PAIR(1));
					mvprintw(Y_DOWN + i, X_OVER + (t * 5), "  ");
					attroff(COLOR_PAIR(1));
					attron(COLOR_PAIR(5));
				}
			}
		}
	}
}

void grid::draw_map_incorrect()
{
	// attroff(COLOR_PAIR(5));
	// // attron(COLOR_PAIR(8));
	// // attron(A_BOLD);
	// mvprintw(Y_DOWN-4,34,"THE BLACK BOX");
	// // attroff(A_BOLD);
	// // attroff(COLOR_PAIR(8));
	// attron(COLOR_PAIR(5));
	// mvprintw(0,0,"MOVES: %d", moves);
	// mvprintw(1,0,"POINTS: %d", points);
	// if(games > 0){
	// 	mvprintw(0,10,"TOTAL MOVES: %d", total_moves);
	// 	mvprintw(1,11,"TOTAL POINTS: %d", total_points);
	// }
	// mvprintw(2,0,"SESSION RECORD: %d-%d", wins, games-wins);
	for (int i = 0; i < SIZE + 2; i++)
	{
		for (int t = 0; t < SIZE + 2; t++)
		{
			if (i == 0 || i == 9 || t == 0 || t == 9)
			{
				if (user_map[i][t] != 0)
				{
					if (guesses[(user_map[i][t] - 1) * 2])
					{
						if ((user_map[i][t] - 1) * 2 < SIZE * 2)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 5, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 4)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 6)
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "%d", guesses[(user_map[i][t] - 1) * 2 + 1]);
							}
						}
						else
						{
							if (guesses[(user_map[i][t] - 1) * 2 + 1] == 0)
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "HIT");
							}
							else if (guesses[(user_map[i][t] - 1) * 2 + 1] == guesses[(user_map[i][t] - 1) * 2])
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "%d", guesses[(user_map[i][t] - 1) * 2]);
							}
						}
					}
					else
					{
						int x = check_ray(this, user_map[i][t]);
						if ((user_map[i][t] - 1) * 2 < SIZE * 2)
						{
							if (x == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "HIT");
							}
							else if (x == user_map[i][t])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 5, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 4, "%d", x);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 4)
						{
							if (x == 0)
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "HIT");
							}
							else if (x == user_map[i][t])
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "%d", x);
							}
						}
						else if ((user_map[i][t] - 1) * 2 < SIZE * 6)
						{
							if (x == 0)
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "HIT");
							}
							else if (x == user_map[i][t])
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 3, "%d", x);
							}
						}
						else
						{
							if (x == 0)
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "HIT");
							}
							else if (x == user_map[i][t])
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "REFL");
							}
							else
							{
								mvprintw(Y_DOWN + i - 2, X_OVER + (t * 5), "%d", x);
							}
						}
					}
					if ((user_map[i][t] - 1) * 2 < SIZE * 6)
					{
						mvprintw(Y_DOWN + i * 1, X_OVER + t * 5, "%d", user_map[i][t]);
					}
					else
					{
						mvprintw(Y_DOWN + i - 1, X_OVER + t * 5, "%d", user_map[i][t]);
					}
				}
			}
			else
			{
				mvprintw(Y_DOWN + i, X_OVER + (t * 5) - 1, "|");
				mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 2, "|");
				if (i == 1)
				{
					mvprintw(Y_DOWN + i - 1, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i - 1, X_OVER + (t * 5) + 1, "_");
					mvprintw(Y_DOWN + i, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i, X_OVER + (t * 5) + 1, "_");
				}
				if (i != 8)
				{
					mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5), "_");
					mvprintw(Y_DOWN + i + 1, X_OVER + (t * 5) + 1, "_");
				}
				if (user_map[i][t] == -1)
				{
					if (ball_map[i][t] == -1)
					{
						attroff(COLOR_PAIR(5));
						attron(COLOR_PAIR(1));
						mvprintw(Y_DOWN + i, X_OVER + (t * 5), "XX");
						attroff(COLOR_PAIR(1));
						attron(COLOR_PAIR(5));
					}
					else
					{
						attroff(COLOR_PAIR(5));
						attron(COLOR_PAIR(1));
						mvprintw(Y_DOWN + i, X_OVER + (t * 5), "  ");
						attroff(COLOR_PAIR(1));
						attron(COLOR_PAIR(5));
					}
				}
				else if (ball_map[i][t] == -1)
				{
					mvprintw(Y_DOWN + i, X_OVER + (t * 5), "XX");
				}
			}
		}
	}
}

int pass_ray(grid *g, int x, int y, int dir, int num)
{
	// int end = 0;
	// static int input = 1;
	// checks the number of iterations made on pass ray, find new seed if needed
	if (g->count >= 50000)
	{
		std::cout << "Error in this seed: " << g->seed << std::endl;
		g->works = 0;
		return -20;
	}
	g->count++;
	// g->ball_map[y][x] = count;
	// count++;
	// input++;
	int bomb_off_center = 0;
	switch (dir)
	{
	case 1:
		if (g->ball_map[y][x + 1] == -1)
		{
			return 0;
		}
		if (g->ball_map[y - 1][x + 1] == -1)
		{
			// y++;
			x--;
			dir = 4;
			bomb_off_center = 1;
		}
		if (g->ball_map[y + 1][x + 1] == -1)
		{
			if (dir == 4)
			{
				return num;
			}
			else
			{
				x--;
				// y--;
				dir = 2;
				bomb_off_center = 1;
			}
		}
		if (bomb_off_center)
		{
			num = pass_ray(g, dir, x, y, num);
		}
		if (g->ball_map[y][x + 1] > 0)
		{
			return g->ball_map[y][x + 1];
		}
		num = pass_ray(g, x + 1, y, dir, num);
		break;
	case 2:
		if (g->ball_map[y - 1][x] == -1)
		{
			return 0;
		}
		if (g->ball_map[y - 1][x - 1] == -1)
		{
			bomb_off_center = 1;
			// x++;
			y++;
			dir = 1;
		}
		if (g->ball_map[y - 1][x + 1] == -1)
		{
			if (dir == 1)
			{
				return num;
			}
			else
			{
				// x--;
				y++;
				dir = 3;
				bomb_off_center = 1;
			}
		}
		if (bomb_off_center)
		{
			num = pass_ray(g, x, y, dir, num);
		}
		if (g->ball_map[y - 1][x] > 0)
		{
			return g->ball_map[y - 1][x];
		}
		num = pass_ray(g, x, y - 1, dir, num);
		break;
	case 3:
		if (g->ball_map[y][x - 1] == -1)
		{
			return 0;
		}
		if (g->ball_map[y - 1][x - 1] == -1)
		{
			// y++;
			x++;
			dir = 4;
			bomb_off_center = 1;
		}
		if (g->ball_map[y + 1][x - 1] == -1)
		{
			if (dir == 4)
			{
				return num;
			}
			else
			{
				// y--;
				x++;
				dir = 2;
				bomb_off_center = 1;
			}
		}
		if (bomb_off_center)
		{
			num = pass_ray(g, x, y, dir, num);
		}
		if (g->ball_map[y][x - 1] > 0)
		{
			return g->ball_map[y][x - 1];
		}
		num = pass_ray(g, x - 1, y, dir, num);
		break;
	case 4:
		if (g->ball_map[y + 1][x] == -1)
		{
			return 0;
		}
		if (g->ball_map[y + 1][x - 1] == -1)
		{
			// x++;
			y--;
			dir = 1;
			bomb_off_center = 1;
		}
		if (g->ball_map[y + 1][x + 1] == -1)
		{
			if (dir == 1)
			{
				return num;
			}
			else
			{
				// x+;
				y--;
				dir = 3;
				bomb_off_center = 1;
			}
		}
		if (bomb_off_center)
		{
			num = pass_ray(g, x, y, dir, num);
		}
		if (g->ball_map[y + 1][x] > 0)
		{
			return g->ball_map[y + 1][x];
		}
		num = pass_ray(g, x, y + 1, dir, num);
		break;
		// if(!end){
		// 	pass_ray(g, dir, x, y, num);
	}
	return num;
}

int check_ray(grid *g, int num)
{
	int x, y;
	int dir;
	if (num <= SIZE)
	{
		x = 0;
		y = num;
		dir = 1;
	}
	else if (num <= SIZE * 2)
	{
		x = num - SIZE;
		y = SIZE + 1;
		dir = 2;
	}
	else if (num <= SIZE * 3)
	{
		x = SIZE + 1;
		y = SIZE * 3 - num + 1;
		dir = 3;
	}
	else
	{
		x = SIZE * 4 - num + 1;
		y = 0;
		dir = 4;
	}
	int ret = -1;
	switch (dir)
	{
	case 1:
		if (g->ball_map[y - 1][x + 1] == -1)
		{
			ret = num;
		}
		else if (g->ball_map[y + 1][x + 1] == -1)
		{
			ret = num;
		}
		if (g->ball_map[y][x + 1] == -1)
		{
			return 0;
		}
		break;
	case 2:
		if (g->ball_map[y - 1][x - 1] == -1)
		{
			ret = num;
		}
		else if (g->ball_map[y - 1][x + 1] == -1)
		{
			ret = num;
		}
		if (g->ball_map[y - 1][x] == -1)
		{
			return 0;
		}
		break;
	case 3:
		if (g->ball_map[y - 1][x - 1] == -1)
		{
			ret = num;
		}
		else if (g->ball_map[y + 1][x - 1] == -1)
		{
			ret = num;
		}
		if (g->ball_map[y][x - 1] == -1)
		{
			return 0;
		}
		break;
	case 4:
		if (g->ball_map[y + 1][x - 1] == -1)
		{
			ret = num;
		}
		else if (g->ball_map[y + 1][x + 1] == -1)
		{
			ret = num;
		}
		if (g->ball_map[y + 1][x] == -1)
		{
			return 0;
		}
		break;
	}
	if (ret == -1)
	{
		switch (dir)
		{
		case 1:
			x++;
			ret = pass_ray(g, x, y, dir, num);
			break;
		case 2:
			y--;
			ret = pass_ray(g, x, y, dir, num);
			break;
		case 3:
			x--;
			ret = pass_ray(g, x, y, dir, num);
			break;
		case 4:
			y++;
			ret = pass_ray(g, x, y, dir, num);
			break;
		}
	}
	return ret;
}

void check_answers(grid *g)
{
	char c;
	g->games++;
	if (g->total_moves == 0)
	{
		g->total_moves = g->moves;
	}
	for (int x = 1; x < 9; x++)
	{
		for (int y = 1; y < 9; y++)
		{
			if (g->user_map[x][y] != g->ball_map[x][y])
			{
				clear();
				attron(COLOR_PAIR(3));
				mvprintw(3, 22, "YOU LOST!");
				mvprintw(4, 22, "WOULD YOU LIKE TO PLAY AGAIN? (Y/n)");
				attroff(COLOR_PAIR(3));
				g->draw_map_incorrect();
				do
				{
					c = getch();
				} while (c != 'n' && c != 'y');
				if (c == 'n')
				{
					g->playing = 0;
				}
				else
				{
					g->reset_grid();
				}
				return;
			}
		}
	}
	clear();
	attron(COLOR_PAIR(3));
	mvprintw(10, 25, "YOU GOT THEM CORRECT!");
	g->wins++;
	g->total_points += g->points;
	mvprintw(11, 25, "WOULD YOU LIKE TO PLAY AGAIN? (Y/n)");
	attroff(COLOR_PAIR(3));
	do
	{
		c = getch();
	} while (c != 'n' && c != 'y');
	if (c == 'n')
	{
		g->playing = 0;
	}
	else
	{
		g->reset_grid();
	}
	return;
}

void already_lost(grid *g)
{
	char c;
	g->games++;
	if (g->total_moves == 0)
	{
		g->total_moves = g->moves;
	}
	clear();
	attron(COLOR_PAIR(3));
	mvprintw(3, 22, "YOU LOST!");
	mvprintw(4, 22, "WOULD YOU LIKE TO PLAY AGAIN? (Y/n)");
	attroff(COLOR_PAIR(3));
	g->draw_map_incorrect();
	do
	{
		c = getch();
	} while (c != 'n' && c != 'y');
	if (c == 'n')
	{
		g->playing = 0;
	}
	else
	{
		g->reset_grid();
	}
	return;
}

void prompt_to_end(grid *g)
{
	char c;
	do
	{
		clear();
		attron(COLOR_PAIR(3));
		mvprintw(10, 30, "CHECK ANSWERS? (Y/n)");
		attroff(COLOR_PAIR(3));
		c = getch();
	} while (c != 'y' && c != 'n');
	if (c == 'y')
	{
		check_answers(g);
	}
	return;
}

void place(grid *g)
{
	char c;
	int x = 1;
	int y = 1;
	do
	{
		if (c == 'a' && y > 1)
		{
			y--;
		}
		else if (c == 's' && x < 8)
		{
			x++;
		}
		else if (c == 'd' && y < 8)
		{
			y++;
		}
		else if (c == 'w' && x > 1)
		{
			x--;
		}
		clear();
		g->draw_map();
		if (g->user_map[x][y] == -1)
		{
			attroff(COLOR_PAIR(5));
			attron(COLOR_PAIR(2));
			mvprintw(Y_DOWN + x * 1, X_OVER + y * 5, "  ");
			attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(5));
		}
		else if (g->user_map[x][y] != -1)
		{
			attroff(COLOR_PAIR(5));
			attron(COLOR_PAIR(1));
			mvprintw(Y_DOWN + x * 1, X_OVER + y * 5, "  ");
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(5));
		}

	} while (((c = getch()) != 'p' && c != 27));
	if (c == 'p')
	{
		if (g->user_map[x][y] == -1)
		{
			do
			{
				clear();
				attron(COLOR_PAIR(3));
				mvprintw(10, 30, "REMOVE BALL?(Y/n)");
				attron(COLOR_PAIR(3));
				c = getch();
			} while (c != 'n' && c != 'y');
			if (c == 'y')
			{
				// g->moves++;
				g->user_map[x][y] = 0;
				g->balls_placed--;
			}
		}
		else
		{
			if (g->balls_placed != 4)
			{
				g->balls_placed++;
				// g->moves++;
				// if(g->games){
				// g->total_moves++;
				// }
				g->user_map[x][y] = -1;
				if (g->balls_placed == 4)
				{
					prompt_to_end(g);
				}
			}
		}
	}
}

void grid::show_history()
{
	clear();
	char c;
	int count = 0;
	int pos = 0;
	do
	{
		clear();
		mvprintw(3, 27, "HISTORY OF GUESSES...");
		mvprintw(7, 52, "SCROLL UP WITH \'w\' ARROW");
		mvprintw(18, 50, "SCROLL DOWN WITH \'s\' ARROW");
		count = 0;
		if (c == 's' && pos < SIZE / 2 - 1)
		{
			pos++;
		}
		else if (c == 'w' && pos > 0)
		{
			pos--;
		}
		for (int i = pos * (SIZE * 2); i < pos * SIZE * 2 + SIZE * 2; i += 2)
		{
			if (guesses[i] == guesses[i + 1] && guesses[i])
			{
				mvprintw(9 + (count), 33, "%d -> REFLECTS", guesses[i]);
			}
			else if (guesses[i] && guesses[i + 1] != 0)
			{
				mvprintw(9 + (count), 33, "%d -> %d", guesses[i], guesses[i + 1]);
			}
			else if (guesses[i] && guesses[i + 1] == 0)
			{
				mvprintw(9 + (count), 33, "%d -> HIT", guesses[i]);
			}
			count++;
		}
		mvprintw(12, 3, "PAGE %d OF %d", pos + 1, SIZE / 2);
	} while ((c = getch()) != 27);
}

void grid::prompt_for_ray_input()
{
	char c;
	static int info = 0;
	attron(COLOR_PAIR(3));
	mvprintw(Y_DOWN + SIZE + 3, 15, "WHERE WOULD YOU LIKE RAY TO ENTER? SPACE TO CONFIRM.");
	attroff(COLOR_PAIR(3));
	if (!info)
	{
		mvprintw(22, 3, "USE \'p\' TO PLACE BALLS, \'h\' TO VIEW HISTORY, \'1-5\' TO CHANGE COLOR THEME.");
		info = 1;
	}
	char num[5];
	for (int i = 0; i < 5; i++)
	{
		num[i] = 0;
	}
	c = 'd';
	int count = 0;
	while ((c = getch()) != ' ')
	{
		if (c == 27)
		{
			for (int i = 0; i < 5; i++)
			{
				num[i] = 0;
			}
			mvprintw(Y_DOWN + 4 + SIZE, 38, "\n");
			count = 0;
		}
		else if (c == 'Q')
		{
			playing = 0;
			return;
		}
		else if (c == 'h')
		{
			show_history();
			return;
		}
		else if (c == 'p')
		{
			place(this);
			return; //WITHOUT REQUIRES 2 Q's
		}
		else if (c == 'f')
		{
			if (balls_placed == 4)
			{
				prompt_to_end(this);
			}
			return;
		}
		else if ((c < 48 || c > 57))
		{ //&& c != 44){
		}
		else
		{
			if (points > 0)
			{
				mvaddch(Y_DOWN + 4 + SIZE, 40 + count, c);
				num[count] = c;
				count++;
			}
		}
	}
	if (points > 0)
	{
		if (atoi(num) < 1 || atoi(num) > SIZE * 4)
		{
			clear();
			attron(COLOR_PAIR(3));
			mvprintw(10, 20, "%d IS NOT IN THE RANGE OF THE BOARD!", atoi(num));
			mvprintw(11, 20, "HIT SPACE TO CONTINUE.");
			attroff(COLOR_PAIR(3));
			while ((getch()) != ' ')
			{
			}
		}
		else if (guesses[(atoi(num) - 1) * 2] == atoi(num))
		{
			clear();
			attron(COLOR_PAIR(3));
			mvprintw(10, 20, "%d HAS ALREADY BEEN USED!", atoi(num));
			mvprintw(11, 20, "HIT SPACE TO CONTINUE.");
			attroff(COLOR_PAIR(3));
			while (getch() != ' ')
			{
			}
		}
		else
		{
			moves++;
			if (points > 0)
			{
				points--;
			}
			if (games > 0)
			{
				total_moves++;
			}
			int pass = atoi(num);
			guesses[(atoi(num) - 1) * 2] = atoi(num);
			guesses[(atoi(num) - 1) * 2 + 1] = check_ray(this, pass);
			clear();
			// mvprintw(10,20,"%d MAPS TO %d!", atoi(num), guesses[(atoi(num)-1)*2+1]);
			// mvprintw(11,20,"HIT SPACE TO CONTINUE.");
			// while(getch() != ' '){

			// }
		}
	}
	return;
}