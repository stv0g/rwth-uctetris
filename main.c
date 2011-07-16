#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "main.h"
#include "display.h"
#include "tetris.h"
#include "conway.h"

volatile extern uint8_t *volatile display_buffer; /* Buffer für Display */

static uint8_t rwth_logo[] = {0x00, 0x7c, 0x10, 0x7c, 0x40, 0x7c, 0x40, 0x78, 0x04, 0x18, 0x04, 0x78, 0x00, 0x2c, 0x50, 0x7c};
static char bill_txt[] = "'Nobody will ever need more than 640k RAM!' - Bill Gates, 1981 ;-)";

uint8_t get_seed() {
	uint8_t seed = 0;
	uint8_t *p = (uint8_t *) (RAMEND+1);
	extern uint8_t __heap_start;
    
	while (p >= &__heap_start + 1)
		seed ^= * (--p);
    
	return seed;
}

void random_start() {
	volatile uint8_t random_buffer[16];
	display_buffer = memset(random_buffer, 0, 16);
	
	while ( TRUE ) {
		uint8_t row = rand() % 16;
		uint8_t col = rand() % 8;
		display_toggle(col, row);
			
		if (~PINB & KEY_Y) {
			break;
		}	
		_delay_ms(20);
	}
}

int main( void ) {
	display_init();
	srand(get_seed());
	
	/* setup gamepad */
	DDRB = 0x00;
	PORTB = 0xff;

	while ( TRUE ) {	
		/* Demo 0: Bitmaps */
		display_buffer = rwth_logo;
		_delay_ms(1500);
	
		/* Demo 1: Tetris */
		tetris_start();
		_delay_ms(300);
		
		/* Demo 4: Conways Game of Life */
		conway_start();
		_delay_ms(300);
	
		/* Demo 2: Laufschrift */
///		display_laufschrift("test", 120, 1);

		/* Demo 3: Zufall */
		random_start();
		_delay_ms(300);
	}
}
