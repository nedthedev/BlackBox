#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>

#include "grid.h"

int infinite(){
	static int count = 0;
	std::cout << count << std::endl;
	count++;
	if(1){
		infinite();
	}
	return 0;
}

int main(int argc, char *argv[]){

	// infinite();

	time_t seed;
  	struct timeval tv;
	gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
	// srand(1523145647);
	// srand(2132880270);
	srand(seed);
	// std::cout << seed << std::endl;

	grid g;

	while(error_found(&g)){
		g.reset_grid();
	}

	WINDOW *win = initscr();
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_YELLOW);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(5, COLOR_BLUE, COLOR_WHITE);
	init_pair(8,COLOR_BLACK, COLOR_RED);
	init_pair(6, COLOR_BLACK, COLOR_WHITE);
	init_pair(7, COLOR_WHITE, COLOR_CYAN);
	wbkgd(win, COLOR_PAIR(5));
 	raw();
  	noecho();
 	curs_set(0);
 	keypad(stdscr, TRUE);
 	ESCDELAY = 0;
	clear();

	do{
	// 	attron(COLOR_PAIR(1));
	// 	mvprintw(5,2," ");
	// 	attroff(COLOR_PAIR(1));
	// 	mvprintw(5,4,"YELLOW MEANS IT REFLECTED.");
	// 	attron(COLOR_PAIR(2));
	// 	mvprintw(5,31," ");
	// 	attroff(COLOR_PAIR(2));
	// 	mvprintw(5,33,"RED MEANS IT HIT.");
	// 	attron(COLOR_PAIR(3));
	// 	mvprintw(5,51," ");
	// 	attroff(COLOR_PAIR(3));
	// 	attron(COLOR_PAIR(4));
	// 	mvprintw(4,51," ");
	// 	attroff(COLOR_PAIR(4));
	// 	attron(COLOR_PAIR(5));
	// 	mvprintw(6,51," ");
	// 	attroff(COLOR_PAIR(5));
	// 	mvprintw(5,53, "SAME COLOR SHOWS MATCHES.");
	// 	refresh();

	// 	mvprintw(15, 30, "SEED:%d", seed);
		mvprintw(10,17, "WELCOME TO THE BLACK BOX. SPACE TO CONTINUE.");
	}while(getch() != ' ');

		clear();

	while(g.playing){
		attron(COLOR_PAIR(3));
		clear();
		g.draw_map();
		g.prompt_for_ray_input();
	}

	endwin();
	return 0;
}