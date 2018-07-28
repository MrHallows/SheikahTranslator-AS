/* *
 * Class for interfacing with the Atmel 24LC64 EEPROM chip (I2C)
 * 
 * Wiring:
 * 
 * 		24LC64       Arduino
 * 		--------------------
 * 		A0...........GND
 * 		A1...........GND
 * 		A2...........GND
 * 		VSS..........GND
 * 		SDA..........A4
 * 		SCL..........A5
 * 		WP...........GND (Until Write Protection is necessary)
 * 		VCC..........5V
 */
#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>
#include <Wire.h>


class EEPROM_24LC64
{
public:
	void init(void);
    void writeByte(int deviceAddr, unsigned int chipAddr, unsigned char data);
    void writePage(int deviceAddr, unsigned int chipAddr, unsigned char* data, unsigned char length);
    unsigned char readByte(int deviceAddr, unsigned int chipAddr);
    void readBuffer(int deviceAddr, unsigned int chipAddr, unsigned char* buffer, int length);

	void writeBitmap(int deviceAddr, unsigned int chipAddr, unsigned char* data, unsigned char length);
	unsigned char loadBitmap(int deviceAddr, unsigned int chipAddr, unsigned char length);
};

#endif /* defined("MEMORY_H") */