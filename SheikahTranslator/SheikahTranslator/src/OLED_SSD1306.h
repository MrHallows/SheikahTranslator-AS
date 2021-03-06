/* *
 * Class for interfacing with the SSD1306 (128x64px) OLED display
 * 
 * Wiring:
 * 
 * 		OLED         Arduino
 * 		--------------------
 * 		GND..........GND
 * 		VCC..........5V
 * 		D0...........10
 * 		D1...........9
 * 		RES..........13
 * 		DC...........11
 * 		CS...........GND
 */
#pragma once

#ifndef __OLED_SSD1306_H__
#define __OLED_SSD1306_H__

//#include <Arduino.h>

#include "../utilities/EEPROM_24LC64.h"
//#include "../utilities/SRAM_23LC1024.h"
//#include "../utilities/RTC_MPC7940M.h"
#include "../include/Shard_6x8.h"
#include "../include/Sheikah_8x8.h"
#include "../include/F8x16.h"
#include "../include/F6x8.h"
#include "../include/F8x16.h"
#include "../include/SheikahEyeSplash.h"


// OLED Resolution
#define OLED_WIDTH						0x80 // 128
#define OLED_HEIGHT						0x40 // 64

// Pixel Status
#define OFF								0
#define ON								1
#define INVERSE							2

// OLED Pins (4-wire SPI mode)
//#define SCK_PIN 						10 // D0
//#define MOSI_PIN 						9  // D1
//#define RST_PIN 						13 // RST
//#define DC_PIN  						11 // DC

// OLED Pins (4-wire SPI mode)
#define SCK_PIN 						10 // D0
#define MOSI_PIN 						9  // D1
#define RST_PIN 						13 // RST
#define DC_PIN  						11 // DC
#define CS_PIN  						12 // CS

// Button Pins
#define BTN_UP_PIN						5  // D5
#define BTN_DOWN_PIN					4  // D4
#define BTN_LEFT_PIN					3  // D3
#define BTN_RIGHT_PIN					2  // D2
#define BTN_A_PIN						6  // D6
#define BTN_B_PIN						7  // D7
//#define BTN_LS_PIN // Left Shoulder (NOT YET ADDED)
//#define BTN_RS_PIN // Right Shoulder (NOT YET ADDED)
//#define BTN_START_PIN // (NOT YET ADDED)
//#define BTN_SELECT_PIN // (NOT YET ADDED)



// Globals
static const unsigned char Selector[6] PROGMEM = { 0x00, 0x7C, 0x38, 0x10, 0x00, 0x00 };
static const unsigned char SelectorBlank[6] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const unsigned char SheikahChars[38] PROGMEM = { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x20, 0x2E, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
static const unsigned char Cursor[6] PROGMEM = { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 };
static const unsigned char CursorBlank[6] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const unsigned char CursorBlank_8x8[8] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const unsigned char HR_Top[6] PROGMEM = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
static const unsigned char HR_Middle[6] PROGMEM = { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 };
static const unsigned char HR_Bottom[6] PROGMEM = { 0x40, 0x40, 0x40, 0x40, 0x40, 0x40 };

// Box grid pattern
static const unsigned char boxPattern[64] PROGMEM = {
	0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00,
	0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00
};

// Cross weave pattern
static const unsigned char weavePattern[64] PROGMEM = {
	0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81,
	0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
};


class OLED_SSD1306
{
public:
	// Display
	void initPins(void); 										// Initialize pin configurations
	void initDisplay(void); 									// Initialize the display
	void writeCmd(unsigned char cmd);							// Write command
	void writeData(unsigned char data);							// Write data
	void cls(void); 											// Clear the screen
	void fill(unsigned char data);								// Fill the screen
	void clearBuffer(void);										// Clear the buffer
	void clearBuffer(unsigned char page_start, unsigned char page_end);		// Clear the buffer
	void flushBuffer(void);										// Push buffer data to the screen
	void setPosition(unsigned char x, unsigned char y); 		// Set the coordinates
	void setDelay(unsigned int ms); 							// Set delay in milliseconds

	// Screens & Menus
	void loadSplash(void);										// Load the splash screen bitmap from flash memory
	void loadTitle(void);										// Load the title screen
	void mainMenu(void);										// Menu for selecting other menus and screens
	void settingsMenu(void);									// Display the Settings menu
	void setContrast(void);										// Contrast settings
	void setInverse(void);										// Inverse display settings
	void graphicsTest(void);									// Display the graphics test

	// Text
	void print6x8Char(unsigned char x, unsigned char y, unsigned char ch); 						// Print char (byte) at the given coordinates
	void print6x8Str(unsigned char x, unsigned char y, const char ch[]); 						// Print string at the given coordinates (Shard_6x8 font)
	//void drawString(int x, int y, String text);
	void print6x8Single(unsigned char x, unsigned char y, char ch); 							// Print a single character (Shard_6x8 font)
	void print8x8Str(unsigned char x, unsigned char y, const char ch[]); 						// Print string at the given coordinates (Sheikah_8x8 font)
	void print8x8Single(unsigned char x, unsigned char y, char ch); 							// Print a single character (Sheikah_8x8 font)
	void print8x16Str(unsigned char x, unsigned char y, const char ch[]); 					// Print string at the given coordinates (F8x16 font)
	void printValueC(unsigned char x, unsigned char y, char data); 								// Print the value of a char
	void printValueI(unsigned char x, unsigned char y, int data); 								// Print the value of an integer
	void printShortValueI(unsigned char x, unsigned char y, int data, unsigned char digits);	// Print the value of an integer (user-specified digit count)
	void printValueF(unsigned char x, unsigned char y, float data, unsigned char num); 			// Print the value of a float
	void printValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num); 	// Print the value of a floating point

	// Graphics
	void drawPixel(int x, int y, int status = ON);																	// Draw a single pixel at the given coordinates (set colour on/off)
	void drawLine(int x1, int y1, int x2, int y2, int spacing, int status = ON);									// Draw a line from point A (x1, y1) to point B (x2, y2)
	void drawRect(int x1, int y1, int x2, int y2, int spacing, int status = ON);									// Draw a rectangle, top left (x1, y1) to bottom right (x2, y2)
	void drawDotGrid(int x1, int y1, int x2, int y2, int spacing, int status = ON);									// Draw a dot grid, top left (x1, y1) to bottom right (x2, y2)
	void drawLineGrid(int x1, int y1, int x2, int y2, int spacing, int status = ON);								// Draw a line grid, top left (x1, y1) to bottom right (x2, y2)
	void drawCircle(int x0, int y0, int radius, int status = ON);													// Draw a circle at the given coordinates with the given radius
	void drawBitmap(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char bmp[]); 	// Draw a bitmap image with the given coordinates and size

	// Sheikah Character Map
	void printSheikahMap(void); 								// Display the Sheikah character map
	void printSheikahNums(void); 								// Display the Sheikah number map
	void setSelector(unsigned char x, unsigned char y); 		// Place the selector at the given position
	void setSelectorPos(unsigned char x, unsigned char y); 		// 
	void clearPrevSelector(void); 								// 
	inline unsigned char getSelectedChar(void); 				// 
	void moveSelector(void); 									// 

	// Terminal Input
	void setCursor(unsigned char x, unsigned char y); 			// 
	void setCursorPos(unsigned char x, unsigned char y); 		// 
	void clearPrevCursor(void); 								// 
	void printLine(void); 										// 
	void clearLine(void); 										// 
	void clearLine8x8(void);									// 
	void backspace(void); 										// 
	void backspace_8x8(void); 									// 

	// Display Configuration
	void setStartColumn(unsigned char d); 						// 
	void setAddressingMode(unsigned char d); 					// 
	void setColumnAddress(unsigned char a, unsigned char b); 	// 
	void setPageAddress(unsigned char a, unsigned char b); 		// 
	void setStartLine(unsigned char d); 						// 
	void setContrastControl(unsigned char d); 					// 
	void setChargePump(unsigned char d); 						// 
	void setSegmentRemap(unsigned char d); 						// 
	void setEntireDisplay(unsigned char d); 					// 
	void setInverseDisplay(unsigned char d); 					// 
	void setMultiplexRatio(unsigned char d); 					// 
	void setDisplayOnOff(unsigned char d); 						// 
	void setStartPage(unsigned char d); 						// 
	void setCommonRemap(unsigned char d); 						// 
	void setDisplayOffset(unsigned char d); 					// 
	void setDisplayClock(unsigned char d); 						// 
	void setPrechargePeriod(unsigned char d); 					// 
	void setCommonConfig(unsigned char d); 						// 
	void setVCOMH(unsigned char d); 							// 
	void setNOP(void); 											// 
	
	unsigned char buffer[1024]; 								// Screen buffer
	unsigned char currentMenu = 0;
	unsigned char prevMenu;
	unsigned char contrast;
	unsigned char contrastBar;
	bool isInverted;
	unsigned char inverse;
	unsigned char inverseBar;

	unsigned char selectorPosX = 0;
	unsigned char selectorPosY = 0;
	unsigned char prevSelectorPosX;
	unsigned char prevSelectorPosY;
	unsigned char selectedChar;

	unsigned char cursorPosX = 1;
	unsigned char cursorPosY = 7;
	unsigned char prevCursorPosX;
	unsigned char prevCursorPosY;

	unsigned char cursor8x8PosX = 0;
	unsigned char cursor8x8PosY = 5;
	unsigned char prevCursor8x8PosX;
	unsigned char prevCursor8x8PosY;
};

#endif /* defined("__OLED_SSD1306_H__") */