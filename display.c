#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "display.h"
#include "font.h"
#include "tetris.h"

volatile uint8_t *volatile display_buffer; /* Buffer für Display */

extern volatile stone_t stn;

void display_set(uint8_t col, uint8_t row, uint8_t val) {
	if (val) {
		display_buffer[row] |= (1 << col);
	}
	else {
		display_buffer[row] &= ~(1 << col);
	}
}

void display_toggle(uint8_t col, uint8_t row) {
	display_buffer[row] ^= (1 << col);
}

/**
 * Initialisiere Display im Multiplexing Modus
 */
void display_init() {
	DDRC = 0xff; /* Ausgang: Zeilen 0-7 */
	DDRA = 0xff; /* Ausgang: Zeilen 8-15 */
	DDRD = 0xff; /* Ausgang: Spalten 0-7 (gespiegelt) */
	
	/* Initalisiere Timer */
	/**
	 * "NES Tetris operates at 60 frames per second. At level 0, a piece falls one step every 48 frames, and at level 19, a piece falls one step every 2 frames"
	 * (http://en.wikipedia.org/wiki/Tetris)
	 */
	 TCCR0 |= (1 << CS01) | (1 << CS00) | (1 << WGM01);
	 OCR0 = 240;
	 TIMSK |= (1 << OCIE0);
	 TIFR |= (1 << OCF0);
	 
	 sei();
}

uint8_t * display_print(char *text, uint8_t *buffer) {
	uint16_t len = strlen(text);

	strupr(text); /* Nur Großbuchstaben sind verfügbar */

	for (uint16_t c = len-1; c >= 0; c--) {
		char p = text[c];
		char q = (p >= ' ' && p <= '_') ? p - ' ' : 0;
	
		mempcpy(buffer[c*4], font[q], 3);
	}
	
	return buffer;
}

void display_laufschrift(char *text, uint8_t speed, uint8_t rounds) {
	uint16_t len = 4 * strlen(text) + 16; // 4 Bytes pro Character + 2 * 16 Bytes Padding
	uint8_t *orig_buffer = display_buffer;
	
	volatile uint8_t *buffer = malloc(len);
	
	memset(buffer, 0, len);
	display_buffer = display_print("test", buffer);

	while ( TRUE ) {
		buffer[15]++;
		_delay_ms(500);
	}
	
	//display_roll(buffer, len, speed, rounds);

	display_buffer = orig_buffer; /* reset to old buffer */
	free(buffer);
}

void display_roll(uint16_t bytes, uint8_t speed, uint8_t rounds) {
	uint8_t *end_buffer = display_buffer;
	display_buffer += bytes - 16;
	
	while (rounds) {
		if (display_buffer == end_buffer) {
			display_buffer = end_buffer + bytes - 16;
			rounds--;
		}
		
		display_buffer--;
		_delay_ms(speed);
	}
}

/**
 * Multiplexing Routine
 */
ISR(TIMER0_COMP_vect) {
	static uint8_t column;
	
	uint8_t row_mask = (1 << column);
	uint16_t column_mask = 0;
	
	for (uint8_t i = 0; i < 16; i++) {
		if (row_mask & display_buffer[i]) { /* fixed pixels, dimmed */
			column_mask |= (1 << i);
		}
		
		if (i+4 >= stn.pos_y && i < stn.pos_y) { /* in clipping of falling stone ? */
			if (row_mask & stn.clipping[i+4-stn.pos_y]) {
				column_mask |= (1 << i);
			}
		}
	}
	
	PORTD = 0;
	PORTC = (uint8_t) column_mask;
	PORTA = (uint8_t) (column_mask >> 8);
	PORTD = row_mask;

	column++;
	column %= 8;
}
