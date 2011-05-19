#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>

#define TRUE 1
#define FALSE 0

#define NUM_SHAPES 7
#define NUM_LINES 20 /* 16 visible + 4 to spawn new stones */

typedef uint8_t board_t[NUM_LINES];
typedef uint8_t bool_t;

/* Named according to: http://de.wikipedia.org/wiki/Tetris */
typedef enum {
	SHAPE_I, /* line of four dots */
	SHAPE_J, /* inverted L */
	SHAPE_L,
	SHAPE_O, /* cubeoid */
	SHAPE_S,
	SHAPE_T,
	SHAPE_Z /* inverted S */
} shape_t;

typedef enum {
	DIR_LEFT,
	DIR_RIGHT,
	DIR_DOWN
} direction_t;

typedef struct {
	shape_t shape;
	uint8_t clipping[4];
	uint8_t pos;
} stone_t;

uint8_t flush_lines(board_t brd);
bool_t turn_stone(stone_t stn);
bool_t shift_stone(stone_t stn, direction_t dir);
bool_t detect_collision(board_t brd, stone_t stn);

/* platform dependent */
void draw_screen(board_t brd, stone_t stn, uint16_t score, uint8_t level, uint8_t stones);


#endif /* _MAIN_H_ */
