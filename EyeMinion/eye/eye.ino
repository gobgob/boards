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
            {7,2},{7,3},{7,4},{7,5},
      {6,6},{6,5},{6,4},{6,3},{6,2},{6,1},
{5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},
{4,7},{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0},
{3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},
{2,7},{2,6},{2,5},{2,4},{2,3},{2,2},{2,1},{2,0},
      {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},
            {0,5},{0,4},{0,3},{0,2}
};

/**
 * Display a sprite
 *
 * @param sprite_t *sprite
 * @param uint8_t [offset_x = 0]
 * @param uint8_t [offset_y = 0]
 **/
void display_sprite(sprite_t *sprite, uint8_t offset_x = 0, uint8_t offset_y = 0)
{
    int i;
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        pixel_t pix = (*sprite)[(reverse_table[i][0] + offset_y) % LINE_COUNT][(reverse_table[i][1] + offset_x) % ROW_COUNT];
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


void setup()
{
    pinMode(14, OUTPUT);
    leds.begin();
    leds.show();
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