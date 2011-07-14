#ifndef _MAIN_H_
#define _MAIN_H_

#define TRUE 1
#define FALSE 0

#define KEY_DOWN (1 << 0)
#define KEY_UP (1 << 1)
#define KEY_A (1 << 2)
#define KEY_RIGHT (1 << 3)
#define KEY_LEFT (1 << 4)
#define KEY_X (1 << 5)
#define KEY_B (1 << 6)
#define KEY_Y (1 << 7)

typedef uint8_t bool_t;

uint8_t get_seed( void );

#endif /* _MAIN_H_ */
