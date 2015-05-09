#include <OctoWS2811.h>
#include "eye.h"
#include "spriteLetters.h"
#include "sprites.h"

DMAMEM int displayMemory[LED_COUNT_PER_EYE * 6];
int drawingMemory[LED_COUNT_PER_EYE * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(LED_COUNT_PER_EYE, displayMemory, drawingMemory, config);

/**
 * Apply luminosity percent to a pixel
 *
 * @param  pixel_t pixel - pixel to set
 * @param  int [intensityPercent = 10]
 * @return pixel_t
 **/
pixel_t adjustPixelIntensity(pixel_t pixel, int intensityPercent = 10)
{
	int r = (((pixel >> 16) & 0xFF) * intensityPercent) / 100;
	int g = (((pixel >> 8) & 0xFF)  * intensityPercent) / 100;
	int b = (((pixel >> 0) & 0xFF)  * intensityPercent) / 100;
	return (r << 16) | (g << 8) | (b << 0);
}


const uint8_t reverseTable[52][2] {
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
 * Add a sprite.
 * Note: you must execute `leds.show()` to see the result ;)
 *
 * @param const sprite_t *sprite
 * @param int            [eyePort = 1] - Port where eye is connected (1 to 8)
 * @param int            [offsetX = 0]
 * @param int            [offsetY = 0]
 * @param bool           [loop = false]
 * @param pixel_t        [alphaColor = 0xae8fad] - this color not erase pixel set before (useful for multi-layer management)
 **/
void addSprite(const sprite_t *sprite, int eyePort = 1, int offsetX = 0, int offsetY = 0, bool loop = false, pixel_t alphaColor = 0xae8fad)
{
	for (int i = 0 + LED_COUNT_PER_EYE * (eyePort - 1); i < (LED_COUNT_PER_EYE * eyePort); i++) {
		pixel_t pix;
		int x = reverseTable[i % LED_COUNT_PER_EYE][0] - offsetY;
		int y = reverseTable[i % LED_COUNT_PER_EYE][1] - offsetX;
		bool isInMatrice = (y >= 0 && y < LINE_COUNT && x >= 0 && x < ROW_COUNT);

		if (loop || isInMatrice) {
			pix = (*sprite)[x % LINE_COUNT][y % ROW_COUNT];
		}
		else {
			pix = 0;
		}

		if (pix != alphaColor) {
			pix = adjustPixelIntensity(pix);
			leds.setPixel(i, pix);
		}
	}
}

/**
 * Display a text with left-right scrolling runAnimation.
 * Note: Only caps letters implement for instant.
 *
 * @param char *text
 * @param pixel_t  [color = 0xffffff]
 *
 * Example:
 * printText("ABC");
 *  => Display ABC on eye
 *
 **/
void printText(char * text, pixel_t color = 0xffffff)
{
	int text_length = strlen(text);
	for (int offsetX = -(ROW_COUNT * EYES_COUNT); offsetX < text_length * ROW_COUNT; offsetX++) {
		for (int i = 0; i < LED_COUNT; i++) {
			int offset_eye2 = (i >= LED_COUNT_PER_EYE * (EYE2_PORT - 1)) ? EYE2_OFFSET : 0;

			pixel_t pix = 0;
			for (int letter = 0; letter < text_length; letter++) {
				int x = reverseTable[i % LED_COUNT_PER_EYE][0];
				int y = reverseTable[i % LED_COUNT_PER_EYE][1] + offsetX - letter * 6 + offset_eye2;
				sprite_t * spriteLetter = letters[text[letter]];
				bool isInMatrice = (y >= 0 && y < ROW_COUNT);
				if (isInMatrice && spriteLetter != NULL){
					if ((*spriteLetter)[x][y]){
						pix = color;
					}
				}
			}
			pix = adjustPixelIntensity(pix);
			leds.setPixel(i, pix);
		}
		leds.show();
		delay(100);
	}
}

/**
 * Add a background color.
 * Note: you must execute `leds.show()` to see the result ;)
 *
 * @param pixel_t color
 * @param int [eyePort = 1] - Port where eye is connected (1 to 8)
 **/
void addBackgroundColor(pixel_t color, int eyePort = 1)
{
	color = adjustPixelIntensity(color);
	for (int i = 0 + LED_COUNT_PER_EYE * (eyePort - 1); i < (LED_COUNT_PER_EYE * eyePort); i++) {
		leds.setPixel(i, color);
	}
}

/**
 * Add single row in one color.
 * Note: you must execute `leds.show()` to see the result ;)
 *
 * @param pixel_t color
 * @param uint8_t row
 **/
void addColorRow(pixel_t color, uint8_t row)
{
	for (int i = 0; i < LED_COUNT; i++) {
		int offset_eye2 = (i >= LED_COUNT_PER_EYE * (EYE2_PORT - 1)) ? EYE2_OFFSET : 0;
		int x = reverseTable[i % LED_COUNT_PER_EYE][0];
		int y = (reverseTable[i % LED_COUNT_PER_EYE][1] - row + offset_eye2);
		bool isInMatrice = (y >= 0 && y < ROW_COUNT);
		if (isInMatrice){
			pixel_t pix = (sprite_row)[x][y];
			if (pix) {
				pix = color;
				pix = adjustPixelIntensity(pix);
				leds.setPixel(i, pix);
			}
		}
	}
}

/**
 * Add single line in one color
 * Note: you must execute `leds.show()` to see the result ;)
 *
 * @param pixel_t color
 * @param uint8_t line
 * @param int [eyePort = 1] - Port where eye is connected (1 to 8)
 **/
void addColorLine(pixel_t color, uint8_t line, int eyePort = 1)
{
	for (int i = 0 + LED_COUNT_PER_EYE * (eyePort - 1); i < (LED_COUNT_PER_EYE * eyePort); i++) {
		pixel_t pix = (sprite_line)[(reverseTable[i % LED_COUNT_PER_EYE][0] + line) % LINE_COUNT][reverseTable[i % LED_COUNT_PER_EYE][1]];
		if (pix) {
			pix = color;
			pix = adjustPixelIntensity(pix);
			leds.setPixel(i, pix);
		}
	}
}


int rainbowColors[180];

/**
 * Pre-compute rainbow colors
 **/
void precomputeRainbowColors()
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
 * Clear all leds (buffer only)
**/
void clear()
{
	for (int i = 0; i < LED_COUNT; i++)
		leds.setPixel(i, 0);
}


/**
 * Set and show all leds in white (intensity max!)
 * /!\ Don’t look leds without sunglasses in this mode /!\
**/
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
void switchSprite(const sprite_t &sprite_before, const sprite_t &sprite_after)
{
	addSprite(&sprite_before, EYE1_PORT);
	addSprite(&sprite_before, EYE2_PORT);
	addSprite(&lid01, EYE1_PORT);
	addSprite(&lid01, EYE2_PORT);
	leds.show();
	delay(70);

	addSprite(&sprite_before, EYE1_PORT);
	addSprite(&sprite_before, EYE2_PORT);
	addSprite(&lid02, EYE1_PORT);
	addSprite(&lid02, EYE2_PORT);
	leds.show();
	delay(70);

	addSprite(&sprite_before, EYE1_PORT);
	addSprite(&sprite_before, EYE2_PORT);
	addSprite(&lid03, EYE1_PORT);
	addSprite(&lid03, EYE2_PORT);
	leds.show();
	delay(70);

	addSprite(&sprite_after, EYE1_PORT);
	addSprite(&sprite_after, EYE2_PORT);
	addSprite(&lid02, EYE1_PORT);
	addSprite(&lid02, EYE2_PORT);
	leds.show();
	delay(70);

	addSprite(&sprite_after, EYE1_PORT);
	addSprite(&sprite_after, EYE2_PORT);
	addSprite(&lid01, EYE1_PORT);
	addSprite(&lid01, EYE2_PORT);
	leds.show();
	delay(70);
}

/**
 * Display a sprite into lid01 sprite
 *
 * @param const sprite_t &sprite
 **/
void displayOnLid(const sprite_t &sprite)
{
	addSprite(&sprite, EYE1_PORT);
	addSprite(&sprite, EYE2_PORT);
	addSprite(&lid01, EYE1_PORT);
	addSprite(&lid01, EYE2_PORT);
	leds.show();
}

/**
 * Display same sprite on two eyes
 *
 * @param const sprite_t &sprite
 **/
void displayClone(const sprite_t &sprite)
{
	addSprite(&sprite, EYE1_PORT);
	addSprite(&sprite, EYE2_PORT);
	leds.show();
}


/**
 * Display k2000 rows.
 *
 * @param int i
 **/
void displayK2000Rows(int i, int sign = 1)
{
	addBackgroundColor(0x110000, EYE1_PORT);
	addBackgroundColor(0x110000, EYE2_PORT);

	//addColorRow(0xFF0000, MIN(MAX(i, 0), 15));
	addColorRow(0xFF0000, i);
	addColorRow(0xCC0000, i - (1 * sign));
	addColorRow(0xAA0000, i - (2 * sign));
	addColorRow(0x550000, i - (3 * sign));
	leds.show();
	delay(50);
}


// Animations
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
#define COUNT_DOWN  13
#define PACMAN      14

int animNum = 0;

void runAnimation(int anim)
{
	int overlap = 6;
	switch (anim)
	{
	case SILON:
		for (int i = 0; i < 8; i++){
			clear();
			addColorRow(0xFF0000, i);
			addColorRow(0xFF0000, (15 - i));
			leds.show();
			delay(80 - i * 3);
		}
		for (int i = 7; i > 0; i--){
			clear();
			addColorRow(0xFF0000, i);
			addColorRow(0xFF0000, (15 - i));
			leds.show();
			delay(80 - i * 3);
		}
		break;

	case K2000:
		for (int i = 1 - overlap; i < 16 + overlap; i++) displayK2000Rows(i);
		for (int i = 16 + overlap; i > 1 - overlap; i--) displayK2000Rows(i, -1);
		break;

	case HEART:
		switchSprite(iris, heart01);
		displayOnLid(heart02);
		delay(50);
		for (int i = 0; i < 4; i++){
			displayOnLid(heart03);
			delay(200);
			displayOnLid(heart04);
			delay(200);
			displayOnLid(heart03);
			delay(200);
			displayOnLid(heart04);
			delay(1000);
		}
		delay(800);
		switchSprite(heart04, iris);
		break;

	case CLAP:
		displayClone(clap_open);
		delay(500);
		displayClone(clap_close);
		delay(250);
		displayClone(clap_open);
		delay(500);
		printText("ACTION");
		break;

	case RAINBOW:
		rainbow(100, 2500);
		break;

	case EYES:
		runAnimation(STATIC_EYES);
		runAnimation(LOOK_AROUND);
		delay(1000);
		runAnimation(CLOSE_EYES);
		runAnimation(CLOSE_EYES);
		break;

	case CLOSE_EYES:
		displayOnLid(iris);
		delay(70);

		addSprite(&iris, EYE1_PORT);
		addSprite(&iris, EYE2_PORT);
		addSprite(&lid02, EYE1_PORT);
		addSprite(&lid02, EYE2_PORT);
		leds.show();
		delay(70);

		addSprite(&iris, EYE1_PORT);
		addSprite(&iris, EYE2_PORT);
		addSprite(&lid03, EYE1_PORT);
		addSprite(&lid03, EYE2_PORT);
		leds.show();
		delay(70);

		addSprite(&iris, EYE1_PORT);
		addSprite(&iris, EYE2_PORT);
		addSprite(&lid02, EYE1_PORT);
		addSprite(&lid02, EYE2_PORT);
		leds.show();
		delay(70);

		displayOnLid(iris);
		delay(70);
		break;

	case LOOK_RIGHT:
		for (int i = 0; i < 3; i++) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(900);
		for (int i = 2; i > -1; i--) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		break;

	case LOOK_LEFT:
		for (int i = 0; i > -3; i--) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(900);
		for (int i = -2; i < 1; i++) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		break;

	case LOOK_AROUND:
		for (int i = 0; i < 3; i++) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		for (int i = 2; i > -3; i--) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		delay(200);
		for (int i = -2; i < 1; i++) {
			addSprite(&iris, EYE1_PORT, i, 0, true);
			addSprite(&lid01, EYE1_PORT);
			addSprite(&iris, EYE2_PORT, i, 0, true);
			addSprite(&lid01, EYE2_PORT);
			leds.show();
			delay(100);
		}
		break;

	case STATIC_EYES:
		displayOnLid(iris);
		delay(1000);
		break;

	case FLASH:
		flash();
		delay(70);
		break;

	case COUNT_DOWN:
		for (int i = 5; i > -1; i--){
			addBackgroundColor(rainbowColors[i * 10], EYE1_PORT);
			addBackgroundColor(rainbowColors[i * 10], EYE2_PORT);
			addSprite(letters[i], EYE1_PORT, 0, 0, false, 0xFFFFFF);
			addSprite(letters[i], EYE2_PORT, 0, 0, false, 0xFFFFFF);
			leds.show();
			delay(1000);
		}
		break;

	case PACMAN:
		overlap = 0;
		for (int i = -10; i < 16; i++){

			// Ghosts
			// TODO method to colorize sprite without using background color
			//for (int j = 0; j < 3; j++) {
			//	addSprite(&ghost, EYE1_PORT, 7 * j + 10, 0, false, 0x000000);
			//	addSprite(&ghost, EYE1_PORT, 7 * j + 2, 0, false, 0x000000);
			//}

			// Dots
			clear(); // TODO delete this when ghosts are functionnal
			if (i % 4 == 0 && i > -5) overlap++;
			for (int j = 0; j < 4; j++) {
				addSprite(&dot, EYE1_PORT, (4 * overlap) + j * 4, 0, false, 0x000000);
				addSprite(&dot, EYE2_PORT, (4 * overlap) + j * 4 - 8, 0, false, 0x000000);
			}

			// Pacman
			if (i % 2) {
				addSprite(&pacman_open, EYE1_PORT, i, 0, false, 0x000000);
				addSprite(&pacman_open, EYE2_PORT, i - 8, 0, false, 0x000000);
			}
			else {
				addSprite(&pacman_close, EYE1_PORT, i, 0, false, 0x000000);
				addSprite(&pacman_close, EYE2_PORT, i - 8, 0, false, 0x000000);
			}

			leds.show();
			delay(200);
		}
		break;
	}

	// Default runAnimation
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
 *   ## runAnimations
 *   A01 -> runAnimation 1
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
	case 'A': // runAnimation - `A01` for run animation 1
		animNum = serialInput[1] * 10 + serialInput[2];
		Serial.print("Run animation ");
		Serial.println(animNum);
		break;

	case 'V': // Version - useful for autoconnect
		Serial.println("MIB Eyes");
		break;

	case 'T': // Texte
		Serial.println("Print text");
		printText(serialInput + 1);
		break;

	default:
		Serial.print("Unkwnown command (");
		Serial.print(serialInput[0]);
		Serial.println(")");
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

	precomputeRainbowColors();
	Serial.begin(9600);
}


void loop()
{
	if (animNum > 0){
		runAnimation(animNum);
	} else {
		// Default animation waiting for first serial event
		runAnimation(PACMAN);
		//for (int i = 0; i < 16; i++){
		//	addBackgroundColor(rainbowColors[i * 5], EYE1_PORT);
		//	addBackgroundColor(rainbowColors[i * 5], EYE2_PORT);
		//	addSprite(&dot, EYE1_PORT, i, 0, false, 0xFFFFFF);
		//	addSprite(&dot, EYE2_PORT, i - 8, 0, false, 0xFFFFFF);
		//	leds.show();
		//	delay(20);
		//}
	}
}