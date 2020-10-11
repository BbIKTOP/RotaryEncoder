/*
 * Encoder.h
 *
 *  Created on: Mar 20, 2019
 *      Author: viktor
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <Arduino.h>

#include <Ticker.h>

#include "EncoderEvent.h"
#include "Fifo.h"

#ifndef MAX_ENCODERS
#define MAX_ENCODERS 20
#endif

class Encoder
{
private:
	long currentValue = 0;
	long minValue = 0;
	bool minValueSet = 0;
	long maxValue = 0;
	bool maxValueSet = 0;

	bool rotateOver = 0;

	int pinL, pinR, pinB;
	int lastState = 0x11;
	int lastDirection = 0;

	int lastButtonState = 0;
	unsigned long lastButtonPressTime = 0;
	bool pressed = false;
	bool rotated = false;

	Ticker pressTicker;

	long longPressDuration = 1000;
	long pressBias = 20;

	static Encoder *encoders[MAX_ENCODERS];
	static int encodersCount;
	static void ICACHE_RAM_ATTR interruptHandler(void);
	static void tickerHandler(int);

	static volatile int inInterruptFlag;

	bool interruptAttached;

	Fifo<EncoderEvent> eventBuffer;

public:
	Encoder(int pinL, int pinR, int pinB);
	Encoder(int pinL, int pinR);
	~Encoder();

	void begin(void);
	void beginNoInterrupt(void);
	void end(void);

	bool isInterruptAttached();

	int getPinL(void);
	int getPinR(void);
	int getPinB(void);

	long setLongPressDuration(long);
	long getlongPressDuration(void);

	long setPressBias(long);
	long getPressBias(void);

	long getValue(void);
	long setValue(long);

	bool isPressed(void);

	long setMinValue(long);
	long getMinValue(void);
	bool isMinValue();
	void removeMinValue();

	long setMaxValue(long);
	long getMaxValue(void);
	bool isMaxValue();
	void removeMaxValue();

	bool setRotateOver(bool);
	bool isRotateOver(void);

	static void iteration(void);
	static void poll(void);

	int getEventsCount(void);
	bool hasEvent(void);

	void clear(void);

	EncoderEvent getEvent(void);
};

#endif /* ENCODER_H_ */
