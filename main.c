#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>

#include "main.h"

static stone_t shapes[NUM_SHAPES] = {
	{SHAPE_I, {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00111100
	}, 0},
	{SHAPE_J, {
		0b00000000,
		0b00001000,
		0b00001000,
		0b00011000
	}, 0
	},
	{SHAPE_L, {
		0b00000000,
		0b00010000,
		0b00010000,
		0b00011000
	}, 0},
	{SHAPE_O, {
		0b00000000,
		0b00000000,
		0b00011000,
		0b00011000
	}, 0},
	{SHAPE_S, {
		0b00000000,
		0b00000000,
		0b00011000,
		0b00110000
	}, 0},
	{SHAPE_T, {
		0b00000000,
		0b00000000,
		0b00010000,
		0b00111000
	}, 0},
	{SHAPE_Z, {
		0b00000000,
		0b00000000,
		0b00011000,
		0b00001100
	}, 0}

};

void draw_screen(board_t brd, stone_t stn, uint16_t score, uint8_t level, uint8_t stones) {
	/* clear screen */
	clear();
		
	for (uint8_t i = 0; i < NUM_LINES; i++) {
		addch('|');
		
		for (uint8_t j = 0; j < 8; j++) {
			
			if (brd[i] & (1 << j)) { /* fixed pixel */
				addch('#');
			}
			else if (i >= stn.pos && i < stn.pos+4 && stn.clipping[i-stn.pos] & (1 << j)) {
				addch('0');
			}
			else {
				addch(' ');
			}
		}
		
		printw("%c\n", (i == 3) ? '-' : '|');
	}
	
	printw("----------");
	printw("\nScore: %i\tLevel: %i\tStones: %i\n", score, level, stones);
}

uint8_t flush_lines(board_t brd) {
	uint8_t lines = 0;

	for (uint8_t i = NUM_LINES-1; i >= 0; i--) {
		if (brd[i] == 0xFF) {
			lines++;
			
			//for () { /* restock with remaining lines */
		
			//}
		}
		else if (brd[i] == 0x00) { /* empty line, no following line can be non-empty => aborting */
			break;
		}
	}
	
	return lines; /* required to calculate score */
}

bool_t turn_stone(stone_t s) {

}

bool_t shift_stone(stone_t stn, direction_t dir) {
	if (dir == DIR_DOWN) {
		stn.pos++;
	}
	else {
		for (uint8_t i = 0; i < 4; i++) {
			if (dir == DIR_LEFT) {
				stn.clipping[i] <<= 1;
			}
			else if (dir == DIR_RIGHT) {
				stn.clipping[i] >>= 1;
			}
		}
	}
	stn.clipping[3] = 0;
}

bool_t detect_collision(board_t brd, stone_t stn) {
	return FALSE; // TODO implement
}

WINDOW * init_screen() {
	WINDOW * win = initscr();
	noecho();
	cbreak();
	timeout(0);
	keypad(win, 1);
	curs_set(0);
	
	return win;
}

void main() {
	board_t brd;
	stone_t falling;
	uint8_t level = 0;
	uint16_t score = 0;
	uint16_t stones = 0;
	uint16_t steps = 0;

	memset(&brd, 0, NUM_LINES); /* init board */
	WINDOW * win = init_screen(); /* init terminal */
	
	/* init prng */
	// TODO init with adc value & eeprom seed
	srand(129);
	
	while (TRUE) {
		/* add new stone on the top of our board */
		falling = shapes[rand() % NUM_SHAPES];
	
		if (stones > level * 10) {
			level++; /* next level */
		}
	
		while (detect_collision(brd, falling) == FALSE) {
			/* lets fall stone */
			falling.pos++;
	
			draw_screen(brd, falling, score, level, stones);
			printw("Steps: %i\n", steps++);
			
			/* poll for user interaction */
	        	switch (getch()) {
				case KEY_UP:
					turn_stone(falling);
					break;
				case KEY_LEFT:
					shift_stone(falling, DIR_LEFT);
					break;
				case KEY_RIGHT:
					shift_stone(falling, DIR_RIGHT);
					break;
				case KEY_DOWN:
					shift_stone(falling, DIR_DOWN);
					break;
	        	}
	        	flushinp();
			refresh();
			usleep(800000);
		}
	
		/* check for completed lines and calculate score */
		uint8_t lines = flush_lines(brd);
		score += lines * lines;
	
		if (brd[3] > 0) {
			break; /* game over */
		}
	}
	
	// TODO handle highscore
	
	endwin(); /* reset terminal */
}
