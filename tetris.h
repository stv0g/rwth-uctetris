#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdint.h>

#include "main.h"

#define NUM_SHAPES 7
#define NUM_LINES 20 /* 16 visible + 4 to spawn new stones */
#define FRAMES 60

typedef uint8_t board_t[NUM_LINES];
typedef uint8_t clipping_t[4];

typedef struct {
	stone_t stn;
	board_t brd;
} tetris_t;

/* Named according to: http://de.wikipedia.org/wiki/Tetris */
typedef enum {
	SHAPE_I, /* line of four dots */
	SHAPE_J, /* inverted L */
	SHAPE_L,
	SHAPE_O, /* cubeoid */
	SHAPE_S,
	SHAPE_Z, /* inverted S */
	SHAPE_T
} shape_t;

typedef enum {
	DIR_LEFT,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_ALL
} direction_t;

typedef struct {
	shape_t shape;
	uint8_t clipping[4];
	int8_t pos_x, pos_y, orientation;
} stone_t;

inline static void tetris_copy_clipping(clipping_t src, clipping_t dest) {
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
}

void tetris_start( void );
uint8_t tetris_flush_lines( void );
bool_t tetris_turn_stone( void );
bool_t tetris_shift_stone(direction_t dir, uint8_t times);
bool_t tetris_detect_collision(direction_t dir);

#endif /* _TETRIS_H_ */
