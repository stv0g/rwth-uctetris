#include <avr/io.h>
#include <avr/interrupt.h>

#include "display.h"
#include "tetris.h"

volatile uint8_t *volatile display_buffer; /* Buffer für Display */

extern volatile board_t brd;
extern volatile stone_t stn;

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

	for (uint16_t c = 0; c < len; c++) {
		char chr = text[len-c-1];
		uint8_t pattern;
		
		if (chr >= ' ' && chr <= '_')
			pattern = chr - ' ';
		else
			pattern = 0; /* space */
	
		for (uint8_t p = 0; p < 3; p++) {
			buffer[p+c*4+16] = font[pattern][p];
		}
		//buffer[c*4+16] = 0; /* padding */
	}
	
	return buffer;
}

void display_laufschrift(uint8_t *buffer, uint16_t bytes, uint8_t speed, uint8_t rounds) {
	display_buffer = buffer;
	while(1) {
		if (display_buffer == buffer) {
			display_buffer = buffer+bytes-16;
			if (rounds-- == 0) {
				return;
			}
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
	static uint8_t counter;
	
	uint8_t row_mask = (1 << column);
	uint16_t column_mask = 0;
	
	for (uint8_t i = 4; i < NUM_LINES; i++) {
		if (row_mask & brd[i]) { /* fixed pixels, dimmed */
			column_mask |= (1 << (i-4));
		}
		
		if (tetris) { /* in tetris mode ? */
			if (i >= tetris->stn.pos_y && i < tetris->stn.pos_y+4) { /* in clipping of falling stone ? */
				if (row_mask & tetris->stn.clipping[i-stn.pos_y]) {
					column_mask |= (1 << (i-4));
				}
			}
		}
	}
	
	PORTC = (uint8_t) column_mask;
	PORTA = (uint8_t) (column_mask >> 8);
	PORTD = row_mask;

	column++;
	if (column == 8) {
		column = 0;
		counter++;
	}
}
