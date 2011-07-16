#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void display_set(uint8_t col, uint8_t row, uint8_t val);
void display_toggle(uint8_t col, uint8_t row);

void display_init( void );
void display_laufschrift(char * text, uint8_t speed, uint8_t rounds);
void display_roll(uint16_t bytes, uint8_t speed, uint8_t rounds);
uint8_t * display_print(char *text, uint8_t *buffer);

#endif /* _DISPLAY_H_ */
