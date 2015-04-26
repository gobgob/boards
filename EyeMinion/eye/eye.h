#ifndef _OEIL_H_
#define _OEIL_H_

// Utilities
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define LED_COUNT_PER_EYE 52
#define LINE_COUNT 8
#define ROW_COUNT 8

#define EYE1_PORT 1 // PIN 2
#define EYE2_PORT 3 // PIN 7

#define LED_COUNT LED_COUNT_PER_EYE * MAX(EYE1_PORT, EYE2_PORT)

typedef uint32_t pixel_t;
typedef pixel_t sprite_t[ROW_COUNT][LINE_COUNT];

#endif //_OEIL_H_
