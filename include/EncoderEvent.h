/*
 * EncoderEvent.h
 *
 *  Created on: Mar 20, 2019
 *      Author: viktor
 */

#ifndef ENCODEREVENT_H_
#define ENCODEREVENT_H_

#include <Arduino.h>

#include "Encoder.h"

class Encoder;

class EncoderEvent
{
public:
	enum EventName
	{
		ROTATELEFT,
		ROTATERIGHT,
		PRESS,
		LONGPRESS,
		RELEASE,
		NONE
	};

protected:
	Encoder *encoder;
	EventName event;

	int currentValue;
	bool pressed;
	bool rotated;
	int holdTime;
	int direction;

public:
	EncoderEvent();
	EncoderEvent(Encoder *encoder, EventName event, int currentValue, bool pressed, int holdTime, bool rotated, int direction);

	int getCurrentValue(void);
	void setCurrentValue(int currentValue);

	int getDirection(void);
	void setDirection(int currentValue);

	Encoder *getEncoder(void);
	void setEncoder(Encoder *encoder);

	EventName getEvent(void);
	void setEvent(EventName event);

	int getHoldTime(void);
	void setHoldTime(int holdTime);

	bool isPressed(void);
	void setPressed(bool rotatedWhilePressed);

	bool isRotated(void);
	void setRotated(bool rotated);

	String toString(void);
	operator String();
};

#endif /* ENCODEREVENT_H_ */
