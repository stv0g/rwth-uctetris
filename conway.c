#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "main.h"
#include "display.h"
#include "conway.h"

volatile extern uint8_t *volatile display_buffer; /* Buffer für Display */

uint8_t conway_cell_neighbours(uint8_t x, uint8_t y, uint8_t *world) {
	uint8_t neighbours = 0;
	int8_t a, b;

	for (a = x-1; a <= x+1; a++) {
		int8_t c = a;
		if (a < 0) c += 8;
		if (a >= 8) c -= 8;

		for (b = y-1; b <= y+1; b++) {
			int8_t d = b;
			if (a == x && b == y) continue;
			if (b < 0) d += 16;
			if (b >= 16) d -= 16;

			neighbours += (world[d] & (1 << c)) ? 1 : 0;
		}
	}

	return neighbours; /* 0 <= neighbours <= 8 */
}

uint8_t conway_next_cell_gen(uint8_t x, uint8_t y, uint8_t *world) {
	uint8_t neighbours = conway_cell_neighbours(x, y, world);
	uint8_t alive = world[y] & (1 << x);

	if (alive) {
		if (neighbours > 3 || neighbours < 2) {
			return 0; /* died by over-/underpopulation */
		}
		else {
			return 1; /* kept alive */
		}
	}
	else if (neighbours == 3) {
		return 1; /* born */
	}
	else {
		return 0; /* still dead */
	}
}

void conway_next_gen(uint8_t *world, uint8_t *next_gen) {
	uint8_t x, y;

	for (y = 0; y < 16; y++) {
		next_gen[y] = 0;
		for (x = 0; x < 8; x++) {
			next_gen[y] |= conway_next_cell_gen(x, y, world) << x;
		}
	}
}

void conway_start() {
	volatile uint8_t worlds[2][16];
	display_buffer = memset(worlds, 0, 32);
	
	uint8_t  i = 0;
	
	/* by random */
	for (uint8_t q = 0; q < 32; q++) {
		uint8_t row = rand() % 16;
		uint8_t col = rand() % 8;
		display_set(col, row, 1);
	}
	
	while ( TRUE ) {
		_delay_ms(100);
		conway_next_gen(worlds[i], worlds[1 - i]);
		
		display_buffer = worlds[1 - i];
		i = 1 - i; // switch world
		
		if (~PINB & KEY_Y) {
			return; // exit
		}
		if (~PINB & KEY_A) {
			worlds[i][7] |= 0b00001100;
			worlds[i][8] |= 0b00011000;
			worlds[i][9] |= 0b00001000;
		}
		if (~PINB & KEY_B) {
			worlds[i][7] |= 0b00010000;
			worlds[i][8] |= 0b00001000;
			worlds[i][9] |= 0b00111000;
		}
	}
}
