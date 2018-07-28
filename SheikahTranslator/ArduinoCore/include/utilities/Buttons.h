#pragma once

#ifndef BUTTONS_H
#define	BUTTONS_H

//#include <Arduino.h>
//#include "settings.c"


class Buttons
{
public:
    void begin();
    void update();

    bool pressed(unsigned char button);
    bool released(unsigned char button);
    bool held(unsigned char button, unsigned char time);
    bool repeat(unsigned char button, unsigned char period);

    unsigned char timeHeld(unsigned char button);
    unsigned char pins[NUM_BTN];
    unsigned char states[NUM_BTN];
};


#endif /* defined("BUTTONS_H") */