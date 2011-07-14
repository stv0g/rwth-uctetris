#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void display_init( void );
void display_laufschrift(uint8_t *buffer, uint16_t bytes, uint8_t speed, uint8_t rounds);
uint8_t * display_print(char *text, uint8_t *buffer);

#endif /* _DISPLAY_H_ */
