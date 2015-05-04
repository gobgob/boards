#include <OctoWS2811.h>
#include "eye.h"
#include "sprite_letters.h"
#include "sprites.h"

DMAMEM int displayMemory[LED_COUNT_PER_EYE * 6];
int drawingMemory[LED_COUNT_PER_EYE * 6];
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
	int r = (((pixel_in >> 16) & 0xFF)*luminosity_percent) / 100;
	int g = (((pixel_in >> 8) & 0xFF)*luminosity_percent) / 100;
	int b = (((pixel_in >> 0) & 0xFF)*luminosity_percent) / 100;
	return (r << 16) | (g << 8) | (b << 0);
}


const uint8_t reverse_table[52][2] {
				    { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 },
		  { 6, 1 }, { 5, 1 }, { 4, 1 }, { 3, 1 }, { 2, 1 }, { 1, 1 },
{ 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 }, { 4, 2 }, { 5, 2 }, { 6, 2 }, { 7, 2 },
{ 7, 3 }, { 6, 3 }, { 5, 3 }, { 4, 3 }, { 3, 3 }, { 2, 3 }, { 1, 3 }, { 0, 3 },
{ 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 }, { 4, 4 }, { 5, 4 }, { 6, 4 }, { 7, 4 },
{ 7, 5 }, { 6, 5 }, { 5, 5 }, { 4, 5 }, { 3, 5 }, { 2, 5 }, { 1, 5 }, { 0, 5 },
		  { 1, 6 }, { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 }, { 6, 6 },
			        { 5, 7 }, { 4, 7 }, { 3, 7 }, { 2, 7 }
};

/**
 * Display a sprite
 *
 * @param const sprite_t *sprite
 * @param int            [eye_port = 1] - Port where eye is connected (1 to 8)
 * @param int            [offset_x = 0]
 * @param int            [offset_y = 0]
 * @param bool           [loop = false]
 * @param pixel_t        [transparent_color = 0xae8fad]
 **/
void display_sprite(const sprite_t *sprite, int eye_port = 1, int offset_x = 0, int offset_y = 0, bool loop = false, pixel_t transparent_color = 0xae8fad)
{
	for (int i = 0 + LED_COUNT_PER_EYE * (eye_port - 1); i < (LED_COUNT_PER_EYE * eye_port); i++) {
		pixel_t pix;
		int x = reverse_table[i % LED_COUNT_PER_EYE][0] - offset_y;
		int y = reverse_table[i % LED_COUNT_PER_EYE][1] - offset_x;
		bool is_in_matrice = (y >= 0 && y < LINE_COUNT && x >= 0 && x < ROW_COUNT);

		if (loop || is_in_matrice) {
			pix = (*sprite)[x % LINE_COUNT][y % ROW_COUNT];
		}
		else {
			pix = 0;
		}

		if (pix != transparent_color) {
			pix = adjust_pixel_luminosity(pix);
			leds.setPixel(i, pix);
		}
	}
}

/**
 * Display a text
 *
 * @param char *text
 * @param pixel_t  [color = 0xffffff]
 *
 * Example:
 * display_text("abc");
 *  => Display ABC on eye
 *
 **/
void display_text(char * text, pixel_t color = 0xffffff)
{
	int text_length = strlen(text);
	for (int offset_x = -(ROW_COUNT * EYES_COUNT); offset_x < text_length * ROW_COUNT; offset_x++) {
		for (int i = 0; i < LED_COUNT; i++) {
			int offset_eye2 = (i >= LED_COUNT_PER_EYE * (EYE2_PORT - 1)) ? EYE2_OFFSET : 0;

			pixel_t pix = 0;
			for (int letter = 0; letter < text_length; letter++) {
				int x = reverse_table[i % LED_COUNT_PER_EYE][0];
				int y = reverse_table[i % LED_COUNT_PER_EYE][1] + offset_x - letter * 6 + offset_eye2;
				sprite_t * sprite_letter = letters[text[letter]];
				bool is_in_matrice = (y >= 0 && y < ROW_COUNT);
				if (is_in_matrice && sprite_letter != NULL){
					if ((*sprite_letter)[x][y]){
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
 * @param int [eye_port = 1] - Port where eye is connected (1 to 8)
 **/
void display_color(pixel_t color, int eye_port = 1)
{
	color = adjust_pixel_luminosity(color);
	for (int i = 0 + LED_COUNT_PER_EYE * (eye_port - 1); i < (LED_COUNT_PER_EYE * eye_port); i++) {
		leds.setPixel(i, color);
	}
}

/**
 * Display single row in one color
 *
 * @param pixel_t color
 * @param uint8_t row
 **/
void display_row(pixel_t color, uint8_t row)
{
	for (int i = 0; i < LED_COUNT; i++) {
		int offset_eye2 = (i >= LED_COUNT_PER_EYE * (EYE2_PORT - 1)) ? EYE2_OFFSET : 0;
		int x = reverse_table[i % LED_COUNT_PER_EYE][0];
		int y = (reverse_table[i % LED_COUNT_PER_EYE][1] - row + offset_eye2);
		bool is_in_matrice = (y >= 0 && y < ROW_COUNT);
		if (is_in_matrice){
			pixel_t pix = (sprite_row)[x][y];
			if (pix) {
				pix = color;
				pix = adjust_pixel_luminosity(pix);
				leds.setPixel(i, pix);
			}
		}
	}
}

/**
 * Display single line in one color
 *
 * @param pixel_t color
 * @param uint8_t line
 * @param int [eye_port = 1] - Port where eye is connected (1 to 8)
 **/
void display_line(pixel_t color, uint8_t line, int eye_port = 1)
{
	for (int i = 0 + LED_COUNT_PER_EYE * (eye_port - 1); i < (LED_COUNT_PER_EYE * eye_port); i++) {
		pixel_t pix = (sprite_line)[(reverse_table[i % LED_COUNT_PER_EYE][0] + line) % LINE_COUNT][reverse_table[i % LED_COUNT_PER_EYE][1]];
		if (pix) pix = color;
		pix = adjust_pixel_luminosity(pix);
		leds.setPixel(i, pix);
	}
}


int rainbowColors[180];

/**
 * Pre-compute rainbow colors
 **/
void precompute_rainbow_colors()
{
	for (int i = 0; i < 180; i++) {
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
	for (color = 0; color < 180; color++) {
		for (x = 0; x < LED_COUNT_PER_EYE; x++) {
			for (y = 0; y < 8; y++) {
				int index = (color + x + y*phaseShift / 2) % 180;
				leds.setPixel(x + y*LED_COUNT_PER_EYE, rainbowColors[index]);
			}
		}
		leds.show();
		delay(wait);
	}
}

/**
 * Clear all leds
**/
void clear()
{
	for (int i = 0; i < LED_COUNT; i++) {
		leds.setPixel(i, 0);
	}
void flash()
{
	for (int i = 0; i < LED_COUNT; i++)
		leds.setPixel(i, 0xFFFFFF);
	leds.show();
}

/**
 * Switch between two sprite with a lid closing transition.
 *
 * @param const sprite_t &sprite_before
 * @param const sprite_t &sprite_after
 **/
void switch_sprite(const sprite_t &sprite_before, const sprite_t &sprite_after)
{
	display_sprite(&sprite_before, EYE1_PORT);
	display_sprite(&sprite_before, EYE2_PORT);
	display_sprite(&lid01, EYE1_PORT);
	display_sprite(&lid01, EYE2_PORT);
	leds.show();
	delay(70);

	display_sprite(&sprite_before, EYE1_PORT);
	display_sprite(&sprite_before, EYE2_PORT);
	display_sprite(&lid02, EYE1_PORT);
	display_sprite(&lid02, EYE2_PORT);
	leds.show();
	delay(70);

	display_sprite(&sprite_before, EYE1_PORT);
	display_sprite(&sprite_before, EYE2_PORT);
	display_sprite(&lid03, EYE1_PORT);
	display_sprite(&lid03, EYE2_PORT);
	leds.show();
	delay(70);

	display_sprite(&sprite_after, EYE1_PORT);
	display_sprite(&sprite_after, EYE2_PORT);
	display_sprite(&lid02, EYE1_PORT);
	display_sprite(&lid02, EYE2_PORT);
	leds.show();
	delay(70);

	display_sprite(&sprite_after, EYE1_PORT);
	display_sprite(&sprite_after, EYE2_PORT);
	display_sprite(&lid01, EYE1_PORT);
	display_sprite(&lid01, EYE2_PORT);
	leds.show();
	delay(70);
}

/**
 * Display a sprite into lid01 sprite
 *
 * @param const sprite_t &sprite
 **/
void display_on_lid(const sprite_t &sprite)
{
	display_sprite(&sprite, EYE1_PORT);
	display_sprite(&sprite, EYE2_PORT);
	display_sprite(&lid01, EYE1_PORT);
	display_sprite(&lid01, EYE2_PORT);
	leds.show();
}

/**
 * Display same sprite on two eyes
 *
 * @param const sprite_t &sprite
 **/
void display_clone(const sprite_t &sprite)
{
	display_sprite(&sprite, EYE1_PORT);
	display_sprite(&sprite, EYE2_PORT);
	leds.show();
}


/**
 * Display k2000 row.
 *
 * @param int i
 **/
void display_k2000_row(int i, int sign = 1)
{
	display_color(0x110000, EYE1_PORT);
	display_color(0x110000, EYE2_PORT);

	//display_row(0xFF0000, MIN(MAX(i, 0), 15));
	display_row(0xFF0000, i);
	display_row(0xCC0000, i - (1 * sign));
	display_row(0xAA0000, i - (2 * sign));
	display_row(0x550000, i - (3 * sign));
	leds.show();
	delay(50);
}


// ANIMATIONS
#define K2000       1
#define HEART       2
#define CLAP        3
#define RAINBOW     4
#define SILON       5
#define EYES        6
#define CLOSE_EYES  7
#define LOOK_AROUND 8
#define STATIC_EYES 9
#define FLASH       10
#define LOOK_LEFT   11
#define LOOK_RIGHT  12

int animNum = 0;

void animation(int anim)
{
	int overlap = 6;
	switch (anim)
	{
	case SILON:
		for (int i = 0; i < 8; i++){
			clear();
			display_row(0xFF0000, i);
			display_row(0xFF0000, (15 - i));
			leds.show();
			delay(80 - i * 3);
		}
		for (int i = 7; i > 0; i--){
			clear();
			display_row(0xFF0000, i);
			display_row(0xFF0000, (15 - i));
			leds.show();
			delay(80 - i * 3);
		}
		break;
	case K2000:
		for (int i = 1 - overlap; i < 16 + overlap; i++) display_k2000_row(i);
		for (int i = 16 + overlap; i > 1 - overlap; i--) display_k2000_row(i, -1);
		break;
	case HEART:
		switch_sprite(iris, heart01);
		display_on_lid(heart02);
		delay(50);
		for (int i = 0; i < 4; i++){
			display_on_lid(heart03);
			delay(200);
			display_on_lid(heart04);
			delay(200);
			display_on_lid(heart03);
			delay(200);
			display_on_lid(heart04);
			delay(1000);
		}
		delay(800);
		switch_sprite(heart04, iris);
		break;

	case CLAP:
		display_clone(clap_open);
		delay(500);
		display_clone(clap_close);
		delay(250);
		display_clone(clap_open);
		delay(500);
		display_text("ACTION");
		break;

	case RAINBOW:
		rainbow(100, 2500);
		break;

	case EYES:
		animation(STATIC_EYES);
		animation(LOOK_AROUND);
		delay(1000);
		animation(CLOSE_EYES);
		animation(CLOSE_EYES);
		break;
	case CLOSE_EYES:
		display_on_lid(iris);
		delay(70);

		display_sprite(&iris, EYE1_PORT);
		display_sprite(&iris, EYE2_PORT);
		display_sprite(&lid02, EYE1_PORT);
		display_sprite(&lid02, EYE2_PORT);
		leds.show();
		delay(70);

		display_sprite(&iris, EYE1_PORT);
		display_sprite(&iris, EYE2_PORT);
		display_sprite(&lid03, EYE1_PORT);
		display_sprite(&lid03, EYE2_PORT);
		leds.show();
		delay(70);

		display_sprite(&iris, EYE1_PORT);
		display_sprite(&iris, EYE2_PORT);
		display_sprite(&lid02, EYE1_PORT);
		display_sprite(&lid02, EYE2_PORT);
		leds.show();
		delay(70);

		display_on_lid(iris);
		delay(70);
		break;
	case LOOK_RIGHT:
		for (int i = 0; i < 3; i++) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(900);
		for (int i = 2; i > -1; i--) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		break;

	case LOOK_LEFT:
		for (int i = 0; i > -3; i--) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(900);
		for (int i = -2; i < 1; i++) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		break;
	case LOOK_AROUND:
		for (int i = 0; i < 3; i++) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		for (int i = 2; i > -3; i--) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		for (int i = -2; i < 1; i++) {
			display_sprite(&iris, EYE1_PORT, i, 0, true);
			display_sprite(&lid01, EYE1_PORT);
			display_sprite(&iris, EYE2_PORT, i, 0, true);
			display_sprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		break;
	case STATIC_EYES:
		display_on_lid(iris);
		delay(1000);
		break;

	case FLASH:
		flash();
		delay(70);
	}

	// Default animation
	if (animNum > 0 && anim == animNum) animNum = EYES;
}

// Serial
char serialInput[100];
int serialPrompt = 0;

/**
 * Capture and parse serial events.
 * SerialEvent() is called after a loop(), if there is serial data in the buffer.
 *
 * Note: This livecycle is given by arduino framework ;)
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
 *   ## Version
 *   V
 *
 *   ## Text
 *   TPLOP -> display "PLOP"
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

	case 'V': // Version - useful for autoconnect
		Serial.println("MIB Eyes");
		break;

	case 'T': // Texte
		Serial.println("Print text");
		display_text(serialInput + 1);
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
	if (animNum > 0){
		animation(animNum);
	} else {
		// Default animation waiting for first serial event
		animation(K2000);
	}
}