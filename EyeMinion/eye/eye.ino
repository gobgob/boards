#include <OctoWS2811.h>
#include "eye.h"

DMAMEM int displayMemory[LED_COUNT_PER_EYE*6];
int drawingMemory[LED_COUNT_PER_EYE*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT_PER_EYE, displayMemory, drawingMemory, config);

#define LUMINOSITY_PERCENT 30

/**
 * Apply luminosity percent to a pixel
 *
 * @param  pixel_t pixel_in - pixel to set
 * @return pixel_t
 **/
pixel_t adjust_pixel_luminosity(pixel_t pixel_in)
{
    int r = (((pixel_in >> 16) & 0xFF)*LUMINOSITY_PERCENT)/100;
    int g = (((pixel_in >>  8) & 0xFF)*LUMINOSITY_PERCENT)/100;
    int b = (((pixel_in >>  0) & 0xFF)*LUMINOSITY_PERCENT)/100;
    return (r << 16)|(g << 8)|(b << 0);
}

uint8_t reverse_table [52][2] {
            {2,0},{3,0},{4,0},{5,0},
      {6,1},{5,1},{4,1},{3,1},{2,1},{1,1},
{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{6,2},{7,2},
{7,3},{6,3},{5,3},{4,3},{3,3},{2,3},{1,3},{0,3},
{0,4},{1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},
{7,5},{6,5},{5,5},{4,5},{3,5},{2,5},{1,5},{0,5},
      {1,6},{2,6},{3,6},{4,6},{5,6},{6,6},
            {5,7},{4,7},{3,7},{2,7}
};

/**
 * Display a sprite
 *
 * @param sprite_t *sprite
 * @param int [offset_x = 0]
 * @param int [offset_y = 0]
 * @param bool [loop = false]
 **/
void display_sprite(sprite_t *sprite, int offset_x = 0, int offset_y = 0, bool loop = false)
{
    for (int i=0; i<LED_COUNT_PER_EYE; i++) {
        pixel_t pix;
        int x = reverse_table[i][0] - offset_y;
        int y = reverse_table[i][1] - offset_x;
        bool is_in_matrice = (y >= 0 && y < LINE_COUNT && x >= 0 && x < ROW_COUNT);

        if (loop || is_in_matrice) {
            pix = (*sprite)[x % LINE_COUNT][y % ROW_COUNT];
        } else {
            pix = 0;
        }

        pix = adjust_pixel_luminosity(pix);
        leds.setPixel(i, pix);
    }
    leds.show();
}

/**
 * Display a color on all leds
 *
 * @param pixel_t color
 **/
void display_color(pixel_t color)
{
    int i;
    color = adjust_pixel_luminosity(color);
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        leds.setPixel(i, color);
    }
    leds.show();
}

sprite_t sprite_row = {
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0},
    {1,0,0,0,0,0,0,0}
};

/**
 * Display single row in one color
 *
 * @param pixel_t color
 * @param uint8_t row
 **/
void display_row(pixel_t color, uint8_t row)
{
    int i;
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        pixel_t pix = (sprite_row)[reverse_table[i][0]][(reverse_table[i][1] + row) % ROW_COUNT];
        if (pix) pix = color;
        pix = adjust_pixel_luminosity(pix);
        leds.setPixel(i, pix);
    }
    leds.show();
}

sprite_t sprite_line = {
    {1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

/**
 * Display single line in one color
 *
 * @param pixel_t color
 * @param uint8_t line
 **/
void display_line(pixel_t color, uint8_t line)
{
    int i;
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        pixel_t pix = (sprite_line)[(reverse_table[i][0] + line) % LINE_COUNT][reverse_table[i][1]];
        if (pix) pix = color;
        pix = adjust_pixel_luminosity(pix);
        leds.setPixel(i, pix);
    }
    leds.show();
}


int rainbowColors[180];
/**
 * Pre-compute rainbow colors
 **/
void precompute_rainbow_colors()
{
    for (int i=0; i<180; i++) {
        int hue = i * 2;
        int saturation = 100;
        int lightness = 5;
        rainbowColors[i] = makeColor(hue, saturation, lightness);
    }
}

/**
 * Display a beautiful rainbow
 *
 * @param int phaseShift
 *   phaseShift is the shift between each row.  phaseShift=0
 *   causes all rows to show the same colors moving together.
 *   phaseShift=180 causes each row to be the opposite colors
 *   as the previous.
 *
 * @param int cycleTime
 *   cycleTime is the number of milliseconds to shift through
 *   the entire 360 degrees of the color wheel:
 *   Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
 **/
void rainbow(int phaseShift, int cycleTime)
{
  int color, x, y, offset, wait;

  wait = cycleTime / LED_COUNT_PER_EYE;
  for (color=0; color < 180; color++) {
    for (x=0; x < LED_COUNT_PER_EYE; x++) {
      for (y=0; y < 8; y++) {
        int index = (color + x + y*phaseShift/2) % 180;
        leds.setPixel(x + y*LED_COUNT_PER_EYE, rainbowColors[index]);
      }
    }
    leds.show();
    delay(wait);
  }
}

// SPRITES
sprite_t arrow_top = {
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x841686,0x841686,0x841686,0x841686,0x000000,0x000000},
    {0x000000,0x841686,0x000000,0x841686,0x841686,0x000000,0x841686,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x841686,0x841686,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

sprite_t heart = {
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xff0000,0xff0000,0x000000,0xff0000,0xff0000,0x000000},
    {0x000000,0xff0000,0xff0000,0xff0000,0x000000,0xff0000,0xff0000,0xff0000},
    {0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000},
    {0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000},
    {0x000000,0x000000,0xff0000,0xff0000,0xff0000,0xff0000,0xff0000,0x000000},
    {0x000000,0x000000,0x000000,0xff0000,0xff0000,0xff0000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0xff0000,0x000000,0x000000,0x000000}
};

sprite_t clap_open = {
    {0x000000,0x000000,0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

sprite_t clap_close = {
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
    {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};

// ANIMATIONS
#define K2000   1
#define HEART   2
#define CLAP    3
#define RAINBOW 4
void animation(int anim)
{
    switch(anim)
    {
        case K2000:
            for (int i = 1; i < 8; i++){
                display_row(0xFF0000, i);
                delay(70-i*3);
            }
            for (int i = 8; i > 0 ; i--){
                display_row(0xFF0000, i);
                delay(70-i*3);
            }
            break;
        case HEART:
            for (int i = 8; i > -1; i--){
                display_sprite(&heart, 0, i);
                delay(50);
            }
            delay(1000);
            break;

        case CLAP:
            display_sprite(&clap_open);
            delay(500);
            display_sprite(&clap_close);
            delay(250);
            display_sprite(&clap_open);
            delay(500);
            break;

        case RAINBOW:
            rainbow(100, 2500);
            break;
    }
}

void setup()
{
    pinMode(14, OUTPUT);
    leds.begin();
    leds.show();

    precompute_rainbow_colors();
}

uint8_t lap = 0;

void loop()
{
    //display_sprite(&arrow_top, 0, lap % 8);
    //display_color(0x0000FF);
    display_line(0x002222, lap % 8);
    delay(100);
    lap++;
}
