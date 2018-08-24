/* *
 * Class for interfacing with the Atmel 24LC64 EEPROM chip (I2C)
 */

#include "EEPROM_24LC64.h"


void EEPROM_24LC64::init(void)
{
	// Initialize Serial and I2C connections
	Serial.begin(57600);
	Wire.begin();
}


void EEPROM_24LC64::writeByte(int deviceAddr, unsigned int chipAddr, unsigned char data)
{
    int rdata = data;

    Wire.beginTransmission(deviceAddr);
    Wire.write((int)(chipAddr >> 8)); // MSB (Most Significant Bit)
    Wire.write((int)(chipAddr & 0xFF)); // LSB (Least Significant Bit)
    Wire.write(rdata);
    Wire.endTransmission();

    delay(10);
}

// WARNING: Address is a page address, 6-bit end will wrap around.
// Maximum of 30 bytes due to Wire library's 32 byte buffer
void EEPROM_24LC64::writePage(int deviceAddr, unsigned int chipAddr, unsigned char* data, unsigned char length)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write((int)(chipAddr >> 8)); // MSB (Most Significant Bit)
    Wire.write((int)(chipAddr & 0xFF)); // LSB (Least Significant Bit)
    unsigned char c;
    for(c = 0; c < length; c++)
	{
        Wire.write(data[c]);
        delay(10);
	}
    Wire.endTransmission();
}

unsigned char EEPROM_24LC64::readByte(int deviceAddr, unsigned int chipAddr)
{
    unsigned char rdata = 0xFF;

    Wire.beginTransmission(deviceAddr);
    Wire.write((int)(chipAddr >> 8)); // MSB (Most Significant Bit)
    Wire.write((int)(chipAddr & 0xFF)); // LSB (Least Significant Bit)
    Wire.endTransmission();

    Wire.requestFrom(deviceAddr, 1);
    if(Wire.available()) rdata = Wire.read();
    
    return rdata;
}

// Do not read more than 30 at a time..
void EEPROM_24LC64::readBuffer(int deviceAddr, unsigned int chipAddr, unsigned char* buffer, int length)
{
    Wire.beginTransmission(deviceAddr);
    Wire.write((int)(chipAddr >> 8)); // MSB (Most Significant Bit)
    Wire.write((int)(chipAddr & 0xFF)); // LSB (Least Significant Bit)
    Wire.endTransmission();

    Wire.requestFrom(deviceAddr, length);
    int c = 0;
    for(c = 0; c < length; c++)
        if(Wire.available()) buffer[c] = Wire.read();
}

void EEPROM_24LC64::writeBitmap(int deviceAddr, unsigned int chipAddr, unsigned char* data, unsigned char length)
{
    /*unsigned char c;
	int i = 0;

    for(c = 0; c < length; c++)
	{
        writeByte(deviceAddr, (chipAddr + i), data[c]);
		i++;
		delay(100);
	}*/

	unsigned char c = 0, j = 0, buffer = 30;
	int i = 0;

	while(j < length)
	{
		for(c = 0; c < buffer; c++)
		{
			writePage(deviceAddr, (chipAddr + i), &data[j], buffer);
			delay(10);
			i++;
			j++;
		}
		c = 0;
	}
}

unsigned char EEPROM_24LC64::loadBitmap(int deviceAddr, unsigned int chipAddr, unsigned char length)
{
	unsigned int addr = chipAddr;
	unsigned char bitmapArray[length];
	unsigned char i = 0;

	for(i = 0; i < length; i++)
	{
		bitmapArray[i] = readByte(deviceAddr, addr);
		addr++;
	}

	return *bitmapArray;
}
