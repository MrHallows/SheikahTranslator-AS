/* *
 * RTC_MPC7940M.h
 * Class for interfacing with the MPC7940M Real-Time Clock
 *
 * Created: 08/17/18 11:22:52
 * Author: MrHallows
 *
 *
 * Pinout:
 *
 *		 X1 +--u--+ Vcc
 *		 X2 +     + MFP
 *		 NC +     + SCL
 *		Vss +-----+ SDA
 *
 * Wiring:
 *
 * 		MPC7940M     Arduino
 * 		--------------------
 * 		X1...........GND
 * 		X2...........GND
 * 		NC...........GND
 * 		Vss..........GND
 * 		SDA..........A4
 * 		SCL..........A5
 * 		MFP..........GND (Until Write Protection is necessary)
 * 		Vcc..........5V
 */
 #pragma once

#ifndef __RTC_MPC7940M_H__
#define __RTC_MPC7940M_H__


// RTC Pins (I2C)
#define SCL_PIN 						13 // D0
#define SDA_PIN 						11 // D1


class RTC_MPC7940M
{
public:
protected:
private:

// Functions
public:
	RTC_MPC7940M();
	~RTC_MPC7940M();
protected:
private:
	RTC_MPC7940M(const RTC_MPC7940M &c);
	RTC_MPC7940M& operator=(const RTC_MPC7940M &c);

}; //RTC_MPC7940M

#endif /* defined("__RTC_MPC7940M_H__") */