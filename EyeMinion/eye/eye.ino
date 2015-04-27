#include <OctoWS2811.h>
#include "eye.h"
#include "sprite_letters.h"
#include "sprites.h"

DMAMEM int displayMemory[LED_COUNT_PER_EYE*6];
int drawingMemory[LED_COUNT_PER_EYE*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT_PER_EYE, displayMemory, drawingMemory, config);

/**
 * Apply luminosity percent to a pixel
 *
 * @param  pixel_t pixel_in - pixel to set
 * @param  int [luminosity_percent = 10]
 * @return pixel_t
 **/
pixel_t adjust_pixel_luminosity(pixel_t pixel_in, int luminosity_percent = 10)
{
    int r = (((pixel_in >> 16) & 0xFF)*luminosity_percent)/100;
    int g = (((pixel_in >>  8) & 0xFF)*luminosity_percent)/100;
    int b = (((pixel_in >>  0) & 0xFF)*luminosity_percent)/100;
    return (r << 16)|(g << 8)|(b << 0);
}


const uint8_t reverse_table [52][2] {
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
 * @param int [eye_port = 1] - Port where eye is connected (1 to 8)
 * @param int [offset_x = 0]
 * @param int [offset_y = 0]
 * @param bool [loop = false]
 **/
void display_sprite(const sprite_t *sprite, int eye_port = 1, int offset_x = 0, int offset_y = 0, bool loop = false)
{
	for (int i = 0 + LED_COUNT_PER_EYE * (eye_port - 1); i<(LED_COUNT_PER_EYE * eye_port); i++) {
        pixel_t pix;
		int x = reverse_table[i%LED_COUNT_PER_EYE][0] - offset_y;
		int y = reverse_table[i%LED_COUNT_PER_EYE][1] - offset_x;
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
 * Display a text
 *
 * @param char *text
 * @param pixel_t  [color=0xffffff]
 *
 * Example:
 * display_text("abc");
 *  => Display ABC on eye
 *
 **/
void display_text(char * text, pixel_t color = 0xffffff)
{
    int text_length = strlen(text);
    for(int offset_x=-(ROW_COUNT * EYES_COUNT); offset_x<text_length*ROW_COUNT; offset_x++) {
        for (int i=0; i<LED_COUNT; i++) {
			int offset_eye2 = (i > LED_COUNT_PER_EYE * (EYE2_PORT - 1)) ? EYE2_OFFSET : 0;

            pixel_t pix = 0;
            for( int letter=0; letter<text_length; letter++) {
                int x = reverse_table[i%LED_COUNT_PER_EYE][0];
				int y = reverse_table[i%LED_COUNT_PER_EYE][1] + offset_x - letter * 6 + offset_eye2;
                sprite_t * sprite_letter = letters[text[letter]];
                bool is_in_matrice = (y >= 0 && y < ROW_COUNT);
                if ( is_in_matrice && sprite_letter!=NULL){
                    if((*sprite_letter)[x][y]){
                        pix = color;                        
                    }
                }
            }
            pix = adjust_pixel_luminosity(pix);
            leds.setPixel(i, pix);
        }
        leds.show();
        delay(100);
    }
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
  int color, x, y, wait;

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
			display_text("ACTION");
            break;

        case RAINBOW:
            rainbow(100, 2500);
            break;
    }
}

// Serial
char serialInput[10];
int serialPrompt = 0;
int animNum = 0;
int mode = 3; // Manuel

/**
 * Capture and parse serial events.
**/
void serialEvent()
{
	while (Serial.available())
	{
		char inChar = (char)Serial.read();

		switch (inChar)
		{
		case '0'...'9':
			serialInput[serialPrompt] = inChar - '0';
			serialPrompt++;
			break;

		case '\n':
			serialExecute();
			break;

		default:
			serialInput[serialPrompt] = inChar;
			serialPrompt++;
			break;
		}
	}
}

/**
 * Execute serial command (after `\n` char)
 * 
 * # Commands
 *   ## Animations
 *   A01 -> animation 1
 *
**/
void serialExecute()
{
	switch (serialInput[0])
	{
	case 'A': // Animation - `A01` for animation 1
		animNum = serialInput[1] * 10 + serialInput[2];
		Serial.print("Run animation ");
		Serial.println(animNum);
		break;

	default:
		Serial.println("Unkwnown command");
	}

	// Reset var for next instruction
	for (int i = 0; i < 10; i++)
		serialInput[i] = 0;
	serialPrompt = 0;
}

void setup()
{
    pinMode(14, OUTPUT);
    leds.begin();
    leds.show();

    precompute_rainbow_colors();
	Serial.begin(9600);
}


void loop()
{
    animation(CLAP);
    // serialEvent();
}
