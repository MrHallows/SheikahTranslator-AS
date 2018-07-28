/* *
 * Class for interfacing with the Microchip 23LC1024 SRAM chip (SPI)
 * 
 * Wiring:
 * 
 * 		23LC1024       Arduino
 * 		----------------------
 * 		A0.............GND
 * 		A1.............GND
 * 		A2.............GND
 * 		VSS............GND
 * 		SDA............A4
 * 		SCL............A5
 * 		WP.............GND (Until Write Protection is necessary)
 * 		VCC............5V
 */
#pragma once

#ifndef SRAM_H
#define SRAM_H

//#include <Arduino.h>
#include <SPI.h>


/* *
 * SRAM OpCodes: Commands to the 23LC1024 chip
 */
#define RDMR        5 		// Read the Mode Register
#define WRMR        1 		// Write to the Mode Register
#define READ        3 		// Read command
#define WRITE       2 		// Write command
#define RSTIO     	0xFF 	// Reset memory to SPI mode
#define ByteMode    0x00 	// Byte Mode (Read/Write one byte at a time)
#define Sequential  0x40 	// Sequential Mode (Read/Write blocks of memory)
#define CS          10 		// Chip Select
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
signed char int8_t;           ///< 8-bit signed integer.
unsigned char uint8_t;        ///< 8-bit unsigned integer.
signed short int16_t;         ///< 16-bit signed integer.
unsigned short uint16_t;      ///< 16-bit unsigned integer.
signed int int32_t;           ///< 32-bit signed integer.
unsigned int uint32_t;        ///< 32-bit unsigned integer.
signed long long int64_t;     ///< 64-bit signed integer.
unsigned long long uint64_t;  ///< 64-bit unsigned integer.
*/


class SRAM_23LC1024
{
	//
	void init(void);
	void setMode(unsigned char mode);
	void writeByte(unsigned int address, unsigned char data_byte);
	unsigned char readByte(unsigned int address);
	void writeArray(unsigned int address, unsigned char* data, unsigned short big);
	void readArray(unsigned int address, unsigned char* data, unsigned short big);
};

#endif /* defined("SRAM_H") */