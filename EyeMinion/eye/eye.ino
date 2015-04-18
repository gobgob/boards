#include <OctoWS2811.h>
#include "eye.h"

DMAMEM int displayMemory[LED_COUNT_PER_EYE*6];
int drawingMemory[LED_COUNT_PER_EYE*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT_PER_EYE, displayMemory, drawingMemory, config);

#define LUMINOSITY_PERCENT 30

pixel_t adujst_pixel_luminosity(pixel_t pixel_in)
{
    //this could be optimised by using a lookup table calculated once
    int r = (((pixel_in >> 16) & 0xFF)*LUMINOSITY_PERCENT)/100;
    int g = (((pixel_in >>  8) & 0xFF)*LUMINOSITY_PERCENT)/100;
    int b = (((pixel_in >>  0) & 0xFF)*LUMINOSITY_PERCENT)/100;
    return (r << 16)|(r << 8)|(b << 0);
}

uint8_t reverse_table [52][2] {
            {7,2},{7,3},{7,4},{7,5},
      {6,6},{6,5},{6,4},{6,3},{6,2},{6,1},
{5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},
{4,7},{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0},
{3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},
{2,7},{2,6},{2,5},{2,4},{2,3},{2,2},{2,1},{2,0},
      {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},
            {0,5},{0,4},{0,3},{0,2},
};

frame_t test_frame ={
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
    {0x0F0000,0x00000F,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000,0x0F0000},
};

void setup()
{
    pinMode(14, OUTPUT);
    leds.begin();
    leds.show();
    Serial.begin(9600);
}

void loop()
{
    unsigned long prev_time = 0;
    unsigned long post_time = 0;

    prev_time = micros();
    display_frame(&test_frame);
    post_time = micros();
    Serial.print("Delay ");
    Serial.println(post_time - prev_time);
    delay(1000);
}

void display_frame(frame_t * frame)
{
    int i;
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        pixel_t pix = (*frame)[reverse_table[i][0]][reverse_table[i][1]];
    Serial.println(pix);
        pix = adujst_pixel_luminosity(pix);
        leds.setPixel(i, pix);
    }
    leds.show();    
}

void display_color(pixel_t color)
{
    int i;
    color = adujst_pixel_luminosity(color);
    for(i=0; i<LED_COUNT_PER_EYE; i++){
        leds.setPixel(i, color);
    }
    leds.show();
}
