/* *
 * Class for interfacing with the SSD1306 OLED display
 */

#include "OLED_SSD1306.h"
#include <SPI.h>


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 								Display
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Initialize pins
void OLED_SSD1306::initPins(void)
{
	pinMode(RST_PIN, OUTPUT);
	pinMode(DC_PIN, OUTPUT);

	pinMode(BTN_UP_PIN, INPUT);
	pinMode(BTN_DOWN_PIN, INPUT);
	pinMode(BTN_LEFT_PIN, INPUT);
	pinMode(BTN_RIGHT_PIN, INPUT);
	pinMode(BTN_A_PIN, INPUT);
	pinMode(BTN_B_PIN, INPUT);
}


// Write Command
void OLED_SSD1306::writeCmd(unsigned char cmd)
{
	digitalWrite(DC_PIN, LOW);
	SPI.transfer(cmd);
}


// Write Data
void OLED_SSD1306::writeData(unsigned char data)
{
	digitalWrite(DC_PIN, HIGH);
	SPI.transfer(cmd);
}


// Clear the screen
void OLED_SSD1306::cls(void)
{
	unsigned char x, y;

	for(y = 0; y < 16; y++)
	{
		for(x = 0; x < OLED_WIDTH; x++)
			writeData(0x00);
	}
}


// Fill the screen
void OLED_SSD1306::fill(unsigned char data)
{
	unsigned char y, x;

	for(y = 0; y < 8; y++)
	{
		writeCmd(0xB0 + y);
		writeCmd(0x00);
		writeCmd(0x10);

		for(x = 0; x < OLED_WIDTH; x++)
			writeData(data);
	}
}


// Clear the buffer
inline void OLED_SSD1306::clearBuffer()
{
	// Set full buffer (1024 bytes) to 0
	memset(buffer, 0, sizeof(buffer));
}


// Flush the buffer to the display
void OLED_SSD1306::flushBuffer(void)
{
	int i = 0, j = 0;
	
	setPosition(0, 0);

	for(i = 0; i < 1024; i++)
	{
		// Transmit data to the display
		writeData(buffer[i]);
	}
}


// Set Position
void OLED_SSD1306::setPosition(unsigned char x, unsigned char y)
{
	writeCmd(0xB0 | y);
	writeCmd(((x & 0xf0) >> 4) | 0x10);
	writeCmd(x & 0x0f);
}


// Load char array data from PROGMEM
void OLED_SSD1306::loadSplash(void)
{
	clearBuffer();

	int i = 0;

	for(i = 0; i < 1024; i++)
		buffer[i] = pgm_read_byte(&(SheikahEyeSplash[i]));

	flushBuffer();

	while (digitalRead(BTN_A_PIN) == LOW && digitalRead(BTN_B_PIN) == LOW) { }
}


// Display the title screen
void OLED_SSD1306::loadTitle(void)
{
	clearBuffer();
	
	drawRect(0, 5, 127, 18, 0);
	drawPixel(53, 51);
	drawPixel(60, 51);
	drawPixel(67, 51);
	drawPixel(74, 51);

	print6x8Str(12, 1, "Sheikah Translator");
	print6x8Str(0, 4, "\"It's dangerous to go");
	print6x8Str(6, 5, "alone! Take this.\"");
	print6x8Str(0, 7, "Happy Birthday, Matt!");
	
	flushBuffer();

	// Wait for A or B button to be pressed
	while (digitalRead(BTN_A_PIN) == LOW && digitalRead(BTN_B_PIN) == LOW) { }
}


// 
void OLED_SSD1306::mainMenu(void)
{
	currentMenu = 4;
	unsigned char selectorX = 0;
	prevSelectorPosX = 0;
	prevSelectorPosY = 4;

	clearBuffer();
	
	drawRect(0, 5, 127, 18, 0);

	//setPosition(0, 0);
	print6x8Str(37, 1, "Main Menu");
	setSelector(selectorX, currentMenu);
	print6x8Str(6, 4, "Translator");
	print6x8Str(6, 5, "Settings");
	print6x8Str(6, 6, "Graphics Test");
	
	flushBuffer();

	// Wait for a selection to be made
	while (1)
	{
		// Up
		if (digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevMenu = currentMenu;
			// Decrement selectorPosY
			if (currentMenu <= 4) {
				currentMenu = 6;
				prevMenu = 4;
			}
			else
				currentMenu--;

			//clearPrevSelector();
			unsigned char i = 0;

			setPosition(selectorX, prevMenu);

			for(i = 0; i < 6; i++)
				writeData(pgm_read_byte(&(SelectorBlank[i])));

			setSelector(selectorX, currentMenu);
			delay(200);
		}

		// Down
		if (digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevMenu = currentMenu;
			// Increment selectorPosY
			if (currentMenu >= 6) {
				currentMenu = 4;
				prevMenu = 6;
			}
			else
				currentMenu++;

			//clearPrevSelector();
			unsigned char i = 0;

			setPosition(selectorX, prevMenu);

			for(i = 0; i < 6; i++)
				writeData(pgm_read_byte(&(SelectorBlank[i])));

			setSelector(selectorX, currentMenu);
			delay(200);
		}

		// A
		if (digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			if (currentMenu == 4) {
				return printSheikahMap();
			}
			if (currentMenu == 5) {
				return settingsMenu();
			}
			if (currentMenu == 6) {
				return graphicsTest();
			}
			delay(200);
		}
	}
}


// Settings Menu
void OLED_SSD1306::settingsMenu(void)
{
	currentMenu = 4;
	unsigned char selectorX = 0;
	prevSelectorPosX = 0;
	prevSelectorPosY = 4;

	clearBuffer();
	
	drawRect(0, 5, 127, 18, 0);

	print6x8Str(40, 1, "Settings");
	setSelector(selectorX, currentMenu);
	print6x8Str(6, 4, "Contrast");
	print6x8Str(6, 5, "Setting 2");
	print6x8Str(6, 6, "Setting 3");
	
	flushBuffer();

	// Wait for a selection to be made
	while (1)
	{
		// Up
		if (digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevMenu = currentMenu;
			// Decrement selectorPosY
			if (currentMenu <= 4) {
				currentMenu = 6;
				prevMenu = 4;
			}
			else
			currentMenu--;

			//clearPrevSelector();
			unsigned char i = 0;

			setPosition(selectorX, prevMenu);

			for(i = 0; i < 6; i++)
			writeData(pgm_read_byte(&(SelectorBlank[i])));

			setSelector(selectorX, currentMenu);
			delay(200);
		}

		// Down
		if (digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevMenu = currentMenu;
			// Increment selectorPosY
			if (currentMenu >= 6) {
				currentMenu = 4;
				prevMenu = 6;
			}
			else
			currentMenu++;

			//clearPrevSelector();
			unsigned char i = 0;

			setPosition(selectorX, prevMenu);

			for(i = 0; i < 6; i++)
			writeData(pgm_read_byte(&(SelectorBlank[i])));

			setSelector(selectorX, currentMenu);
			delay(200);
		}

		// A
		if (digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			if (currentMenu == 4) {
				return setContrast();
			}
			if (currentMenu == 5) {
				return settingsMenu();
			}
			if (currentMenu == 6) {
				return graphicsTest();
			}
			delay(200);
		}

		// B
		if (digitalRead(BTN_B_PIN) == HIGH) {
			return mainMenu();
		}
	}
}


// Set Contrast
void OLED_SSD1306::setContrast(void)
{
	//! This is causing the display to freeze for some reason..
	Serial.println("Data was successfully written to EEPROM!");

	clearBuffer();
	
	drawRect(0, 5, 127, 18, 0);

	// Draw the progress bar
	drawRect(0, 52, 127, 58, 0);
	drawRect(2, 54, (contrast / 2), 56, 0);
	//drawRect(3, 54, 3, 56, 0);

	print6x8Str(40, 1, "Contrast");
	print6x8Str(43, 4, "<");
	//printValueI(18, 4, contrast);
	print6x8Str(79, 4, ">");
	
	flushBuffer();

	// Wait for a selection to be made
	while (1)
	{
		// Left
		if (digitalRead(BTN_LEFT_PIN) == HIGH) {
			// Decrement contrast
			if (contrast <= 0xA) {
				contrast = 0xA;
				contrastBar = 4;
			}
			else {
				contrast -= 10;
				contrastBar = contrast / 2;
			}
			
			drawRect(2, 54, 125, 56, 0, OFF);
			drawRect(2, 54, contrastBar, 56, 0, ON);
			drawRect(0, 52, 127, 58, 0);
			flushBuffer();

			setContrastControl(contrast);
		}

		// Right
		if (digitalRead(BTN_RIGHT_PIN) == HIGH) {
			// Increment contrast
			if (contrast >= 0xFA) {
				contrast = 0xFA;
				contrastBar = 125;
			}
			else {
				contrast += 10;
				contrastBar = contrast / 2;
			}
			drawRect(2, 54, 125, 56, 0, OFF);
			drawRect(2, 54, contrastBar, 56, 0, ON);
			drawRect(0, 52, 127, 58, 0);
			flushBuffer();

			setContrastControl(contrast);
		}

		// B
		if (digitalRead(BTN_B_PIN) == HIGH) {
			return settingsMenu();
		}

		printShortValueI(49, 4, contrast / 10, 2);
	}
}


// Set Delay
void OLED_SSD1306::setDelay(unsigned int ms)
{
	// TODO: Create a timer that is interruptible
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 								Text
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Print Shard_6x8 Char
void OLED_SSD1306::print6x8Char(unsigned char x, unsigned char y, unsigned char ch)
{
	unsigned char c = 0, i = 0, j = 0;
	
	c = ch - 32;
	if (x > 122) {
		x = 0;
		y++;
	}
	
	setPosition(x, y);
	for(i = 0; i < 6; i++) {
		buffer[x + (y * 128) + i] = pgm_read_byte(&(Shard_6x8[c][i]));
		//writeData(pgm_read_byte(&(Shard_6x8[c][i])));
	}
}


// Print Shard_6x8 String
void OLED_SSD1306::print6x8Str(unsigned char x, unsigned char y, const char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > OLED_WIDTH) {
			x = 0;
			y++;
		}
		
		setPosition(x, y);
		for(i = 0; i < 6; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(Shard_6x8[c][i]));
			//writeData(pgm_read_byte(&(Shard_6x8[c][i])));
		}
		x += 6;
		j++;
	}
}


// Print Single Shard_6x8 Character
void OLED_SSD1306::print6x8Single(unsigned char x, unsigned char y, char ch)
{
	unsigned char c = 0, i = 0, j = 0;
	
	if (ch != '\0')
	{
		c = ch - 32;
		if (x > OLED_WIDTH) {
			x = 0;
			y++;
		}
		
		setPosition(x, y);
		for(i = 0; i < 6; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(Shard_6x8[c][i]));
			//writeData(pgm_read_byte(&(Shard_6x8[c][i])));
		}
		x += 6;
	}
}


// Print Sheikah_8x8 String
void OLED_SSD1306::print8x8Str(unsigned char x, unsigned char y, const char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > OLED_WIDTH - 8) {
			x = 0;
			y++;
		}
		
		setPosition(x, y);
		for(i = 0; i < 8; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(Sheikah_8x8[c][i]));
			//writeData(pgm_read_byte(&(Sheikah_8x8[c][i])));
		}
		x += 9;
		j++;
	}
}


// Print Single Sheikah_8x8 Character
void OLED_SSD1306::print8x8Single(unsigned char x, unsigned char y, char ch)
{
	unsigned char c = 0, i = 0, j = 0;
	
	if (ch != '\0')
	{
		c = ch - 32;
		if (x > OLED_WIDTH - 8) {
			x = 0;
			y++;
		}
		
		setPosition(x, y);
		for(i = 0; i < 8; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(Sheikah_8x8[c][i]));
			//writeData(pgm_read_byte(&(Sheikah_8x8[c][i])));
		}
		x += 9;
	}
}


// Print F8x16 String
void OLED_SSD1306::print8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 120) {
			x = 0;
			y++;
		}

		setPosition(x, y);
		for(i = 0; i < 8; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(F8X16[(c << 4) + i]));
			//writeData(pgm_read_byte(&(F8X16[(c << 4) + i])));
		}

		setPosition(x, y + 1);
		for(i = 0; i < 8; i++) {
			buffer[x + (y * 128) + i] = pgm_read_byte(&(F8X16[(c << 4) + i + 8]));
			//writeData(pgm_read_byte(&(F8X16[(c << 4) + i + 8])));
		}
		x += 8;
		j++;
	}
}


// 
void OLED_SSD1306::printValueC(unsigned char x, unsigned char y, char data)
{
	unsigned char i, j, k;
	
	if (data < 0) {
		print6x8Char(x, y, '-');
		data = -data;
	}
	else
		print6x8Char(x, y, '+');

	i = data / 100;
	j = (data % 100) / 10;
	k = data % 10;
	
	print6x8Char(x + 6, y, i + 48);
	print6x8Char(x + 12, y, j + 48);
	print6x8Char(x + 18, y, k + 48);
}


// 
void OLED_SSD1306::printValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i, j, k, l, m;
	
	if (data < 0) {
		print6x8Char(x, y, '-');
		data = -data;
	}
	else
		print6x8Char(x, y, '+');

	l = data / 10000;
	m = (data % 10000) / 1000;
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	
	print6x8Char(x + 6, y, l + 48);
	print6x8Char(x + 12, y, m + 48);
	print6x8Char(x + 18, y, i + 48);
	print6x8Char(x + 24, y, j + 48);
	print6x8Char(x + 30, y, k + 48);
}


// 
void OLED_SSD1306::printShortValueI(unsigned char x, unsigned char y, int data, unsigned char digits)
{
	unsigned char i, j, k, l, m;
	
	if (data < 0) {
		print6x8Char(x, y, '-');
		data = -data;
	}

	l = data / 10000;
	m = (data % 10000) / 1000;
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	
	switch(digits)
	{
		case 0:
			// Nothing to print
			break;
		case 1:
			print6x8Char(x + 6, y, k + 48);
			break;
		case 2:
			print6x8Char(x + 6, y, j + 48);
			print6x8Char(x + 12, y, k + 48);
			break;
		case 3:
			print6x8Char(x + 6, y, i + 48);
			print6x8Char(x + 12, y, j + 48);
			print6x8Char(x + 18, y, k + 48);
			break;
		case 4:
			print6x8Char(x + 6, y, m + 48);
			print6x8Char(x + 12, y, i + 48);
			print6x8Char(x + 18, y, j + 48);
			print6x8Char(x + 24, y, k + 48);
			break;
		case 5:
			print6x8Char(x + 6, y, l + 48);
			print6x8Char(x + 12, y, m + 48);
			print6x8Char(x + 18, y, i + 48);
			print6x8Char(x + 24, y, j + 48);
			print6x8Char(x + 30, y, k + 48);
			break;
	}
}


// 
void OLED_SSD1306::printValueF(unsigned char x, unsigned char y, float data, unsigned char num)
{
	unsigned char l, m, i, j, k;
	unsigned char databiti = 6;
	unsigned int tempdataui = 0;
	int tempdataii = (int)data;
	long int tempdatalp = (long int)((data - (int)data) * 10000);

	if (data < 0.0000001)
		print6x8Char(x, y, '-');
	else
		print6x8Char(x, y, '+');
		
	if (tempdataii < 0)
		tempdataii = -tempdataii;
		
	tempdataui = tempdataii;
	l = tempdataui / 10000;
	m = (tempdataui % 10000) / 1000;
	i = (tempdataui % 1000) / 100;
	j = (tempdataui % 100) / 10;
	k = tempdataui % 10;
	
	if (l != 0) {
		print6x8Char(x + 6, y, l + 48);
		print6x8Char(x + 12, y, m + 48);
		print6x8Char(x + 18, y, i + 48);
		print6x8Char(x + 24, y, j + 48);
		print6x8Char(x + 30, y, k + 48);
	}
	else if (m != 0) {
		databiti = 5;
		print6x8Char(x + 6, y, m + 48);
		print6x8Char(x + 12, y, i + 48);
		print6x8Char(x + 18, y, j + 48);
		print6x8Char(x + 24, y, k + 48);
	}
	else if (i != 0) {
		databiti = 4;
		print6x8Char(x + 6, y, i + 48);
		print6x8Char(x + 12, y, j + 48);
		print6x8Char(x + 18, y, k + 48);
	}
	else if (j != 0) {
		databiti = 3;
		print6x8Char(x + 6, y, j + 48);
		print6x8Char(x + 12, y, k + 48);
	}
	else {
		databiti = 2;
		print6x8Char(x + 6, y, k + 48);
	}
	
	if (tempdatalp < 0)
		tempdatalp = -tempdatalp;
		
	switch(num)
	{
		case 0:
			// Nothing to print
			break;
		case 1:
			printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000), num);
			break;
		case 2:
			printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 100), num);
			break;
		case 3:
			printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 10), num);
			break;
		case 4:
			printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp), num);
			break;
	}
}


// 
void OLED_SSD1306::printValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
{
	unsigned char m, i, j, k;
	print6x8Char(x, y, '.');
	
	m = data / 1000;
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	
	switch(num)
	{
		case 0:
			// Nothing to print
			break;
		case 1:
			print6x8Char(x + 6, y, k + 48);
			break;
		case 2:
			print6x8Char(x + 6, y, j + 48);
			print6x8Char(x + 12, y, k + 48);
			break;
		case 3:
			print6x8Char(x + 6, y, i + 48);
			print6x8Char(x + 12, y, j + 48);
			print6x8Char(x + 18, y, k + 48);
			break;
		case 4:
			print6x8Char(x + 6, y, m + 48);
			print6x8Char(x + 12, y, i + 48);
			print6x8Char(x + 18, y, j + 48);
			print6x8Char(x + 24, y, k + 48);
			break;
	}
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 							Graphics
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// Draw Pixel (set status on/off)
void OLED_SSD1306::drawPixel(int x, int y, int status)
{
	if ((x < 0) || (x >= OLED_WIDTH) || (y < 0) || (y >= OLED_HEIGHT)) // Check for boundaries
		return;

	switch(status)
	{
		case ON:
			buffer[x + (y / 8) * OLED_WIDTH] |=  (1 << (y & 7)); // Store pixel data in the buffer
			break;
		case OFF:
			buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7)); // Store pixel data in the buffer
			break;
		case INVERSE:
			buffer[x + (y / 8) * OLED_WIDTH] ^=  (1 << (y & 7)); // Store pixel data in the buffer
			break;
	}
}


// Draw Line
// Bresenham's line drawing algorithm
void OLED_SSD1306::drawLine(int x1, int y1, int x2, int y2, int spacing, int status)
{
	int cx = x1;
	int cy = y1;
	int dx = x2 - cx;
	int dy = y2 - cy;

	if (dx < 0)
		dx = 0 - dx;
	if (dy < 0)
		dy = 0 - dy;

	int sx = 0;
	int sy = 0;

	if (cx < x2)
		sx = 1;
	else
		sx = -1;
	
	if (cy < y2)
		sy = 1;
	else
		sy = -1;
	

	int err = dx - dy;
	
	for(int n = 0; n < 1000; n++)
	{
		drawPixel(cx, cy, status);

		if ((cx == x2) && (cy == y2))
			return;

		int e2 = 2 * err;

		if (e2 > (0 - dy))
		{
			err = err - dy;
			cx = cx + sx;
			cx += spacing;
		}
		if (e2 < dx)
		{
			err = err + dx;
			cy = cy + sy;
			cy += spacing;
		}
	}
}


// Draw Rectangle
void OLED_SSD1306::drawRect(int x1, int y1, int x2, int y2, int spacing, int status)
{
	int x, y;
	
	// Alternative method using drawLine(...) (works perfectly)
	drawLine(x1, y1, x2 - 1, y1, spacing, status); // Top
	drawLine(x2, y1, x2, y2 - 1, spacing, status); // Right
	drawLine(x2, y2 - 1, x1 + 1, y2 - 1, spacing, status); // Bottom
	drawLine(x1, y2 - 1, x1, y1 + 1, spacing, status); // Left
}


// Draw Dot Grid
void OLED_SSD1306::drawDotGrid(int x1, int y1, int x2, int y2, int spacing, int status)
{
	// TODO: Write it already
}


// Draw Line Grid
void OLED_SSD1306::drawLineGrid(int x1, int y1, int x2, int y2, int spacing, int status)
{
	// TODO: Write it already
}


// Draw Circle
void OLED_SSD1306::drawCircle(int x0, int y0, int radius, int status)
{
	int x = radius;
	int y = 0;
	int de = 1 - x;
 
	while (x >= y)
	{
		drawPixel(x + x0, y + y0, status);   // First octant
		drawPixel(y + x0, x + y0, status);   // Second octant
		drawPixel(-y + x0, x + y0, status);  // Third octant
		drawPixel(-x + x0, y + y0, status);  // Fourth octant
		drawPixel(-x + x0, -y + y0, status); // Fifth octant
		drawPixel(-y + x0, -x + y0, status); // Sixth octant
		drawPixel(y + x0, -x + y0, status);  // Seventh octant
		drawPixel(x + x0, -y + y0, status);  // Eighth octant
		y++;

		if (de <= 0)
			de += 2 * y + 1;
		else {
			x--;
			de += 2 * (y - x) + 1;
		}
	}
}


// Draw Bitmap Image
void OLED_SSD1306::drawBitmap(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char bitmap_array[])
{
	unsigned char i = 0;
	unsigned char x, y;
	y1 /= 8;
	
	clearBuffer();
	setPosition(0, 0);

	for(y = y0; y < y1; y++)
	{
		setPosition(x0, y);				
		for(x = x0; x < x1; x++) {
			buffer[i++] = bitmap_array[i++];
			//writeData(bitmap_array[i++]);
		}
	}
}


// Graphics Test
void OLED_SSD1306::graphicsTest(void)
{
	clearBuffer();
	print6x8Str(0, 0, "Graphics test..");
	flushBuffer();
	delay(3000);	

	// Draw pixel
	clearBuffer();
	print6x8Str(0, 0, "Draw pixel..");
	flushBuffer();
	delay(2000);
	clearBuffer();
	drawPixel(63, 31);
	flushBuffer();
	delay(3000);

	// Draw line
	clearBuffer();
	print6x8Str(0, 0, "Draw line..");
	flushBuffer();
	delay(2000);
	clearBuffer();
	drawLine(0, 31, 127, 31, 0);
	flushBuffer();
	delay(3000);

	// Draw rectangle
	clearBuffer();
	print6x8Str(0, 0, "Draw rectangle..");
	flushBuffer();
	delay(2000);
	clearBuffer();
	drawRect(0, 0, 127, 63, 0);
	flushBuffer();
	delay(3000);

	// Draw circle
	clearBuffer();
	print6x8Str(0, 0, "Draw circle..");
	flushBuffer();
	delay(2000);
	clearBuffer();
	drawCircle(63, 31, 20);
	flushBuffer();
	delay(3000);

	return mainMenu();
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 						Sheikah Character Map
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Display the Sheikah Character Map
// TODO: Create a for loop to display this [hopefully] using less memory.
void OLED_SSD1306::printSheikahMap(void)
{
	selectorPosX = 0;
	selectorPosY = 0;
	cursorPosX = 1;
	cursorPosY = 7;

	clearBuffer();
	drawLine(0, 35, 127, 35, 0);
	drawLine(0, 54, 127, 54, 0);
	flushBuffer();

	setSelectorPos(selectorPosX, selectorPosY);
	setCursor(cursorPosX * 6, cursorPosY);
	print6x8Str(0, 7, ">");

	print8x8Str(6, 0, "A");
	print8x8Str(24, 0, "B");
	print8x8Str(42, 0, "C");
	print8x8Str(60, 0, "D");
	print8x8Str(78, 0, "E");
	print8x8Str(96, 0, "F");
	print8x8Str(114, 0, "G");

	print8x8Str(6, 1, "H");
	print8x8Str(24, 1, "I");
	print8x8Str(42, 1, "J");
	print8x8Str(60, 1, "K");
	print8x8Str(78, 1, "L");
	print8x8Str(96, 1, "M");
	print8x8Str(114, 1, "N");

	print8x8Str(6, 2, "O");
	print8x8Str(24, 2, "P");
	print8x8Str(42, 2, "Q");
	print8x8Str(60, 2, "R");
	print8x8Str(78, 2, "S");
	print8x8Str(96, 2, "T");
	print8x8Str(114, 2, "U");

	print8x8Str(6, 3, "V");
	print8x8Str(24, 3, "W");
	print8x8Str(42, 3, "X");
	print8x8Str(60, 3, "Y");
	print8x8Str(78, 3, "Z");
	print8x8Str(96, 3, " ");
	print8x8Str(114, 3, ".");
	flushBuffer();

	while (1)
	{
		// Up
		if (digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevSelectorPosY = selectorPosY;
			// Decrement selectorPosY
			if (selectorPosY <= 0) {
				selectorPosY = 3;
				prevSelectorPosY = 0;
			}
			else
				selectorPosY--;

			clearPrevSelector();
			setSelectorPos(selectorPosX, selectorPosY);
			delay(200);
		}

		// Down
		if (digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			prevSelectorPosY = selectorPosY;
			// Increment selectorPosY
			if (selectorPosY > 3) {
				selectorPosY = 0;
				prevSelectorPosY = 3;
			}
			else
				selectorPosY++;

			clearPrevSelector();
			setSelectorPos(selectorPosX, selectorPosY);
			delay(200);
		}

		// Left
		if (digitalRead(BTN_LEFT_PIN) == HIGH) {
			// Store prevSelectorPosX
			prevSelectorPosX = selectorPosX;
			// Decrement selectorPosX
			if (selectorPosX <= 0) {
				selectorPosX = 6;
				prevSelectorPosX = 0;
			}
			else
				selectorPosX--;

			clearPrevSelector();
			setSelectorPos(selectorPosX, selectorPosY);
			delay(200);
		}

		// Right
		if (digitalRead(BTN_RIGHT_PIN) == HIGH) {
			// Store prevSelectorPosX
			prevSelectorPosX = selectorPosX;
			// Increment selectorPosX
			if (selectorPosX > 6) {
				selectorPosX = 0;
				prevSelectorPosX = 6;
			}
			else
				selectorPosX++;

			clearPrevSelector();
			setSelectorPos(selectorPosX, selectorPosY);
			delay(200);
		}

		// A
		if (digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			prevCursorPosX = cursorPosX - 1;
			// Increment selectorPosX
			if (cursorPosX >= 14) { //20
				cursorPosX = 14; //20
				prevCursorPosX = 13; //19
			}
			else {
				cursorPosX++;
			}
			//clearPrevCursor();
			setCursorPos(cursorPosX, cursorPosY);
			print6x8Single(prevCursorPosX * 6, 7, getSelectedChar());
			print8x8Single(prevCursorPosX * 9, 5, getSelectedChar());
			//cursorPosX++;
			//setCursor(cursorPosX * 6, 7);
			delay(200);
		}

		// B
		if (digitalRead(BTN_B_PIN) == HIGH) {
			//clearLine();
			backspace();
			backspace_8x8();
		
			// Store prevCursorPosX
			prevCursorPosX = cursorPosX + 1;
		
			delay(200);
		}

		// B && Up
		if (digitalRead(BTN_B_PIN) == HIGH && digitalRead(BTN_UP_PIN) == HIGH) {
			// Switch to main menu
			clearBuffer();
			return mainMenu();
		
			delay(120);
		}
	}
}


// Print Sheikah Number Map
void OLED_SSD1306::printSheikahNums(void)
{
	clearBuffer();
	drawLine(0, 35, 127, 35, 0);
	drawLine(0, 54, 127, 54, 0);
	flushBuffer();
	setSelectorPos(selectorPosX, selectorPosY);
	setCursor(cursorPosX * 6, cursorPosY);

	print8x8Str(6, 1, "");
	print8x8Str(24, 1, "0");
	print8x8Str(42, 1, "1");
	print8x8Str(60, 1, "2");
	print8x8Str(78, 1, "3");
	print8x8Str(96, 1, "4");
	print8x8Str(114, 1, "");

	print8x8Str(6, 2, "");
	print8x8Str(24, 2, "5");
	print8x8Str(42, 2, "6");
	print8x8Str(60, 2, "7");
	print8x8Str(78, 2, "8");
	print8x8Str(96, 2, "9");
	print8x8Str(114, 2, "");
}


// Set Selector
void OLED_SSD1306::setSelector(unsigned char x, unsigned char y)
{
	unsigned char c = 0, i = 0, j = 0;

	clearPrevSelector();

	if (x > 114) {
		x = 0;
		y++;
	}
	setPosition(x, y);
	for(i = 0; i < 6; i++)
		writeData(pgm_read_byte(&(Selector[i])));
}


// Set Selector Position
void OLED_SSD1306::setSelectorPos(unsigned char x, unsigned char y)
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 * Sheikah Map Character Positions:
	 * +---------------------------------------------------------------------+
	 * | A(0, 0) | B(1, 0) | C(2, 0) | D(3, 0) | E(4, 0) | F(5, 0) | G(6, 0) |
	 * | H(0, 1) | I(1, 1) | J(2, 1) | K(3, 1) | L(4, 1) | M(5, 1) | N(6, 1) |
	 * | O(0, 2) | P(1, 2) | Q(2, 2) | R(3, 2) | S(4, 2) | T(5, 2) | U(6, 2) |
	 * | V(0, 3) | W(1, 3) | X(2, 3) | Y(3, 3) | Z(4, 3) |  (5, 3) | .(6, 3) |
	 * +---------------------------------------------------------------------+
	 * 
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	prevSelectorPosX = selectorPosX;
	prevSelectorPosY = selectorPosY;
	if (x > 6) {
		prevSelectorPosX = 0;
		x = 0;
	}
	else if (x < 0) {
		prevSelectorPosX = 6;
		x = 6;
	}
	
	if (y > 3) {
		prevSelectorPosY = 0;
		y = 0;
	}
	else if (y < 0) {
		prevSelectorPosY = 3;
		y = 3;
	}
	selectorPosX = x;
	selectorPosY = y;
	setSelector(selectorPosX * 18, selectorPosY);
}


// Clear Previous Selector Position
void OLED_SSD1306::clearPrevSelector(void)
{
	unsigned char i = 0;

	setPosition(prevSelectorPosX * 18, prevSelectorPosY);

	for(i = 0; i < 6; i++)
		writeData(pgm_read_byte(&(SelectorBlank[i])));
}


// 
inline unsigned char OLED_SSD1306::getSelectedChar(void)
{
	return pgm_read_byte(&(SheikahChars[selectorPosY * 7  + selectorPosX]));
}


// 
void OLED_SSD1306::moveSelector(void)
{
	// Up
	if (digitalRead(BTN_UP_PIN) == HIGH) {
		// Store prevSelectorPosY
		prevSelectorPosY = selectorPosY;
		// Decrement selectorPosY
		if (selectorPosY <= 0) {
			selectorPosY = 3;
			prevSelectorPosY = 0;
		}
		else
			selectorPosY--;

		clearPrevSelector();
		setSelectorPos(selectorPosX, selectorPosY);
		delay(120);
	}

	// Down
	if (digitalRead(BTN_DOWN_PIN) == HIGH) {
		// Store prevSelectorPosY
		prevSelectorPosY = selectorPosY;
		// Increment selectorPosY
		if (selectorPosY > 3) {
			selectorPosY = 0;
			prevSelectorPosY = 3;
		}
		else
			selectorPosY++;

		clearPrevSelector();
		setSelectorPos(selectorPosX, selectorPosY);
		delay(120);
	}

	// Left
	if (digitalRead(BTN_LEFT_PIN) == HIGH) {
		// Store prevSelectorPosX
		prevSelectorPosX = selectorPosX;
		// Decrement selectorPosX
		if (selectorPosX <= 0) {
			selectorPosX = 6;
			prevSelectorPosX = 0;
		}
		else
			selectorPosX--;

		clearPrevSelector();
		setSelectorPos(selectorPosX, selectorPosY);
		delay(120);
	}

	// Right
	if (digitalRead(BTN_RIGHT_PIN) == HIGH) {
		// Store prevSelectorPosX
		prevSelectorPosX = selectorPosX;
		// Increment selectorPosX
		if (selectorPosX > 6) {
			selectorPosX = 0;
			prevSelectorPosX = 6;
		}
		else
			selectorPosX++;

		clearPrevSelector();
		setSelectorPos(selectorPosX, selectorPosY);
		delay(120);
	}

	// A
	if (digitalRead(BTN_A_PIN) == HIGH) {
		// Store prevCursorPosX
		prevCursorPosX = cursorPosX - 1;
		// Increment selectorPosX
		if (cursorPosX >= 20) {
			cursorPosX = 20;
			prevCursorPosX = 19;
		}
		else {
			cursorPosX++;
		}
		//clearPrevCursor();
		setCursorPos(cursorPosX, cursorPosY);
		print6x8Single(prevCursorPosX * 6, 7, getSelectedChar());
		print8x8Single(prevCursorPosX * 9, 5, getSelectedChar());
		//cursorPosX++;
		//setCursor(cursorPosX * 6, 7);
		delay(120);
	}

	// B
	if (digitalRead(BTN_B_PIN) == HIGH) {
		//clearLine();
		backspace();
		//backspace_8x8();
		
		// Store prevCursorPosX
		prevCursorPosX = cursorPosX + 1;
		
		delay(120);
	}

	/*print6x8Str(0, 4, "CursorX: ");
	printValueI(67, 4, cursorPosX);
	print6x8Str(0, 5, "pCursorX: ");
	printValueI(67, 5, prevCursorPosX);*/

	/*print6x8Str(0, 4, "SelectorX: ");
	printValueI(67, 4, selectorPosX);
	print6x8Str(0, 5, "SelectorY: ");
	printValueI(67, 5, selectorPosY);*/

	/*print6x8Str(0, 4, "Selected Char: [");
	print6x8Single(96, 4, getSelectedChar());
	print6x8Str(102, 4, "]");*/
	
	print6x8Str(0, 7, ">");

	/*print6x8Str(0, 6, "PreviousX: ");
	printValueI(67, 6, prevSelectorPosX);
	print6x8Str(0, 7, "PreviousY: ");
	printValueI(67, 7, prevSelectorPosY);*/
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 						Terminal Input
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Set Terminal Input Cursor Position
void OLED_SSD1306::setCursor(unsigned char x, unsigned char y)
{
	unsigned char c = 0, i = 0, j = 0;

	//clearPrevCursor();

	if (x > OLED_WIDTH - 6) {
		x = 7;
		y++;
	}
	setPosition(x, y);
	for(i = 0; i < 6; i++)
		writeData(pgm_read_byte(&(Cursor[i])));
}


// 
void OLED_SSD1306::setCursorPos(unsigned char x, unsigned char y)
{
	prevCursorPosX = cursorPosX - 1;
	prevCursorPosY = cursorPosY - 1;

	if (x == 20) {
		prevCursorPosX = 19;
		x = 20;
	}

	cursorPosX = x;
	cursorPosY = y;
	setCursor(cursorPosX * 6, cursorPosY);
}


// 
void OLED_SSD1306::clearPrevCursor(void)
{
	unsigned char i = 0;

	setPosition(prevCursorPosX * 6, prevCursorPosY);

	for(i = 0; i < 6; i++)
		writeData(pgm_read_byte(&(CursorBlank[i])));
}


// Print Line (New Line)
void OLED_SSD1306::printLine(void)
{
	//
}


// Clear Line
void OLED_SSD1306::clearLine(void)
{
	unsigned char i = 0, j = 0;

	setPosition(6, 7);

	for(j = 0; j < 120; j+=6)
	{
		for(i = 0; i < 6; i++)
			writeData(pgm_read_byte(&(CursorBlank[i])));
	}

	cursorPosX = 1;
	setCursor(cursorPosX * 6, cursorPosY);
}


// Backspace
void OLED_SSD1306::backspace(void)
{
	unsigned char i = 0;

	setPosition(cursorPosX * 6, 7);

	for(i = 0; i < 6; i++)
		writeData(pgm_read_byte(&(CursorBlank[i])));

	if (cursorPosX < 2)
		cursorPosX = 1;
	else
	{
		prevCursorPosX++;
		//clearPrevCursor();
		cursorPosX--;
		prevCursorPosX--;
	}
	setCursor(cursorPosX * 6, cursorPosY);
}


// Backspace Sheikah_8x8
void OLED_SSD1306::backspace_8x8(void)
{
	unsigned char i = 0;

	setPosition(cursorPosX * 9, 5);

	for(i = 0; i < 8; i++)
		writeData(pgm_read_byte(&(CursorBlank_8x8[i])));

	if (cursorPosX < 2)
	{
		cursorPosX = 1;
	}
	/*else
	{
		prevCursorPosX++;
		//clearPrevCursor();
		cursorPosX--;
		prevCursorPosX--;
	}*/
	//setCursor(cursorPosX * 9, 5);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 						Display Configuration
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// 
void OLED_SSD1306::setStartColumn(unsigned char d)
{
	writeCmd(0x00 + d % 16); // Set Lower Column Start Address for Page Addressing Mode
								   // Default => 0x00
	writeCmd(0x10 + d / 16); // Set Higher Column Start Address for Page Addressing Mode
								   // Default => 0x10
}


// 
void OLED_SSD1306::setAddressingMode(unsigned char d)
{
	writeCmd(0x20); // Set EEPROM Addressing Mode
	writeCmd(d);	  // Default => 0x02
						  // 0x00 => Horizontal Addressing Mode
						  // 0x01 => Vertical Addressing Mode
						  // 0x02 => Page Addressing Mode
}


// 
void OLED_SSD1306::setColumnAddress(unsigned char a, unsigned char b)
{
	writeCmd(0x21); // Set Column Address
	writeCmd(a);	  // Default => 0x00 (Column Start Address)
	writeCmd(b);	  // Default => 0x7F (Column End Address)
}


// 
void OLED_SSD1306::setPageAddress(unsigned char a, unsigned char b)
{
	writeCmd(0x22); // Set Page Address
	writeCmd(a);	  // Default => 0x00 (Page Start Address)
	writeCmd(b);	  // Default => 0x07 (Page End Address)
}


// 
void OLED_SSD1306::setStartLine(unsigned char d)
{
	writeCmd(0x40 | d); // Set Display Start Line
						 	  // Default => 0x40 (0x00)
}


// 
void OLED_SSD1306::setContrastControl(unsigned char d)
{
	writeCmd(0x81); // Set Contrast Control
	writeCmd(d);	  // Default => 0x7F
}


// 
void OLED_SSD1306::setChargePump(unsigned char d)
{
	writeCmd(0x8D);	  // Set Charge Pump
	writeCmd(0x10 | d); // Default => 0x10
							  // 0x10 (0x00) => Disable Charge Pump
							  // 0x14 (0x04) => Enable Charge Pump
}


// 
void OLED_SSD1306::setSegmentRemap(unsigned char d)
{
	writeCmd(0xA0 | d); // Set Segment Re-Map
							  // Default => 0xA0
							  // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
							  // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}


// 
void OLED_SSD1306::setEntireDisplay(unsigned char d)
{
	writeCmd(0xA4 | d); // Set Entire Display On / Off
							  // Default => 0xA4
							  // 0xA4 (0x00) => Normal Display
							  // 0xA5 (0x01) => Entire Display On
}


// 
void OLED_SSD1306::setInverseDisplay(unsigned char d)
{
	writeCmd(0xA6 | d); // Set Inverse Display On/Off
							  // Default => 0xA6
							  // 0xA6 (0x00) => Normal Display
							  // 0xA7 (0x01) => Inverse Display On
}


// 
void OLED_SSD1306::setMultiplexRatio(unsigned char d)
{
	writeCmd(0xA8); // Set Multiplex Ratio
	writeCmd(d);	  // Default => 0x3F (1/64 Duty)
}


// 
void OLED_SSD1306::setDisplayOnOff(unsigned char d)
{
	writeCmd(0xAE | d); // Set Display On/Off
							  // Default => 0xAE
							  // 0xAE (0x00) => Display Off
							  // 0xAF (0x01) => Display On
}


// 
void OLED_SSD1306::setStartPage(unsigned char d)
{
	writeCmd(0xB0 | d); // Set Page Start Address for Page Addressing Mode
							  // Default => 0xB0 (0x00)
}


// 
void OLED_SSD1306::setCommonRemap(unsigned char d)
{
	writeCmd(0xC0 | d); // Set COM Output Scan Direction
							  // Default => 0xC0
							  // 0xC0 (0x00) => Scan from COM0 to 63
							  // 0xC8 (0x08) => Scan from COM63 to 0
}


// 
void OLED_SSD1306::setDisplayOffset(unsigned char d)
{
	writeCmd(0xD3); // Set Display Offset
	writeCmd(d);	  // Default => 0x00
}


// 
void OLED_SSD1306::setDisplayClock(unsigned char d)
{
	writeCmd(0xD5); // Set Display Clock Divide Ratio / Oscillator Frequency
	writeCmd(d);	  // Default => 0x80
						  // D[3:0] => Display Clock Divider
						  // D[7:4] => Oscillator Frequency
}


// 
void OLED_SSD1306::setPrechargePeriod(unsigned char d)
{
	writeCmd(0xD9); // Set Pre-Charge Period
	writeCmd(d);	  // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						  // D[3:0] => Phase 1 Period in 1~15 Display Clocks
						  // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


// 
void OLED_SSD1306::setCommonConfig(unsigned char d)
{
	writeCmd(0xDA);	  // Set COM Pins Hardware Configuration
	writeCmd(0x02 | d); // Default => 0x12 (0x10)
							  // Alternative COM Pin Configuration
							  // Disable COM Left/Right Re-Map
}


// 
void OLED_SSD1306::setVCOMH(unsigned char d)
{
	writeCmd(0xDB); // Set VCOMH Deselect Level
	writeCmd(d);	  // Default => 0x20 (0.77*VCC)
						  // 0x00 => 0.62*VCC
						  // 0x20 => 0.77*VCC
						  // 0x30 => 0.83*VCC
}


// 
void OLED_SSD1306::setNOP(void)
{
	writeCmd(0xE3); // Command for No Operation
}


// 
void OLED_SSD1306::initDisplay(void)
{
	// setup SPI
	SPI.begin()
#ifdef SPI_HAS_TRANSACTION
	SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
#else
	SPI.setClockDivider (4);
#endif

	initPins();

	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(RST_PIN, LOW);
	delay(50);
	digitalWrite(RST_PIN, HIGH);

	setDisplayOnOff(0x00);   		// Display Off (0x00/0x01)
	setDisplayClock(0x80);		// Set Clock as 100 Frames/Sec
	setMultiplexRatio(0x3F);  	// 1/64 Duty (0x0F~0x3F)
	setDisplayOffset(0x00);   	// Shift Mapping RAM Counter (0x00~0x3F)
	setStartLine(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	setChargePump(0x04);			// Enable Embedded DC/DC Converter (0x00/0x04)
	setAddressingMode(0x00);		// Set Page Addressing Mode (0x00/0x01/0x02)
	setSegmentRemap(0x01);		// Set SEG/Column Mapping
	setColumnAddress(0x00, 0x7F); // 
	setPageAddress(0x00, 0x07);	// 
	setCommonRemap(0x08);			// Set COM/Row Scan Direction
	setCommonConfig(0x10);		// Set Sequential Configuration (0x00/0x10)
	setContrastControl(0xCF); 	// Set SEG Output Current
	setPrechargePeriod(0xF1); 	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	setVCOMH(0x20);				// Set VCOM Deselect Level
	setEntireDisplay(0x00);   	// Disable Entire Display On (0x00/0x01)
	setInverseDisplay(0x00);  	// Disable Inverse Display On (0x00/0x01)
	setDisplayOnOff(0x01);   		// Display On (0x00/0x01)

	fill(0x00);					// Clear all
	setPosition(0, 0);			// Top left
}
