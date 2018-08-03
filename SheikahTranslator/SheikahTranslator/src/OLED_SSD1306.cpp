/* *
 * Class for interfacing with the SSD1306 OLED display
 */

#include "OLED_SSD1306.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 								Display
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Initialize pins
void OLED_SSD1306::initPins(void)
{
	pinMode(SCL_PIN, OUTPUT);
	pinMode(SDA_PIN, OUTPUT);
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
	unsigned char i = 8;
	//OLED_CS = 0;
	//OLED_DCL;
	digitalWrite(DC_PIN, LOW);
	//OLED_SCLL;
	digitalWrite(SCL_PIN, LOW);
	while (i--)
	{
		if (cmd & 0x80)
		{
			//OLED_SDAH;
			digitalWrite(SDA_PIN, HIGH);
		}
		else
		{
			//OLED_SDAL;
			digitalWrite(SDA_PIN, LOW);
		}
		//OLED_SCLH;
		digitalWrite(SCL_PIN, HIGH);
		asm("nop");
		//OLED_SCLL;
		digitalWrite(SCL_PIN, LOW);
		cmd <<= 1;
	}
	//OLED_CS = 1;
}


// Write Data
void OLED_SSD1306::writeData(unsigned char data)
{
	unsigned char i = 8;
	//OLED_CS = 0;
	//OLED_DCH;
	digitalWrite(DC_PIN, HIGH);
	//OLED_SCLL;
	digitalWrite(SCL_PIN, LOW);
	while(i--)
	{
		if (data & 0x80)
			digitalWrite(SDA_PIN, HIGH);
		else
			digitalWrite(SDA_PIN, LOW);

		//OLED_SCLH;
		digitalWrite(SCL_PIN, HIGH);
		asm("nop");

		//OLED_SCLL;
		digitalWrite(SCL_PIN, LOW);
		data <<= 1;
	}
	//OLED_CS = 1;
}


// Clear the screen
void OLED_SSD1306::cls(void)
{
	unsigned char x, y;

	for(y = 0; y < 16; y++)
	{
		/*this->writeCmd(0xB0 + y);
		this->writeCmd(0x00);
		this->writeCmd(0x10);*/

		for (x = 0; x < OLED_WIDTH; x++)
			this->writeData(0x00);
	}
}


// Fill the screen
void OLED_SSD1306::fill(unsigned char data)
{
	unsigned char y, x;

	for (y = 0; y < 8; y++)
	{
		this->writeCmd(0xB0 + y);
		this->writeCmd(0x00);
		this->writeCmd(0x10);

		for (x = 0; x < OLED_WIDTH; x++)
			this->writeData(data);
	}
}


// Clear the buffer
void OLED_SSD1306::clearBuffer(void)
{
	/*int x, y;

	for(x = 0; x < 1024; x++)
	{
		this->writeCmd(0xB0 + y);
		this->writeCmd(0x00);
		this->writeCmd(0x10);

		for(y = 0; y < 8; y++) //OLED_WIDTH
			this->buffer[y] = 0;
	}*/
		
	memset(this->buffer, 0, sizeof(this->buffer));
	this->setPosition(0, 0);
	this->flushBuffer();
}


// Flush the buffer to the display
void OLED_SSD1306::flushBuffer(void)
{
	int i, j;
	this->setPosition(0, 0);

	for(i = 0; i < 1024; i++) //OLED_WIDTH
	{
		/*this->writeCmd(0xB0 + y);
		this->writeCmd(0x00);
		this->writeCmd(0x10);*/

		for(j = 0; j < 8; j++) //OLED_HEIGHT
		{
			this->writeData(this->buffer[i]); // Transmit data to the display
			i++;
		}
		i--;
	}
}


// Set Position
void OLED_SSD1306::setPosition(unsigned char x, unsigned char y)
{
	this->writeCmd(0xB0 + y);
	this->writeCmd(((x & 0xf0) >> 4) | 0x10);
	this->writeCmd((x & 0x0f) | 0x00);
}


// Load char array data from PROGMEM
void OLED_SSD1306::loadSplash(void)
{
	this->clearBuffer();

	int i = 0;

	for(i = 0; i < 1024; i++)
		this->buffer[i] = pgm_read_byte(&(SheikahEyeSplash[i]));

	this->flushBuffer();

	while(1)
	{
		if(digitalRead(BTN_A_PIN) == HIGH || digitalRead(BTN_B_PIN) == HIGH) {
			return;
		}
	}
}


// Display the title screen
void OLED_SSD1306::loadTitle(void)
{
	this->clearBuffer();
	this->drawRect(0, 5, 127, 18, 0);
	this->drawPixel(53, 51);
	this->drawPixel(60, 51);
	this->drawPixel(67, 51);
	this->drawPixel(74, 51);
	this->flushBuffer();

	this->print6x8Str(12, 1, "Sheikah Translator");
	this->print6x8Str(0, 4, "\"It's dangerous to go");
	this->print6x8Str(6, 5, "alone! Take this.\"");
	this->print6x8Str(0, 7, "Happy Birthday, Matt!");

	// Wait for A or B button to be pressed
	while(1)
	{
		if(digitalRead(BTN_A_PIN) == HIGH || digitalRead(BTN_B_PIN) == HIGH) {
			return;
		}
	}
}


// 
void OLED_SSD1306::mainMenu(void)
{
	this->currentMenu = 4;
	unsigned char selectorX = 0;
	this->prevSelectorPosX = 0;
	this->prevSelectorPosY = 4;

	this->clearBuffer();
	this->drawRect(0, 5, 127, 18, 0);
	this->flushBuffer();

	//this->setPosition(0, 0);
	this->print6x8Str(37, 1, "Main Menu");
	this->setSelector(selectorX, this->currentMenu);
	this->print6x8Str(6, 4, "Translator");
	this->print6x8Str(6, 5, "Settings");
	this->print6x8Str(6, 6, "Graphics Test");

	// Wait for a selection to be made
	while(1)
	{
		// Up
		if(digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevMenu = this->currentMenu;
			// Decrement selectorPosY
			if(this->currentMenu <= 4) {
				this->currentMenu = 6;
				this->prevMenu = 4;
			}
			else
				this->currentMenu--;

			//this->clearPrevSelector();
			unsigned char i = 0;

			this->setPosition(selectorX, this->prevMenu);

			for (i = 0; i < 6; i++)
				this->writeData(pgm_read_byte(&(SelectorBlank[i])));

			this->setSelector(selectorX, this->currentMenu);
			delay(200);
		}

		// Down
		if(digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevMenu = this->currentMenu;
			// Increment selectorPosY
			if(this->currentMenu >= 6) {
				this->currentMenu = 4;
				this->prevMenu = 6;
			}
			else
				this->currentMenu++;

			//this->clearPrevSelector();
			unsigned char i = 0;

			this->setPosition(selectorX, this->prevMenu);

			for (i = 0; i < 6; i++)
				this->writeData(pgm_read_byte(&(SelectorBlank[i])));

			this->setSelector(selectorX, this->currentMenu);
			delay(200);
		}

		// A
		if(digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			if(this->currentMenu == 4) {
				return this->printSheikahMap();
			}
			if(this->currentMenu == 5) {
				return this->settingsMenu();
			}
			if(this->currentMenu == 6) {
				return this->graphicsTest();
			}
			delay(200);
		}
	}
}


// Settings Menu
void OLED_SSD1306::settingsMenu(void)
{
	this->currentMenu = 4;
	unsigned char selectorX = 0;
	this->prevSelectorPosX = 0;
	this->prevSelectorPosY = 4;

	this->clearBuffer();
	this->drawRect(0, 5, 127, 18, 0);
	this->flushBuffer();

	this->print6x8Str(40, 1, "Settings");
	this->setSelector(selectorX, this->currentMenu);
	this->print6x8Str(6, 4, "Contrast");
	this->print6x8Str(6, 5, "Setting 2");
	this->print6x8Str(6, 6, "Setting 3");

	// Wait for a selection to be made
	while(1)
	{
		// Up
		if(digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevMenu = this->currentMenu;
			// Decrement selectorPosY
			if(this->currentMenu <= 4) {
				this->currentMenu = 6;
				this->prevMenu = 4;
			}
			else
			this->currentMenu--;

			//this->clearPrevSelector();
			unsigned char i = 0;

			this->setPosition(selectorX, this->prevMenu);

			for (i = 0; i < 6; i++)
			this->writeData(pgm_read_byte(&(SelectorBlank[i])));

			this->setSelector(selectorX, this->currentMenu);
			delay(200);
		}

		// Down
		if(digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevMenu = this->currentMenu;
			// Increment selectorPosY
			if(this->currentMenu >= 6) {
				this->currentMenu = 4;
				this->prevMenu = 6;
			}
			else
			this->currentMenu++;

			//this->clearPrevSelector();
			unsigned char i = 0;

			this->setPosition(selectorX, this->prevMenu);

			for (i = 0; i < 6; i++)
			this->writeData(pgm_read_byte(&(SelectorBlank[i])));

			this->setSelector(selectorX, this->currentMenu);
			delay(200);
		}

		// A
		if(digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			if(this->currentMenu == 4) {
				return this->setContrast();
			}
			if(this->currentMenu == 5) {
				return this->settingsMenu();
			}
			if(this->currentMenu == 6) {
				return this->graphicsTest();
			}
			delay(200);
		}

		// B
		if(digitalRead(BTN_B_PIN) == HIGH) {
			return this->mainMenu();
		}
	}
}


// Set Contrast
void OLED_SSD1306::setContrast(void)
{
	/*! This is causing the display to freeze for some reason..
	if(this->EEPROM.readByte(0x50, 0) == 0) {
		this->contrast = 0x7F;
		this->EEPROM.writeByte(0x50, 0, this->contrast);
	}
	else
		this->contrast = this->EEPROM.readByte(0x50, 0);*/

	unsigned char selectorX = 0;
	this->prevSelectorPosX = 0;
	this->prevSelectorPosY = 4;

	this->clearBuffer();
	this->drawRect(0, 5, 127, 18, 0);

	// Draw the progress bar
	this->drawRect(0, 52, 127, 58, 0);
	this->drawRect(2, 54, 125, 56, 0);

	//this->drawCircle(50, 52, 6);
	this->flushBuffer();

	this->print6x8Str(40, 1, "Contrast");
	this->print6x8Str(43, 4, "<");
	//this->printValueI(18, 4, this->contrast);
	this->print6x8Str(79, 4, ">");
	/*this->print6x8Str(6, 6, "Press");
	this->print6x8Str(48, 6, "B");
	this->print6x8Str(66, 6, "to go back");*/

	// Wait for a selection to be made
	while(1)
	{
		// Left
		if(digitalRead(BTN_LEFT_PIN) == HIGH) {
			// Decrement contrast
			if(this->contrast <= 0x00) {
				this->contrast = 0x00;
			}
			else {
				this->contrast--;

				if(this->contrast % 2 == 0) {
					this->contrastBar--;
				}
			}
			this->clearBuffer();
			this->drawRect(2, 54, this->contrastBar, 56, 0);
			this->flushBuffer();

			this->setContrastControl(this->contrast);
			delay(50);
		}

		// Right
		if(digitalRead(BTN_RIGHT_PIN) == HIGH) {
			// Increment contrast
			if(this->contrast >= 0xFF) {
				this->contrast = 0xFF;
			}
			else {
				this->contrast++;

				if(this->contrast % 2 == 0) {
					this->contrastBar++;
				}
			}
			this->clearBuffer();
			this->drawRect(2, 54, this->contrastBar, 56, 0);
			this->flushBuffer();

			this->setContrastControl(this->contrast);
			delay(50);
		}

		// B
		if(digitalRead(BTN_B_PIN) == HIGH) {
			this->EEPROM.writeByte(0x50, 0, this->contrast);
			return this->settingsMenu();
		}

		this->printShortValueI(49, 4, this->contrast);
	}
}


// Set Delay (not currently working..)
//=> TODO: Create a timer that is interruptible
void OLED_SSD1306::setDelay(unsigned int ms)
{
	unsigned int a;

	while(ms)
	{
		a = 6675;
		while(a--)
			ms--;
	}
	return;
	// time_delay_ms(ms);
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
	if (x > 122)
	{
		x = 0;
		y++;
	}
	this->setPosition(x, y);
	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(Shard_6x8[c][i])));
}


// Print Shard_6x8 String
void OLED_SSD1306::print6x8Str(unsigned char x, unsigned char y, const char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > OLED_WIDTH)
		{
			x = 0;
			y++;
		}
		this->setPosition(x, y);
		for (i = 0; i < 6; i++)
			this->writeData(pgm_read_byte(&(Shard_6x8[c][i])));

		x += 6;
		j++;
	}
}


// Print Single Shard_6x8 Character
void OLED_SSD1306::print6x8Single(unsigned char x, unsigned char y, char ch)
{
	unsigned char c = 0, i = 0, j = 0;
	if(ch != '\0')
	{
		c = ch - 32;
		if (x > OLED_WIDTH)
		{
			x = 0;
			y++;
		}
		this->setPosition(x, y);
		for (i = 0; i < 6; i++)
			this->writeData(pgm_read_byte(&(Shard_6x8[c][i])));

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
		if (x > OLED_WIDTH - 8)
		{
			x = 0;
			y++;
		}
		this->setPosition(x, y);
		for (i = 0; i < 8; i++)
			this->writeData(pgm_read_byte(&(Sheikah_8x8[c][i])));

		x += 9;
		j++;
	}
}


// Print Single Sheikah_8x8 Character
void OLED_SSD1306::print8x8Single(unsigned char x, unsigned char y, char ch)
{
	unsigned char c = 0, i = 0, j = 0;
	if(ch != '\0')
	{
		c = ch - 32;
		if (x > OLED_WIDTH - 8)
		{
			x = 0;
			y++;
		}
		this->setPosition(x, y);
		for (i = 0; i < 8; i++)
			this->writeData(pgm_read_byte(&(Sheikah_8x8[c][i])));

		x += 9;
	}
}


// 
void OLED_SSD1306::print8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 120)
		{
			x = 0;
			y++;
		}

		this->setPosition(x, y);
		for (i = 0; i < 8; i++)
		{
			this->writeData(pgm_read_byte(&(F8X16[(c << 4) + i])));
		}

		this->setPosition(x, y + 1);
		for (i = 0; i < 8; i++)
		{
			this->writeData(pgm_read_byte(&(F8X16[(c << 4) + i + 8])));
		}
		x += 8;
		j++;
	}
}


// 
void OLED_SSD1306::printValueC(unsigned char x, unsigned char y, char data)
{
	unsigned char i, j, k;
	if (data < 0)
	{
		this->print6x8Char(x, y, '-');
		data = -data;
	}
	else
		this->print6x8Char(x, y, '+');

	i = data / 100;
	j = (data % 100) / 10;
	k = data % 10;
	this->print6x8Char(x + 6, y, i + 48);
	this->print6x8Char(x + 12, y, j + 48);
	this->print6x8Char(x + 18, y, k + 48);
}


// 
void OLED_SSD1306::printValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i, j, k, l, m;
	if (data < 0)
	{
		this->print6x8Char(x, y, '-');
		data = -data;
	}
	else
		this->print6x8Char(x, y, '+');

	l = data / 10000;
	m = (data % 10000) / 1000;
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	this->print6x8Char(x + 6, y, l + 48);
	this->print6x8Char(x + 12, y, m + 48);
	this->print6x8Char(x + 18, y, i + 48);
	this->print6x8Char(x + 24, y, j + 48);
	this->print6x8Char(x + 30, y, k + 48);
}


// 
void OLED_SSD1306::printShortValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i, j, k, l, m;
	if (data < 0)
	{
		this->print6x8Char(x, y, '-');
		data = -data;
	}
	/*else
		this->print6x8Char(x, y, '+');*/

	/*l = data / 10000;
	m = (data % 10000) / 1000;*/
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	/*this->print6x8Char(x + 6, y, l + 48);
	this->print6x8Char(x + 12, y, m + 48);*/
	this->print6x8Char(x + 6, y, i + 48);
	this->print6x8Char(x + 12, y, j + 48);
	this->print6x8Char(x + 18, y, k + 48);
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
		this->print6x8Char(x, y, '-');
	else
		this->print6x8Char(x, y, '+');
	if (tempdataii < 0)
		tempdataii = -tempdataii;
	tempdataui = tempdataii;
	l = tempdataui / 10000;
	m = (tempdataui % 10000) / 1000;
	i = (tempdataui % 1000) / 100;
	j = (tempdataui % 100) / 10;
	k = tempdataui % 10;
	if (l != 0)
	{
		this->print6x8Char(x + 6, y, l + 48);
		this->print6x8Char(x + 12, y, m + 48);
		this->print6x8Char(x + 18, y, i + 48);
		this->print6x8Char(x + 24, y, j + 48);
		this->print6x8Char(x + 30, y, k + 48);
	}
	else if (m != 0)
	{
		databiti = 5;
		this->print6x8Char(x + 6, y, m + 48);
		this->print6x8Char(x + 12, y, i + 48);
		this->print6x8Char(x + 18, y, j + 48);
		this->print6x8Char(x + 24, y, k + 48);
	}
	else if (i != 0)
	{
		databiti = 4;
		this->print6x8Char(x + 6, y, i + 48);
		this->print6x8Char(x + 12, y, j + 48);
		this->print6x8Char(x + 18, y, k + 48);
	}
	else if (j != 0)
	{
		databiti = 3;
		this->print6x8Char(x + 6, y, j + 48);
		this->print6x8Char(x + 12, y, k + 48);
	}
	else
	{
		databiti = 2;
		this->print6x8Char(x + 6, y, k + 48);
	}
	if (tempdatalp < 0)
		tempdatalp = -tempdatalp;
	switch (num)
	{
		case 0:
			break;
		case 1:
			this->printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 1000), num);
			break;
		case 2:
			this->printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 100), num);
			break;
		case 3:
			this->printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp / 10), num);
			break;
		case 4:
			this->printValueFP(x + databiti * 6, y, (unsigned int)(tempdatalp), num);
			break;
	}
}


// 
void OLED_SSD1306::printValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num)
{
	unsigned char m, i, j, k;
	this->print6x8Char(x, y, '.');
	m = data / 1000;
	i = (data % 1000) / 100;
	j = (data % 100) / 10;
	k = data % 10;
	switch (num)
	{
	case 1:
		this->print6x8Char(x + 6, y, k + 48);
		break;
	case 2:
		this->print6x8Char(x + 6, y, j + 48);
		this->print6x8Char(x + 12, y, k + 48);
		break;
	case 3:
		this->print6x8Char(x + 6, y, i + 48);
		this->print6x8Char(x + 12, y, j + 48);
		this->print6x8Char(x + 18, y, k + 48);
		break;
	case 4:
		this->print6x8Char(x + 6, y, m + 48);
		this->print6x8Char(x + 12, y, i + 48);
		this->print6x8Char(x + 18, y, j + 48);
		this->print6x8Char(x + 24, y, k + 48);
		break;
	}
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 							Graphics
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Draw Pixel
void OLED_SSD1306::drawPixel(int x, int y)
{
	if((x < 0) || (x >= OLED_WIDTH) || (y < 0) || (y >= OLED_HEIGHT)) // Check for boundaries
    	return;
	else
		this->buffer[x + (y / 8) * OLED_WIDTH] |= bit((y % 8)); // Store pixel in the buffer
}


// Draw Line
// Bresenham's line drawing algorithm
void OLED_SSD1306::drawLine(int x1, int y1, int x2, int y2, int spacing)
{
	int cx = x1;
	int cy = y1;
	int dx = x2 - cx;
	int dy = y2 - cy;

	if(dx < 0)
		dx = 0 - dx;
	if(dy < 0)
		dy = 0 - dy;

	int sx = 0;
	int sy = 0;

	if(cx < x2)
		sx = 1;
	else
		sx = -1;
	
	if(cy < y2)
		sy = 1;
	else
		sy = -1;
	

	int err = dx - dy;
	
	for(int n = 0; n < 1000; n++)
	{
		this->drawPixel(cx, cy);

		if((cx == x2) && (cy == y2))
			return;

		int e2 = 2 * err;

		if(e2 > (0 - dy))
		{
			err = err - dy;
			cx = cx + sx;
			cx += spacing;
		}
		if(e2 < dx)
		{
			err = err + dx;
			cy = cy + sy;
			cy += spacing;
		}
	}
}


// Draw Rectangle
void OLED_SSD1306::drawRect(int x1, int y1, int x2, int y2, int spacing)
{
	int x, y;
	
	// Left
	/*for(x = x1; x < x2; x += spacing) // Iterate over x range, draw line on y1
		this->drawPixel(x, y1);
	
	// Left
	for(x = x1; x < x2; x += spacing) // Iterate over x range, draw line on y2
		this->drawPixel(x, y2);
	
	// Left
	for(y = y1; y < y2; y += spacing) // Iterate over y range, draw line on x1
		this->drawPixel(x1, y);
	
	// Left
	for(y = y1; y < y2; y += spacing) // Iterate over y range, draw line on x2 
		this->drawPixel(x2, y);*/
	
	// Alternative method (works perfectly)
	this->drawLine(x1, y1, x2 - 1, y1, spacing); // top
	this->drawLine(x2, y1, x2, y2 - 1, spacing); // right
	this->drawLine(x2, y2 - 1, x1 + 1, y2 - 1, spacing); // bottom
	this->drawLine(x1, y2 - 1, x1, y1 + 1, spacing); // left
}


// Draw Dot Grid
void OLED_SSD1306::drawDotGrid(int x1, int y1, int x2, int y2, int spacing)
{
	//
}


// Draw Line Grid
void OLED_SSD1306::drawLineGrid(int x1, int y1, int x2, int y2, int spacing)
{
	//
}


// Draw Circle
void OLED_SSD1306::drawCircle(int x0, int y0, int radius)
{
	int x = radius; // Set x equal to radius
	int y = 0;
	int de = 1 - x;
 
	while(x >= y)
	{
		this->drawPixel(x + x0, y + y0);   // First octant
		this->drawPixel(y + x0, x + y0);   // Second octant
		this->drawPixel(-y + x0, x + y0);  // Third octant
		this->drawPixel(-x + x0, y + y0);  // Fourth octant
		this->drawPixel(-x + x0, -y + y0); // Fifth octant
		this->drawPixel(-y + x0, -x + y0); // Sixth octant
		this->drawPixel(y + x0, -x + y0);  // Seventh octant
		this->drawPixel(x + x0, -y + y0);  // Eighth octant
		y++;

		if(de <= 0)
			de += 2 * y + 1;
		else
		{
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

	for(y = y0; y < y1; y++)
	{
		this->setPosition(x0, y);				
		for(x = x0; x < x1; x++)
		{
			this->writeData(bitmap_array[i++]);
		}
	}
}


// Graphics Test
void OLED_SSD1306::graphicsTest(void)
{
	this->clearBuffer();
	this->print6x8Str(0, 0, "Graphics test..");
	delay(3000);
	this->clearBuffer();

	// Draw pixel
	this->print6x8Str(0, 0, "Draw pixel..");
	delay(2000);
	this->clearBuffer();
	this->drawPixel(63, 31);
	this->flushBuffer();
	delay(3000);
	this->clearBuffer();

	// Draw line
	this->print6x8Str(0, 0, "Draw line..");
	delay(2000);
	this->clearBuffer();
	this->drawLine(0, 31, 127, 31, 0);
	this->flushBuffer();
	delay(3000);
	this->clearBuffer();

	// Draw rectangle
	this->print6x8Str(0, 0, "Draw rectangle..");
	delay(2000);
	this->clearBuffer();
	this->drawRect(0, 0, 127, 63, 0);
	this->flushBuffer();
	delay(3000);
	this->clearBuffer();

	// Draw circle
	this->print6x8Str(0, 0, "Draw circle..");
	delay(2000);
	this->clearBuffer();
	this->drawCircle(63, 31, 20);
	this->flushBuffer();
	delay(3000);

	return this->mainMenu();
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
	this->selectorPosX = 0;
	this->selectorPosY = 0;
	this->cursorPosX = 1;
	this->cursorPosY = 7;
	/*unsigned char x = 6, y = 0, i = 0;

	for(i = 0; i < sizeof(SheikahChars); i++)
	{
		for(x = 6; x <= 114; x += 18)
			this->print8x8Str(x, y, SheikahChars[i]);

		//x = 6;
		y++;
	}*/

	this->clearBuffer();
	this->drawLine(0, 35, 127, 35, 0);
	this->drawLine(0, 54, 127, 54, 0);
	this->flushBuffer();

	this->setSelectorPos(this->selectorPosX, this->selectorPosY);
	this->setCursor(this->cursorPosX * 6, this->cursorPosY);
	this->print6x8Str(0, 7, ">");

	this->print8x8Str(6, 0, "A");
	this->print8x8Str(24, 0, "B");
	this->print8x8Str(42, 0, "C");
	this->print8x8Str(60, 0, "D");
	this->print8x8Str(78, 0, "E");
	this->print8x8Str(96, 0, "F");
	this->print8x8Str(114, 0, "G");

	this->print8x8Str(6, 1, "H");
	this->print8x8Str(24, 1, "I");
	this->print8x8Str(42, 1, "J");
	this->print8x8Str(60, 1, "K");
	this->print8x8Str(78, 1, "L");
	this->print8x8Str(96, 1, "M");
	this->print8x8Str(114, 1, "N");

	this->print8x8Str(6, 2, "O");
	this->print8x8Str(24, 2, "P");
	this->print8x8Str(42, 2, "Q");
	this->print8x8Str(60, 2, "R");
	this->print8x8Str(78, 2, "S");
	this->print8x8Str(96, 2, "T");
	this->print8x8Str(114, 2, "U");

	this->print8x8Str(6, 3, "V");
	this->print8x8Str(24, 3, "W");
	this->print8x8Str(42, 3, "X");
	this->print8x8Str(60, 3, "Y");
	this->print8x8Str(78, 3, "Z");
	this->print8x8Str(96, 3, " ");
	this->print8x8Str(114, 3, ".");

	while(1)
	{
		// Up
		if(digitalRead(BTN_UP_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevSelectorPosY = this->selectorPosY;
			// Decrement selectorPosY
			if(this->selectorPosY <= 0) {
				this->selectorPosY = 3;
				this->prevSelectorPosY = 0;
			}
			else
				this->selectorPosY--;

			this->clearPrevSelector();
			this->setSelectorPos(this->selectorPosX, this->selectorPosY);
			delay(200);
		}

		// Down
		if(digitalRead(BTN_DOWN_PIN) == HIGH) {
			// Store prevSelectorPosY
			this->prevSelectorPosY = this->selectorPosY;
			// Increment selectorPosY
			if(this->selectorPosY > 3) {
				this->selectorPosY = 0;
				this->prevSelectorPosY = 3;
			}
			else
				this->selectorPosY++;

			this->clearPrevSelector();
			this->setSelectorPos(this->selectorPosX, this->selectorPosY);
			delay(200);
		}

		// Left
		if(digitalRead(BTN_LEFT_PIN) == HIGH) {
			// Store prevSelectorPosX
			this->prevSelectorPosX = this->selectorPosX;
			// Decrement selectorPosX
			if(this->selectorPosX <= 0) {
				this->selectorPosX = 6;
				this->prevSelectorPosX = 0;
			}
			else
				this->selectorPosX--;

			this->clearPrevSelector();
			this->setSelectorPos(this->selectorPosX, this->selectorPosY);
			delay(200);
		}

		// Right
		if(digitalRead(BTN_RIGHT_PIN) == HIGH) {
			// Store prevSelectorPosX
			this->prevSelectorPosX = this->selectorPosX;
			// Increment selectorPosX
			if(this->selectorPosX > 6) {
				this->selectorPosX = 0;
				this->prevSelectorPosX = 6;
			}
			else
				this->selectorPosX++;

			this->clearPrevSelector();
			this->setSelectorPos(this->selectorPosX, this->selectorPosY);
			delay(200);
		}

		// A
		if(digitalRead(BTN_A_PIN) == HIGH) {
			// Store prevCursorPosX
			this->prevCursorPosX = this->cursorPosX - 1;
			// Increment selectorPosX
			if(this->cursorPosX >= 14) { //20
				this->cursorPosX = 14; //20
				this->prevCursorPosX = 13; //19
			}
			else {
				this->cursorPosX++;
			}
			//this->clearPrevCursor();
			this->setCursorPos(this->cursorPosX, this->cursorPosY);
			this->print6x8Single(this->prevCursorPosX * 6, 7, this->getSelectedChar());
			this->print8x8Single(this->prevCursorPosX * 9, 5, this->getSelectedChar());
			//this->cursorPosX++;
			//this->setCursor(this->cursorPosX * 6, 7);
			delay(200);
		}

		// B
		if(digitalRead(BTN_B_PIN) == HIGH) {
			//this->clearLine();
			this->backspace();
			this->backspace_8x8();
		
			// Store prevCursorPosX
			this->prevCursorPosX = this->cursorPosX + 1;
		
			delay(200);
		}

		/*this->print6x8Str(0, 4, "CursorX: ");
		this->printValueI(67, 4, this->cursorPosX);
		this->print6x8Str(0, 5, "pCursorX: ");
		this->printValueI(67, 5, this->prevCursorPosX);*/

		/*this->print6x8Str(0, 4, "SelectorX: ");
		this->printValueI(67, 4, this->selectorPosX);
		this->print6x8Str(0, 5, "SelectorY: ");
		this->printValueI(67, 5, this->selectorPosY);*/

		/*this->print6x8Str(0, 4, "Selected Char: [");
		this->print6x8Single(96, 4, this->getSelectedChar());
		this->print6x8Str(102, 4, "]");*/
	
		//this->print6x8Str(0, 7, ">");

		/*this->print6x8Str(0, 6, "PreviousX: ");
		this->printValueI(67, 6, this->prevSelectorPosX);
		this->print6x8Str(0, 7, "PreviousY: ");
		this->printValueI(67, 7, this->prevSelectorPosY);*/

		// B && Up
		if(digitalRead(BTN_B_PIN) == HIGH && digitalRead(BTN_UP_PIN) == HIGH) {
			// Switch to main menu
			this->clearBuffer();
			return this->mainMenu();
		
			delay(120);
		}
	}
}


// Print Sheikah Number Map
void OLED_SSD1306::printSheikahNums(void)
{
	this->clearBuffer();
	this->drawLine(0, 35, 127, 35, 0);
	this->drawLine(0, 54, 127, 54, 0);
	this->flushBuffer();
	this->setSelectorPos(this->selectorPosX, this->selectorPosY);
	this->setCursor(this->cursorPosX * 6, this->cursorPosY);

	this->print8x8Str(6, 1, "");
	this->print8x8Str(24, 1, "0");
	this->print8x8Str(42, 1, "1");
	this->print8x8Str(60, 1, "2");
	this->print8x8Str(78, 1, "3");
	this->print8x8Str(96, 1, "4");
	this->print8x8Str(114, 1, "");

	this->print8x8Str(6, 2, "");
	this->print8x8Str(24, 2, "5");
	this->print8x8Str(42, 2, "6");
	this->print8x8Str(60, 2, "7");
	this->print8x8Str(78, 2, "8");
	this->print8x8Str(96, 2, "9");
	this->print8x8Str(114, 2, "");
}


// Set Selector
void OLED_SSD1306::setSelector(unsigned char x, unsigned char y)
{
	unsigned char c = 0, i = 0, j = 0;

	this->clearPrevSelector();

	if (x > 114)
	{
		x = 0;
		y++;
	}
	this->setPosition(x, y);
	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(Selector[i])));
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

	this->prevSelectorPosX = this->selectorPosX;
	this->prevSelectorPosY = this->selectorPosY;
	if(x > 6) {
		this->prevSelectorPosX = 0;
		x = 0;
	}
	else if(x < 0) {
		this->prevSelectorPosX = 6;
		x = 6;
	}
	
	if(y > 3) {
		this->prevSelectorPosY = 0;
		y = 0;
	}
	else if(y < 0) {
		this->prevSelectorPosY = 3;
		y = 3;
	}
	this->selectorPosX = x;
	this->selectorPosY = y;
	this->setSelector(this->selectorPosX * 18, this->selectorPosY);
}


// Clear Previous Selector Position
void OLED_SSD1306::clearPrevSelector(void)
{
	unsigned char i = 0;

	this->setPosition(this->prevSelectorPosX * 18, this->prevSelectorPosY);

	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(SelectorBlank[i])));
}


// 
unsigned char OLED_SSD1306::getSelectedChar(void)
{
	if(this->selectorPosX == 0 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[0])); // = "A";
	else if(this->selectorPosX == 1 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[1])); // = "B";
	else if(this->selectorPosX == 2 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[2])); // = "C";
	else if(this->selectorPosX == 3 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[3])); // = "D";
	else if(this->selectorPosX == 4 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[4])); // = "E";
	else if(this->selectorPosX == 5 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[5])); // = "F";
	else if(this->selectorPosX == 6 && this->selectorPosY == 0) this->selectedChar = pgm_read_byte(&(SheikahChars[6])); // = "G";
	else if(this->selectorPosX == 0 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[7])); // = "H";
	else if(this->selectorPosX == 1 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[8])); // = "I";
	else if(this->selectorPosX == 2 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[9])); // = "J";
	else if(this->selectorPosX == 3 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[10])); // = "K";
	else if(this->selectorPosX == 4 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[11])); // = "L";
	else if(this->selectorPosX == 5 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[12])); // = "M";
	else if(this->selectorPosX == 6 && this->selectorPosY == 1) this->selectedChar = pgm_read_byte(&(SheikahChars[13])); // = "N";
	else if(this->selectorPosX == 0 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[14])); // = "O";
	else if(this->selectorPosX == 1 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[15])); // = "P";
	else if(this->selectorPosX == 2 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[16])); // = "Q";
	else if(this->selectorPosX == 3 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[17])); // = "R";
	else if(this->selectorPosX == 4 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[18])); // = "S";
	else if(this->selectorPosX == 5 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[19])); // = "T";
	else if(this->selectorPosX == 6 && this->selectorPosY == 2) this->selectedChar = pgm_read_byte(&(SheikahChars[20])); // = "U";
	else if(this->selectorPosX == 0 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[21])); // = "V";
	else if(this->selectorPosX == 1 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[22])); // = "W";
	else if(this->selectorPosX == 2 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[23])); // = "X";
	else if(this->selectorPosX == 3 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[24])); // = "Y";
	else if(this->selectorPosX == 4 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[25])); // = "Z";
	else if(this->selectorPosX == 5 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[26])); // = " ";
	else if(this->selectorPosX == 6 && this->selectorPosY == 3) this->selectedChar = pgm_read_byte(&(SheikahChars[27])); // = ".";

	return this->selectedChar;
}


// 
void OLED_SSD1306::moveSelector(void)
{
	// Up
	if(digitalRead(BTN_UP_PIN) == HIGH) {
		// Store prevSelectorPosY
		this->prevSelectorPosY = this->selectorPosY;
		// Decrement selectorPosY
		if(this->selectorPosY <= 0) {
			this->selectorPosY = 3;
			this->prevSelectorPosY = 0;
		}
		else
			this->selectorPosY--;

		this->clearPrevSelector();
		this->setSelectorPos(this->selectorPosX, this->selectorPosY);
		delay(120);
	}

	// Down
	if(digitalRead(BTN_DOWN_PIN) == HIGH) {
		// Store prevSelectorPosY
		this->prevSelectorPosY = this->selectorPosY;
		// Increment selectorPosY
		if(this->selectorPosY > 3) {
			this->selectorPosY = 0;
			this->prevSelectorPosY = 3;
		}
		else
			this->selectorPosY++;

		this->clearPrevSelector();
		this->setSelectorPos(this->selectorPosX, this->selectorPosY);
		delay(120);
	}

	// Left
	if(digitalRead(BTN_LEFT_PIN) == HIGH) {
		// Store prevSelectorPosX
		this->prevSelectorPosX = this->selectorPosX;
		// Decrement selectorPosX
		if(this->selectorPosX <= 0) {
			this->selectorPosX = 6;
			this->prevSelectorPosX = 0;
		}
		else
			this->selectorPosX--;

		this->clearPrevSelector();
		this->setSelectorPos(this->selectorPosX, this->selectorPosY);
		delay(120);
	}

	// Right
	if(digitalRead(BTN_RIGHT_PIN) == HIGH) {
		// Store prevSelectorPosX
		this->prevSelectorPosX = this->selectorPosX;
		// Increment selectorPosX
		if(this->selectorPosX > 6) {
			this->selectorPosX = 0;
			this->prevSelectorPosX = 6;
		}
		else
			this->selectorPosX++;

		this->clearPrevSelector();
		this->setSelectorPos(this->selectorPosX, this->selectorPosY);
		delay(120);
	}

	// A
	if(digitalRead(BTN_A_PIN) == HIGH) {
		// Store prevCursorPosX
		this->prevCursorPosX = this->cursorPosX - 1;
		// Increment selectorPosX
		if(this->cursorPosX >= 20) {
			this->cursorPosX = 20;
			this->prevCursorPosX = 19;
		}
		else {
			this->cursorPosX++;
		}
		//this->clearPrevCursor();
		this->setCursorPos(this->cursorPosX, this->cursorPosY);
		this->print6x8Single(this->prevCursorPosX * 6, 7, this->getSelectedChar());
		this->print8x8Single(this->prevCursorPosX * 9, 5, this->getSelectedChar());
		//this->cursorPosX++;
		//this->setCursor(this->cursorPosX * 6, 7);
		delay(120);
	}

	// B
	if(digitalRead(BTN_B_PIN) == HIGH) {
		//this->clearLine();
		this->backspace();
		//this->backspace_8x8();
		
		// Store prevCursorPosX
		this->prevCursorPosX = this->cursorPosX + 1;
		
		delay(120);
	}

	/*this->print6x8Str(0, 4, "CursorX: ");
	this->printValueI(67, 4, this->cursorPosX);
	this->print6x8Str(0, 5, "pCursorX: ");
	this->printValueI(67, 5, this->prevCursorPosX);*/

	/*this->print6x8Str(0, 4, "SelectorX: ");
	this->printValueI(67, 4, this->selectorPosX);
	this->print6x8Str(0, 5, "SelectorY: ");
	this->printValueI(67, 5, this->selectorPosY);*/

	/*this->print6x8Str(0, 4, "Selected Char: [");
	this->print6x8Single(96, 4, this->getSelectedChar());
	this->print6x8Str(102, 4, "]");*/
	
	this->print6x8Str(0, 7, ">");

	/*this->print6x8Str(0, 6, "PreviousX: ");
	this->printValueI(67, 6, this->prevSelectorPosX);
	this->print6x8Str(0, 7, "PreviousY: ");
	this->printValueI(67, 7, this->prevSelectorPosY);*/
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

	//this->clearPrevCursor();

	if (x > OLED_WIDTH - 6)
	{
		x = 7;
		y++;
	}
	this->setPosition(x, y);
	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(Cursor[i])));
}


// 
void OLED_SSD1306::setCursorPos(unsigned char x, unsigned char y)
{
	this->prevCursorPosX = this->cursorPosX - 1;
	this->prevCursorPosY = this->cursorPosY - 1;
	if(x == 20) {
		this->prevCursorPosX = 19;
		x = 20;
	}
	/*if(x > 21) {
		this->prevCursorPosX = 0;
		x = 0;
	}
	else if(x < 0) {
		this->prevCursorPosX = 6;
		x = 6;
	}
	
	if(y > 3) {
		this->prevCursorPosY = 0;
		y = 0;
	}
	else if(y < 0) {
		this->prevCursorPosY = 3;
		y = 3;
	}*/
	this->cursorPosX = x;
	this->cursorPosY = y;
	this->setCursor(this->cursorPosX * 6, this->cursorPosY);
}


// 
void OLED_SSD1306::clearPrevCursor(void)
{
	unsigned char i = 0;

	this->setPosition(this->prevCursorPosX * 6, this->prevCursorPosY);

	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(CursorBlank[i])));
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

	this->setPosition(6, 7);

	for(j = 0; j < 120; j+=6)
	{
		for (i = 0; i < 6; i++)
			this->writeData(pgm_read_byte(&(CursorBlank[i])));
	}

	this->cursorPosX = 1;
	this->setCursor(this->cursorPosX * 6, this->cursorPosY);
}


// Backspace
void OLED_SSD1306::backspace(void)
{
	unsigned char i = 0;

	this->setPosition(this->cursorPosX * 6, 7);

	for (i = 0; i < 6; i++)
		this->writeData(pgm_read_byte(&(CursorBlank[i])));

	if(this->cursorPosX < 2)
		this->cursorPosX = 1;
	else
	{
		this->prevCursorPosX++;
		//this->clearPrevCursor();
		this->cursorPosX--;
		this->prevCursorPosX--;
	}
	this->setCursor(this->cursorPosX * 6, this->cursorPosY);
}


// Backspace Sheikah_8x8
void OLED_SSD1306::backspace_8x8(void)
{
	unsigned char i = 0;

	this->setPosition(this->cursorPosX * 9, 5);

	for (i = 0; i < 8; i++)
		this->writeData(pgm_read_byte(&(CursorBlank_8x8[i])));

	if(this->cursorPosX < 2)
		this->cursorPosX = 1;
	/*else
	{
		this->prevCursorPosX++;
		//this->clearPrevCursor();
		this->cursorPosX--;
		this->prevCursorPosX--;
	}*/
	//this->setCursor(this->cursorPosX * 9, 5);
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 						Display Configuration
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// 
void OLED_SSD1306::setStartColumn(unsigned char d)
{
	this->writeCmd(0x00 + d % 16); // Set Lower Column Start Address for Page Addressing Mode
								   // Default => 0x00
	this->writeCmd(0x10 + d / 16); // Set Higher Column Start Address for Page Addressing Mode
								   // Default => 0x10
}


// 
void OLED_SSD1306::setAddressingMode(unsigned char d)
{
	this->writeCmd(0x20); // Set EEPROM Addressing Mode
	this->writeCmd(d);	  // Default => 0x02
						  // 0x00 => Horizontal Addressing Mode
						  // 0x01 => Vertical Addressing Mode
						  // 0x02 => Page Addressing Mode
}


// 
void OLED_SSD1306::setColumnAddress(unsigned char a, unsigned char b)
{
	this->writeCmd(0x21); // Set Column Address
	this->writeCmd(a);	  // Default => 0x00 (Column Start Address)
	this->writeCmd(b);	  // Default => 0x7F (Column End Address)
}


// 
void OLED_SSD1306::setPageAddress(unsigned char a, unsigned char b)
{
	this->writeCmd(0x22); // Set Page Address
	this->writeCmd(a);	  // Default => 0x00 (Page Start Address)
	this->writeCmd(b);	  // Default => 0x07 (Page End Address)
}


// 
void OLED_SSD1306::setStartLine(unsigned char d)
{
	this->writeCmd(0x40 | d); // Set Display Start Line
						 	  // Default => 0x40 (0x00)
}


// 
void OLED_SSD1306::setContrastControl(unsigned char d)
{
	this->writeCmd(0x81); // Set Contrast Control
	this->writeCmd(d);	  // Default => 0x7F
}


// 
void OLED_SSD1306::setChargePump(unsigned char d)
{
	this->writeCmd(0x8D);	  // Set Charge Pump
	this->writeCmd(0x10 | d); // Default => 0x10
							  // 0x10 (0x00) => Disable Charge Pump
							  // 0x14 (0x04) => Enable Charge Pump
}


// 
void OLED_SSD1306::setSegmentRemap(unsigned char d)
{
	this->writeCmd(0xA0 | d); // Set Segment Re-Map
							  // Default => 0xA0
							  // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
							  // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}


// 
void OLED_SSD1306::setEntireDisplay(unsigned char d)
{
	this->writeCmd(0xA4 | d); // Set Entire Display On / Off
							  // Default => 0xA4
							  // 0xA4 (0x00) => Normal Display
							  // 0xA5 (0x01) => Entire Display On
}


// 
void OLED_SSD1306::setInverseDisplay(unsigned char d)
{
	this->writeCmd(0xA6 | d); // Set Inverse Display On/Off
							  // Default => 0xA6
							  // 0xA6 (0x00) => Normal Display
							  // 0xA7 (0x01) => Inverse Display On
}


// 
void OLED_SSD1306::setMultiplexRatio(unsigned char d)
{
	this->writeCmd(0xA8); // Set Multiplex Ratio
	this->writeCmd(d);	  // Default => 0x3F (1/64 Duty)
}


// 
void OLED_SSD1306::setDisplayOnOff(unsigned char d)
{
	this->writeCmd(0xAE | d); // Set Display On/Off
							  // Default => 0xAE
							  // 0xAE (0x00) => Display Off
							  // 0xAF (0x01) => Display On
}


// 
void OLED_SSD1306::setStartPage(unsigned char d)
{
	this->writeCmd(0xB0 | d); // Set Page Start Address for Page Addressing Mode
							  // Default => 0xB0 (0x00)
}


// 
void OLED_SSD1306::setCommonRemap(unsigned char d)
{
	this->writeCmd(0xC0 | d); // Set COM Output Scan Direction
							  // Default => 0xC0
							  // 0xC0 (0x00) => Scan from COM0 to 63
							  // 0xC8 (0x08) => Scan from COM63 to 0
}


// 
void OLED_SSD1306::setDisplayOffset(unsigned char d)
{
	this->writeCmd(0xD3); // Set Display Offset
	this->writeCmd(d);	  // Default => 0x00
}


// 
void OLED_SSD1306::setDisplayClock(unsigned char d)
{
	this->writeCmd(0xD5); // Set Display Clock Divide Ratio / Oscillator Frequency
	this->writeCmd(d);	  // Default => 0x80
						  // D[3:0] => Display Clock Divider
						  // D[7:4] => Oscillator Frequency
}


// 
void OLED_SSD1306::setPrechargePeriod(unsigned char d)
{
	this->writeCmd(0xD9); // Set Pre-Charge Period
	this->writeCmd(d);	  // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						  // D[3:0] => Phase 1 Period in 1~15 Display Clocks
						  // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


// 
void OLED_SSD1306::setCommonConfig(unsigned char d)
{
	this->writeCmd(0xDA);	  // Set COM Pins Hardware Configuration
	this->writeCmd(0x02 | d); // Default => 0x12 (0x10)
							  // Alternative COM Pin Configuration
							  // Disable COM Left/Right Re-Map
}


// 
void OLED_SSD1306::setVCOMH(unsigned char d)
{
	this->writeCmd(0xDB); // Set VCOMH Deselect Level
	this->writeCmd(d);	  // Default => 0x20 (0.77*VCC)
						  // 0x00 => 0.62*VCC
						  // 0x20 => 0.77*VCC
						  // 0x30 => 0.83*VCC
}


// 
void OLED_SSD1306::setNOP(void)
{
	this->writeCmd(0xE3); // Command for No Operation
}


// 
void OLED_SSD1306::initDisplay(void)
{
	unsigned char i;
	this->initPins();
	//OLED_PORT = 0x0F;
	//OLED_SCLH;
	//OLED_RSTL;
	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(RST_PIN, LOW);
	//for(i = 0; i < 100; i++) asm("nop");
	delay(50);
	//OLED_RSTH;
	digitalWrite(RST_PIN, HIGH);
	//delay(100);

	this->setDisplayOnOff(0x00);   		// Display Off (0x00/0x01)
	this->setDisplayClock(0x80);		// Set Clock as 100 Frames/Sec
	this->setMultiplexRatio(0x3F);  	// 1/64 Duty (0x0F~0x3F)
	this->setDisplayOffset(0x00);   	// Shift Mapping RAM Counter (0x00~0x3F)
	this->setStartLine(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	this->setChargePump(0x04);			// Enable Embedded DC/DC Converter (0x00/0x04)
	this->setAddressingMode(0x00);		// Set Page Addressing Mode (0x00/0x01/0x02)
	this->setSegmentRemap(0x01);		// Set SEG/Column Mapping
	this->setColumnAddress(0x00, 0x7F); // 
	this->setPageAddress(0x00, 0x07);	// 
	this->setCommonRemap(0x08);			// Set COM/Row Scan Direction
	this->setCommonConfig(0x10);		// Set Sequential Configuration (0x00/0x10)
	this->setContrastControl(0xCF); 	// Set SEG Output Current
	this->setPrechargePeriod(0xF1); 	// Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	this->setVCOMH(0x20);				// Set VCOM Deselect Level
	this->setEntireDisplay(0x00);   	// Disable Entire Display On (0x00/0x01)
	this->setInverseDisplay(0x00);  	// Disable Inverse Display On (0x00/0x01)
	this->setDisplayOnOff(0x01);   		// Display On (0x00/0x01)

	this->fill(0x00);					// Clear all
	this->setPosition(0, 0);			// Top left
	//OLED_SetFont("Shard_6x8");
	//this->writeCmd(ACTIVATESCROLL);
}
