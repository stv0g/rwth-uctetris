#include <stdint.h>

uint8_t conway_cell_neighbours(uint8_t x, uint8_t y, uint8_t *world);
uint8_t conway_next_cell_gen(uint8_t x, uint8_t y, uint8_t *world);
void conway_next_gen(uint8_t *world, uint8_t *next_gen);
void conway_start( void );
