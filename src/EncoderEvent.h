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
	int holdTime;

public:
	EncoderEvent();
	EncoderEvent(Encoder *encoder, EventName event, int currentValue, bool pressed, int holdTime);

	int getCurrentValue();
	void setCurrentValue(int currentValue);

	Encoder *getEncoder();
	void setEncoder(Encoder *encoder);

	EventName getEvent();
	void setEvent(EventName event);

	int getHoldTime();
	void setHoldTime(int holdTime);

	bool isPressed();
	void setPressed(bool rotatedWhilePressed);

	String toString(void);
	operator String();
};

#endif /* ENCODEREVENT_H_ */
