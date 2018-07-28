/* *
 * Class for interfacing with the SSD1306 OLED display
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

#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H


#include <Arduino.h>
#include <avr/pgmspace.h>

//#include "../utilities/Buttons.h"
#include "../include/Shard_6x8.h"
#include "../include/Sheikah_8x8.h"
#include "../include/F6x8.h"
#include "../include/F8x16.h"
#include "../include/SheikahEyeSplash.h"
//#include "../include/Mickey.h"


// OLED Resolution
#define OLED_WIDTH						0x80 // 128
#define OLED_HEIGHT						0x40 // 64

// OLED Pins
#define SCL_PIN 						10 // D0
#define SDA_PIN 						9  // D1
#define RST_PIN 						13 // RST
#define DC_PIN  						11 // DC

// Button Pins
#define BTN_UP_PIN						5 // D5
#define BTN_DOWN_PIN					4 // D4
#define BTN_LEFT_PIN					3 // D3
#define BTN_RIGHT_PIN					2 // D2
#define BTN_A_PIN						6 // D6
#define BTN_B_PIN						7 // D7
//#define BTN_L_PIN // Left Bumper (NOT YET ADDED)
//#define BTN_R_PIN // Right Bumper (NOT YET ADDED)
//#define BTN_START_PIN // (NOT YET ADDED)
//#define BTN_SELECT_PIN // (NOT YET ADDED)

// Menus
#define SHEIKAH_CHAR					0
#define SHEIKAH_NUMS					1
#define SETTINGS						2

// Display Commands
/*
#define BLACK 							0
#define WHITE 							1
#define INVERSE 						2

#define WIDTH_POS 						0
#define HEIGHT_POS 						1
#define FIRST_CHAR_POS 					2
#define CHAR_NUM_POS 					3
#define CHAR_WIDTH_START_POS 			4

#define TEXT_ALIGN_LEFT 				0
#define TEXT_ALIGN_CENTER 				1
#define TEXT_ALIGN_RIGHT 				2

#define CHARGEPUMP 						0x8D
#define COLUMNADDR 						0x21
#define COMSCANDEC 						0xC8
#define COMSCANINC 						0xC0
#define DISPLAYALLON 					0xA5
#define DISPLAYALLON_RESUME 			0xA4
#define DISPLAYOFF 						0xAE
#define DISPLAYON 						0xAF
#define EXTERNALVCC 					0x1
#define INVERTDISPLAY 					0xA7
#define MEMORYMODE 						0x20
#define NORMALDISPLAY 					0xA6
#define PAGEADDR 						0x22
#define SEGREMAP 						0xA0
#define SETCOMPINS 						0xDA
#define SETCONTRAST 					0x81
#define SETDISPLAYCLOCKDIV 				0xD5
#define SETDISPLAYOFFSET 				0xD3
#define SETHIGHCOLUMN 					0x10
#define SETLOWCOLUMN 					0x00
#define SETMULTIPLEX 					0xA8
#define SETPRECHARGE 					0xD9
#define SETSEGMENTREMAP 				0xA1
#define SETSTARTLINE 					0x40
#define SETVCOMDETECT 					0xDB
#define SWITCHCAPVCC 					0x2

// Scroll
#define ACTIVATESCROLL 					0x2F
#define DEACTIVATESCROLL 				0x2E
#define SETVERTICALSCROLLAREA 			0xA3
#define RIGHTHORIZONTALSCROLL 			0x26
#define LEFTHORIZONTALSCROLL 			0x27
#define VERTICALRIGHTHORIZONTALSCROLL	0x29
#define VERTICALLEFTHORIZONTALSCROLL	0x2A
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Alternative Display Command Table
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */
// SLA (0x3C) + WRITE_MODE (0x00) =  0x78 (0b01111000)
//#define OLED_I2C_ADDRESS   			0x3C
/*
// Control byte
#define OLED_CONTROL_BYTE_CMD_SINGLE    0x80
#define OLED_CONTROL_BYTE_CMD_STREAM    0x00
#define OLED_CONTROL_BYTE_DATA_STREAM   0x40

// Fundamental commands (pg.28)
#define OLED_CMD_SET_CONTRAST           0x81    // follow with 0x7F
#define OLED_CMD_DISPLAY_RAM            0xA4
#define OLED_CMD_DISPLAY_ALLON          0xA5
#define OLED_CMD_DISPLAY_NORMAL         0xA6
#define OLED_CMD_DISPLAY_INVERTED       0xA7
#define OLED_CMD_DISPLAY_OFF            0xAE	// follow with 0x00
#define OLED_CMD_DISPLAY_ON             0xAF	// follow with 0x01

// Addressing Command Table (pg.30)
#define OLED_CMD_SET_MEMORY_ADDR_MODE   0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_CMD_SET_COLUMN_RANGE       0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_CMD_SET_PAGE_RANGE         0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7

// Hardware Config (pg.31)
#define OLED_CMD_SET_DISPLAY_START_LINE 0x40
#define OLED_CMD_SET_SEGMENT_REMAP      0xA1    // (0xA0/0xA1)
#define OLED_CMD_SET_MUX_RATIO          0xA8    // follow with 0x3F = 64 MUX
#define OLED_CMD_SET_COM_SCAN_MODE      0xC8    // ()
#define OLED_CMD_SET_DISPLAY_OFFSET     0xD3    // follow with 0x00
#define OLED_CMD_SET_COM_PIN_MAP        0xDA    // follow with 0x12
#define OLED_CMD_NOP                    0xE3    // NOP

// Timing and Driving Scheme (pg.32)
#define OLED_CMD_SET_DISPLAY_CLK_DIV    0xD5    // follow with 0x80
#define OLED_CMD_SET_PRECHARGE          0xD9    // follow with 0xF1
#define OLED_CMD_SET_VCOMH_DESELCT      0xDB    // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP        0x8D    // follow with 0x14
*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



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


class OLED_SSD1306
{
public:
    //OLED()

	// Display
	void initDisplay(void); 									// Initialize the display
	void initPins(void); 										// Initialize pin configurations
	void writeCmd(unsigned char cmd);							// Write command
	void writeData(unsigned char data);							// Write data
	void cls(void); 											// Clear the screen
	void fill(unsigned char data);								// Fill the screen
	void clearBuffer(void);										// Clear the buffer
	void flushBuffer(void);										// Push buffer data to the screen
	void setPosition(unsigned char x, unsigned char y); 		// Set the coordinates
	void loadSplash(void);										// Load the splash screen bitmap from flash memory
	void loadTitle(void);										// Load the title screen
	void displayMenu(void);										// Options: SHEIKAH_CHAR, SHEIKAH_NUMS, SETTINGS
	void setDelay(unsigned int ms); 							// Set delay in milliseconds

	// Text
	void print6x8Char(unsigned char x, unsigned char y, unsigned char ch); 						// Print char (byte) at the given coordinates
	void print6x8Str(unsigned char x, unsigned char y, unsigned char ch[]); 					// Print string at the given coordinates (Shard_6x8 font)
	void print6x8Single(unsigned char x, unsigned char y, char ch); 							// Print a single character (Shard_6x8 font)
	void print8x8Str(unsigned char x, unsigned char y, unsigned char ch[]); 					// Print string at the given coordinates (Sheikah_8x8 font)
	void print8x8Single(unsigned char x, unsigned char y, char ch); 							// Print a single character (Sheikah_8x8 font)
	void print8x16Str(unsigned char x, unsigned char y, unsigned char ch[]); 					// Print string at the given coordinates (F8x16 font)
	void printValueC(unsigned char x, unsigned char y, char data); 								// 
	void printValueI(unsigned char x, unsigned char y, int data); 								// 
	void printValueF(unsigned char x, unsigned char y, float data, unsigned char num); 			// 
	void printValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num); 	// 

	// Graphics
	void drawPixel(int x, int y);																					// Draw a single pixel at the given coordinates
	void drawLine(int x1, int y1, int x2, int y2);																// Draw a line from point A (x1, y1) to point B (x2, y2)
	void drawRect(int x1, int y1, int x2, int y2);																// Draw a rectangle, top left (x1, y1) to bottom right (x2, y2)
	void drawCircle(int x0, int y0, int radius);																	// Draw a circle at the given coordinates with the given radius
	void drawBitmap(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char bmp[]); 	// Draw a bitmap image with the given coordinates and size

	// Sheikah Character Map
	void printSheikahMap(void); 								// Display the Sheikah character map
	void printSheikahNums(void); 								// Display the Sheikah number map
	void setSelector(unsigned char x, unsigned char y); 		// Place the selector at the given position
	void setSelectorPos(unsigned char x, unsigned char y); 		// 
	void clearPrevSelector(void); 								// 
	unsigned char getSelectedChar(void); 						// 
	void moveSelector(void); 									// 

	// Terminal Input
	void setCursor(unsigned char x, unsigned char y); 			// 
	void setCursorPos(unsigned char x, unsigned char y); 		// 
	void clearPrevCursor(void); 								// 
	void printLine(void); 										// 
	void clearLine(void); 										// 
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
	unsigned char activeMenu = SHEIKAH_CHAR;

	unsigned char selectorPosX = 0;
	unsigned char selectorPosY = 0;
	unsigned char prevSelectorPosX;
	unsigned char prevSelectorPosY;
	unsigned char selectedChar;

	unsigned char cursorPosX = 1;
	unsigned char cursorPosY = 7;
	unsigned char prevCursorPosX;
	unsigned char prevCursorPosY;

	// Box pattern
	/*unsigned char pattern1[64] PROGMEM = {
		0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00,
		0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00
	};*/

	// Cross weave pattern
	/*unsigned char pattern2[64] PROGMEM = {
		0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81,
		0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
	};*/

private:
	//unsigned int frame;
};


#endif /* defined("OLED_SSD1306_H") */