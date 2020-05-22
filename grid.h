#ifndef GRID_H
#define GRID_H

#define SIZE 8
#define Y_DOWN 9
#define X_OVER 17

class grid
{
public:
	grid(time_t &);
	time_t seed;
	int works;
	int games;
	int points;
	int total_points;
	int total_moves;
	int wins;
	int moves;
	static int count;
	int ball_map[SIZE + 2][SIZE + 2];
	int user_map[SIZE + 2][SIZE + 2];
	int guesses[SIZE * 4 * 2];
	int playing;
	int balls_placed;
	void show_history();
	void draw_map_incorrect();
	int is_good();
	void reset_grid();
	void prompt_for_ray_input();
	void draw_map();
	~grid(){};

private:
};

int check_ray(grid *g, int pass);
int error_found(grid *g);
void already_lost(grid *g);

#endif