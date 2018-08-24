/* *
 * Class for interfacing with the Atmel 24LC64 EEPROM chip (I2C)
 * 
 * Pinout:
 *
 *		 A0 +--u--+ Vcc
 *		 A1 +     + WP
 *		 A2 +     + SCL
 *		Vss +-----+ SDA
 * 
 * Wiring:
 * 
 * 		24LC64       Arduino
 * 		--------------------
 * 		A0...........GND
 * 		A1...........GND
 * 		A2...........GND
 * 		Vss..........GND
 * 		SDA..........A4
 * 		SCL..........A5
 * 		WP...........GND (Until Write Protection is necessary)
 * 		Vcc..........5V
 *
 *
 * 		25AA1024     Arduino
 * 		--------------------
 * 		A0...........GND
 * 		A1...........GND
 * 		A2...........5V
 * 		Vss..........GND
 * 		SDA..........A4
 * 		SCL..........A5
 * 		WP...........GND (Until Write Protection is necessary)
 * 		Vcc..........5V
 */
#pragma once

#ifndef __EEPROM_24LC64_H__
#define __EEPROM_24LC64_H__

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

#endif /* defined("__EEPROM_24LC64_H__") */