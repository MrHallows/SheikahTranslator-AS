/*
    Name:       SheikahTranslator.ino
    Created:	07/02/18 21:49:11
    Author:     Andrew Gibson
	License:	MIT (see https://github.com/MrHallows/SheikahTranslator-AS/blob/master/LICENSE)
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Find & Replace patterns for reformatting new GLCD font files:
 * NOTE: The last line will need to be done manually!
 * Find:	(\n\s+)(.*?)(,\s+\/\/)
 * Replace:	$1{ $2 }$3
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Equation for centering text:
 * L = length of text (i.e., "Main Menu" is 9 characters * 6 = 54
 * W = Screen Width (128)
 * 
 * (W / 2) - (L * 6 / 2) = X position of text
 * Ex.: 64 - 27 = 37 => print6x8Str(37, 0, "Main Menu");
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "src/OLED_SSD1306.h"


OLED_SSD1306 OLED;


void setup()
{
	OLED.initDisplay();
	OLED.loadSplash();
	OLED.loadTitle();
	OLED.mainMenu();
}

void loop() { }
