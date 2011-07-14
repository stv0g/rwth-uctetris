#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "tetris.h"

volatile tetris_t *volatile tetris = NULL;

//static int16_t scoring[] = {0, 40, 100, 300, 1200}; /* scoring multiplicator for 0-4 flushed lines */

static clipping_t shapes[][4] = { /* including 4 ccw rotations */
	{ // SHAPE_I
		{0x08, 0x08, 0x08, 0x08},
		{0x00, 0x1e, 0x00, 0x00},
		{0x08, 0x08, 0x08, 0x08},
		{0x00, 0x3c, 0x00, 0x00}
	},
	{ // SHAPE_J
		{0x00, 0x08, 0x08, 0x18},
		{0x00, 0x00, 0x1c, 0x04},
		{0x00, 0x0c, 0x08, 0x08},
		{0x00, 0x10, 0x1c, 0x00}
	},
	{ // SHAPE_L
		{0x00, 0x10, 0x10, 0x18},
		{0x00, 0x08, 0x38, 0x00},
		{0x00, 0x30, 0x10, 0x10},
		{0x00, 0x00, 0x38, 0x20}
	},
	{ // SHAPE_O
		{0x00, 0x18, 0x18, 0x00},
		{0x00, 0x18, 0x18, 0x00},
		{0x00, 0x18, 0x18, 0x00},
		{0x00, 0x18, 0x18, 0x00}
	},
	{ // SHAPE_S
		{0x00, 0x10, 0x18, 0x08},
		{0x00, 0x0c, 0x18, 0x00},
		{0x00, 0x08, 0x0c, 0x04},
		{0x00, 0x0c, 0x18, 0x00}
	},
	{ // SHAPE_Z
		{0x00, 0x08, 0x18, 0x10},
		{0x00, 0x00, 0x18, 0x0c},
		{0x00, 0x04, 0x0c, 0x08},
		{0x00, 0x00, 0x18, 0x0c}
	},
	{ // SHAPE_T
		{0x00, 0x10, 0x38, 0x00},
		{0x00, 0x10, 0x30, 0x10},
		{0x00, 0x38, 0x10, 0x00},
		{0x00, 0x10, 0x18, 0x10}
	}
};

uint8_t tetris_flush_lines() {
	uint8_t lines = 0;
	uint8_t i = NUM_LINES - 1;
	
	while (i >= 0) {
		if (brd[i] == 0xFF) {
			lines++;
			
			for (uint8_t j = i; j > 0; j--) { /* restock with remaining lines */
				brd[j] = brd[j-1];
			}
		}
		else if (brd[i] == 0x00) { /* empty line, no following line can be non-empty => aborting */
			break;
		}
		else {
			i--;
		}
	}
	
	return lines; /* required to calculate score */
}

bool_t tetris_turn_stone() {
	stn.orientation++;
	stn.orientation %= 4;
	
	uint8_t check = 0;
	clipping_t tmp;
	tetris_copy_clipping(shapes[stn.shape][stn.orientation], tmp);
	
	// shift to origin
	for (uint8_t i = 0; i < 4; i++) {
		if (stn.pos_x > 0) {
			tmp[i] >>= abs(stn.pos_x);
			check = tmp[i] << abs(stn.pos_x);
		}
		else {
			tmp[i] <<= abs(stn.pos_x);
			check = tmp[i] >> abs(stn.pos_x);
		}		
		
		
		if (check != shapes[stn.shape][stn.orientation][i] || brd[i+stn.pos_y] & tmp[i]) {
			return FALSE; // detected collision
		}
	}
	
	copy_clipping(tmp, stn.clipping);

	return TRUE;
}

bool_t tetris_shift_stone(direction_t dir, uint8_t times) {
	while (times--) {
		if(tetris_detect_collision(dir)) {
			return FALSE;
		}
	
		if (dir == DIR_DOWN) {			// nach unten fallen lassen
			stn.pos_y++;
		}	
		else {
			if (dir == DIR_LEFT) {		// nach links schieben
				stn.pos_x--;
				for (uint8_t i = 0; i < 4; i++) {
					stn.clipping[i] <<= 1;
				}
			}
			else if (dir == DIR_RIGHT) {	// nach rechts schieben
				stn.pos_x++;		
				for (uint8_t i = 0; i < 4; i++) {
					stn.clipping[i] >>= 1;
				}
			}
		}
	}
	
	return TRUE;
}

bool_t tetris_detect_collision(direction_t dir) {
	// Kollision beim Fallen?
	if (dir == DIR_ALL || dir == DIR_DOWN) {
		for (int8_t i = 3; i >= 0; i--) {
			if (stn.clipping[i]) {
				if (stn.pos_y+i+1 >= NUM_LINES) {			// Kollision mit Boden
					return TRUE;
				}
				else if (stn.clipping[i] & brd[stn.pos_y+i+1]) {	// Kollision mit liegenden Steinen
					return TRUE;
				}
			}
		}
	}
	// Kollision beim Shiften?
	if (dir == DIR_ALL || dir == DIR_LEFT || dir == DIR_RIGHT) {
		for (uint8_t i = 0; i < 4; i++) {
			if (stn.clipping[i] & ((dir == DIR_LEFT) ? 0x80 : 0x01)) {		// Kollision mit Waenden
				return TRUE;
			}
			// Kollision mit Steinen rechts oder links?
			else if (dir == DIR_LEFT  &&  (stn.clipping[i] << 1) & brd[stn.pos_y+i]) {	// links
				return TRUE; 
			}
			else if (dir == DIR_RIGHT &&  (stn.clipping[i] >> 1) & brd[stn.pos_y+i]) {	// rechts
				return TRUE; 
			}
		}
	}
	
	return FALSE; /* no collision */
}

void tetris_start() {
	tetris_t state;

	uint8_t frames = 48;
	uint8_t level = 0;
	//uint16_t score = 0;
	//uint16_t stones = 0;
	uint16_t lines = 0;
	uint8_t debounce;
	
	tetris = &state;
	display_buffer = state.brd+4; /* skipping "virtual" lines */
	
	/* starting with empty board */
	for (uint8_t i = 0; i < NUM_LINES; i++) {
		brd[i] = 0;
	}
	
	while (TRUE) { /* main loop */
		/* add new stone on the top of our board */
		uint8_t shape = rand() % NUM_SHAPES;
		uint8_t orientation = rand() % 4;
		
		stn.shape = shape;
		stn.orientation = orientation;
		stn.pos_x = 0;
		stn.pos_y = 0;
	
		copy_clipping(shapes[shape][orientation], stn.clipping);		
	
		if (lines > (level + 1) * 10) {
			level++; /* next level */
			frames = 48 - level * (46/19.0); /* update frames per step */
		}
	
		do {
			/* lets fall stone */
			tetris_shift_stone(DIR_DOWN, 1);

			/* poll for user interaction */
			for (uint8_t i = 0; i < frames; i++) {
				if (debounce) {
					debounce--;
				}
				else {
					uint8_t keys = ~PINB;
					
					if (keys & KEY_A)
						tetris_turn_stone();
					if (keys & KEY_LEFT)
						tetris_shift_stone(DIR_LEFT, 1);
					if (keys & KEY_RIGHT)
						tetris_shift_stone(DIR_RIGHT, 1);
					if (keys & KEY_DOWN)
						tetris_shift_stone(DIR_DOWN, 1);
					if (keys & KEY_B) /* free fall */
						tetris_shift_stone(DIR_DOWN, NUM_LINES);
					if (keys & KEY_LEFT && keys & KEY_RIGHT) {
						uint8_t *p = brd[NUM_LINES-1];

						while(*p) {
							*p = 0;
							p--;
						}
					}
					if (keys & KEY_Y) {
						_delay_ms(10);
						return; // exit tetris
					}
				
					if (keys) {
						debounce = 6
					}
				}
				
				_delay_ms(900 / FRAMES); // sleep for 1 frame (10% for calculations)
			}
		} while (tetris_detect_collision(DIR_DOWN) == FALSE);
		
		for (uint8_t i = 0; i < 4; i++) {
			brd[stn.pos_y+i] |= stn.clipping[i];
		}
	
		/* check for completed lines and calculate score */
		uint8_t flushed = flush_lines();
		//score += (level + 1) * scoring[flushed];
		//stones++;
		lines += flushed;
	
		if (brd[3] > 0) {
			//return; /* game over */
			for (uint8_t i = 0; i < NUM_LINES; i++) { /* restart with empty board */
				brd[i] = 0; /* starting with empty board */
			}
			_delay_ms(1000);
		}
	}
}
