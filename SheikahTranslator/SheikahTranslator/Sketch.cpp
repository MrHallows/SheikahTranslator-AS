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


//#include <Arduino.h> // Included in "src/OLED_SSD1306.h"
//#include <SPI.h> // Included in "utilities/SRAM_23LC1024.h"
//#include <avr/io.h>

#include "src/OLED_SSD1306.h"
//#include "utilities/EEPROM_24LC64.h"
#include "utilities/SRAM_23LC1024.h"


OLED_SSD1306 OLED;
//EEPROM_24LC64 EEPROM;
//SRAM_23LC1024 SRAM;


/*void EEPROM_TestSetup()
{
	unsigned char testData[] = "Fuck this bullshit! Fuck this!"; // Data to write

	Wire.begin();
	Serial.begin(9600);
	EEPROM.writePage(0x50, 0, (unsigned char*)testData, sizeof(testData)); // Write to EEPROM

	delay(100); // Add a small delay (1/10th of a second)

	Serial.println(F("Data was successfully written to memory (EEPROM)!"));
}

void EEPROM_TestLoop()
{
	int addr = 0; // First address
	unsigned char b = EEPROM.readByte(0x50, 0); // Access the first address from memory

	while(b != -1)
	{
		Serial.print(F("Data from memory address ["));
		Serial.print(addr);
		Serial.print(F("]: "));
		Serial.print((char)b); // Print data to the Serial Port
		Serial.println(F(" ")); // Newline
		addr++; // Next address
		b = EEPROM.readByte(0x50, addr); // Access the next address from memory
	}

	Serial.println(F(" ")); // Newline
	delay(2000); // 2 second delay
}*/


void setup()
{
	//OLED.initPins(); // Called in OLED.initDisplay()
	OLED.initDisplay();

	//Wire.begin();
	//Serial.begin(9600);
	//while(!Serial);

	//OLED.printValueI(0, 3, sizeof(Shard_6x8));
	//Serial.println(F("Data was successfully written to memory (EEPROM)!"), sizeof(SheikahEyeSplash));
	
	/*EEPROM.writeBitmap(0x50, 0, SheikahEyeSplash, sizeof(SheikahEyeSplash));
	delay(100);
	Serial.println(F("Data was successfully written to memory (EEPROM)!"));
	*/

	//OLED.print6x8Str(0, 3, "Data was successfully written to memory (EEPROM)!");

	/*unsigned char bitmapArray[1024];
	int addr0 = 0;
	int i = 0;

	for(i = 0; i < 28; i++)
	{
		bitmapArray[i] = EEPROM.readByte(0x50, addr0);
		addr0++;
	}*/

	//unsigned char receivedArray = EEPROM.loadBitmap(0x50, 0, 1024);
	//OLED.drawBitmap(0, 0, OLED_WIDTH, OLED_HEIGHT, EEPROM.loadBitmap(0x50, 0, sizeof(SheikahEyeSplash)));

	OLED.loadSplash();
	OLED.loadTitle();
	
	//OLED.drawBitmap(32, 0, 96, OLED_HEIGHT, Mickey);
	//delay(10000); //15000

	/*OLED.drawLine(0, 35, 127, 35);
	OLED.drawLine(0, 54, 127, 54);
	OLED.flushBuffer();
	OLED.setSelectorPos(OLED.selectorPosX, OLED.selectorPosY);
	OLED.setCursor(OLED.cursorPosX * 6, OLED.cursorPosY);*/


	/*
	OLED.print6x8Str(0, 0, "Graphics test..");
	delay(2000);
	OLED.clearBuffer();

	// Draw pixel
	OLED.print6x8Str(0, 0, "Draw pixel..");
	delay(2000);
	OLED.clearBuffer();
	OLED.drawPixel(8, 8);
	OLED.flushBuffer();
	delay(2000);
	OLED.clearBuffer();

	// Draw line
	OLED.print6x8Str(0, 0, "Draw line..");
	delay(2000);
	OLED.clearBuffer();
	OLED.drawLine(0, 54, 127, 54);
	OLED.flushBuffer();
	delay(2000);
	OLED.clearBuffer();

	// Draw rectangle
	OLED.print6x8Str(0, 0, "Draw rectangle..");
	delay(2000);
	OLED.clearBuffer();
	OLED.drawRect(16, 16, 80, 60);
	OLED.flushBuffer();
	delay(2000);
	OLED.clearBuffer();

	// Draw circle
	OLED.print6x8Str(0, 0, "Draw circle..");
	delay(2000);
	OLED.clearBuffer();
	OLED.drawCircle(96, 50, 10);
	OLED.flushBuffer();
	delay(2000);
	OLED.clearBuffer();*/

	/*for(unsigned char i = 0; i < 1024; i++)
	{
        for(unsigned char x = 0; x < 16; x++)
		{
            OLED.writeData(OLED.pattern1[x]); //0x81
            i++;
			//delay(100);
        }
        i--;
    }*/
	//OLED.fill(OLED.pattern1);
	//OLED.fill(OLED.pattern2);

	//EEPROM_TestSetup();
	//EEPROM_TestLoop();
}

void loop()
{
	//OLED.printSheikahMap();
	OLED.displayMenu();
	OLED.moveSelector();

	//EEPROM_TestLoop();
}
