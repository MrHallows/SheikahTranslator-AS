/**/

#include "SRAM_23LC1024.h"


void SRAM_23LC1024::init(void)
{
	// Initialize Serial and SPI connections
	Serial.begin(9600);
	SPI.begin();
}


// Set the SRAM chip to either single or sequential byte mode
void SRAM_23LC1024::setMode(unsigned char mode)
{
	digitalWrite(CS, LOW); 							// Set SPI slave select LOW
	SPI.transfer(WRMR); 							// Command to write to the mode register
	SPI.transfer(mode); 							// Set to the mode passed in
	digitalWrite(CS, HIGH); 						// Release chip select to end command
}


void SRAM_23LC1024::writeByte(unsigned int address, unsigned char data_byte)
{
	digitalWrite(CS, LOW); 							// Set SPI slave select LOW
	SPI.transfer(WRITE); 							// Send WRITE command to the SRAM
	SPI.transfer((unsigned char)(address >> 16)); 	// Send high byte of address
	SPI.transfer((unsigned char)(address >> 8)); 	// Send middle byte of address
	SPI.transfer((unsigned char)address); 			// Send low byte of address
	SPI.transfer(data_byte); 						// Write the data to the SRAM
	digitalWrite(CS, HIGH); 						// Release chip select to end command
}


unsigned char SRAM_23LC1024::readByte(unsigned int address)
{
	unsigned char read_byte;
	digitalWrite(CS, LOW); 							// Set SPI slave select LOW
	SPI.transfer(READ); 							// Send READ command to the SRAM
	SPI.transfer((unsigned char)(address >> 16)); 	// Send high byte of address
	SPI.transfer((unsigned char)(address >> 8)); 	// Send middle byte of address
	SPI.transfer((unsigned char)address); 			// Send low byte of address
	read_byte = SPI.transfer(0x00); 				// Read the byte stored at that address
	digitalWrite(CS, HIGH); 						// Release chip select to end command

	return read_byte; 								// Send the data back to the calling function
}


void SRAM_23LC1024::writeArray(unsigned int address, unsigned char* data, unsigned short size)
{
	unsigned short i = 0; 							// Loop counter
	digitalWrite(CS, LOW); 							// Set SPI slave select LOW
	SPI.transfer(WRITE); 							// Send WRITE command to the SRAM
	SPI.transfer((unsigned char)(address >> 16)); 	// Send high byte of address
	SPI.transfer((unsigned char)(address >> 8)); 	// Send middle byte of address
	SPI.transfer((unsigned char)address); 			// Send low byte of address
	SPI.transfer(data, size); 						// Write an array of data to the SRAM => requires array name & size
	digitalWrite(CS, HIGH); 						// Release chip select to end command
}


void SRAM_23LC1024::readArray(unsigned int address, unsigned char* data, unsigned short size)
{
	unsigned short i = 0; 							// Loop counter
	digitalWrite(CS, LOW); 							// Set SPI slave select LOW
	SPI.transfer(READ); 							// Send READ command to the SRAM
	SPI.transfer((unsigned char)(address >> 16)); 	// Send high byte of address
	SPI.transfer((unsigned char)(address >> 8)); 	// Send middle byte of address
	SPI.transfer((unsigned char)address); 			// Send low byte of address
	for(i = 0; i < size; i++)
		data[i] = SPI.transfer(0x00); 				// Read the byte stored at that address
	digitalWrite(CS, HIGH); 						// Release chip select to end command
}
