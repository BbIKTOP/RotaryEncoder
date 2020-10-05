/*
 * Encoder.ino
 *
 *  Created on: Mar 20, 2019
 *      Author: viktor
 */
#include "encoder.h"

int Encoder::encodersCount = 0;
Encoder *Encoder::encoders[MAX_ENCODERS];
volatile int Encoder::inInterruptFlag = 0;

Encoder::Encoder(int pinL, int pinR, int pinB) : eventBuffer()
{
	if (encodersCount >= MAX_ENCODERS)
	{
		Serial.println("Cannot add more encoders");
		return;
	}

	int currentEncoder;
	for (currentEncoder = 0; currentEncoder < encodersCount; currentEncoder++)
	{
		/*
		 if (  encoders[currentEncoder]->getPinL() == pinL &&
		 encoders[currentEncoder]->getPinR() == pinR &&
		 encoders[currentEncoder]->getPinB() == pinB) break;
		 */
		// Use either upper or lower variant
		if (encoders[currentEncoder]->getPinL() == pinL || encoders[currentEncoder]->getPinR() == pinR || encoders[currentEncoder]->getPinB() == pinB)
			break;
	}

	encoders[currentEncoder] = this;
	encodersCount++;

	this->pinL = pinL;
	this->pinR = pinR;
	this->pinB = pinB;

	interruptAttached = false;
}

Encoder::Encoder(int pinL, int pinR) : Encoder(pinL, pinR, -1)
{
}
Encoder::~Encoder()
{
}

void Encoder::beginNoInterrupt(void)
{
	pinMode(pinL, INPUT_PULLUP);
	pinMode(pinR, INPUT_PULLUP);

	digitalWrite(pinL, HIGH);
	digitalWrite(pinR, HIGH);

	if (pinB >= 0)
	{
		pinMode(pinB, INPUT_PULLUP);
		digitalWrite(pinB, HIGH);
	}
}
void Encoder::begin(void)
{
	beginNoInterrupt();
	attachInterrupt(digitalPinToInterrupt(pinL), interruptHandler, CHANGE);
	attachInterrupt(digitalPinToInterrupt(pinR), interruptHandler, CHANGE);

	if (pinB >= 0)
	{
		attachInterrupt(digitalPinToInterrupt(pinB), interruptHandler, CHANGE);
	}

	interruptAttached = true;
}
void Encoder::end(void)
{
	if (isInterruptAttached())
	{
		detachInterrupt(digitalPinToInterrupt(pinL));
		detachInterrupt(digitalPinToInterrupt(pinR));

		if (pinB >= 0)
		{
			detachInterrupt(digitalPinToInterrupt(pinB));
		}
		interruptAttached = false;
	}
}

bool Encoder::isInterruptAttached()
{
	return (interruptAttached);
}

int Encoder::getPinL(void)
{
	return (pinL);
}
int Encoder::getPinR(void)
{
	return (pinR);
}
int Encoder::getPinB(void)
{
	return (pinB);
}

bool Encoder::isPressed(void)
{
	return (pressed);
}

int Encoder::getEventsCount(void)
{
	return (eventBuffer.length());
}

bool Encoder::hasEvent(void)
{
	return (eventBuffer.length() > 0);
}

EncoderEvent Encoder::getEvent(void)
{
	return (eventBuffer.get());
}

long Encoder::setLongPressDuration(long lpd)
{
	long old = longPressDuration;
	longPressDuration = lpd;
	return (old);
}
long Encoder::getlongPressDuration(void)
{
	return (longPressDuration);
}

long Encoder::setPressBias(long pb)
{
	long old = pressBias;
	pressBias = pb;
	return (old);
}
long Encoder::getPressBias(void)
{
	return (pressBias);
}

long Encoder::getValue(void)
{
	return (currentValue);
}
long Encoder::setValue(long cv)
{
	long old = currentValue;
	currentValue = cv;
	return (old);
}

long Encoder::setMinValue(long mv)
{
	long old = getMinValue();
	minValue = mv;
	minValueSet = true;

	if (getValue() < getMinValue())
	{
		setValue(getMinValue());
	}
	return (old);
}
long Encoder::getMinValue(void)
{
	return (minValue);
}
bool Encoder::isMinValue()
{
	return (minValueSet);
}
void Encoder::removeMinValue()
{
	minValueSet = false;
	minValue = 0;
}

long Encoder::setMaxValue(long mv)
{
	long old = getMaxValue();
	maxValue = mv;
	maxValueSet = true;

	if (getValue() > getMaxValue())
	{
		setValue(getMaxValue());
	}

	return (old);
}
long Encoder::getMaxValue(void)
{
	return (maxValue);
}
bool Encoder::isMaxValue()
{
	return (maxValueSet);
}
void Encoder::removeMaxValue()
{
	maxValueSet = false;
	maxValue = 0;
}

bool Encoder::setRotateOver(bool v)
{
	bool old = rotateOver;
	rotateOver = v;
	return (old);
}

bool Encoder::isRotateOver(void)
{
	return (rotateOver);
}

void ICACHE_RAM_ATTR Encoder::interruptHandler(void)
{
	if (inInterruptFlag > 0)
	{
		Serial.println("!!!!----!!!!----interrupt handler is already called!!!!!");
		return;
	}
	inInterruptFlag++;
	for (int i = 0; i < Encoder::encodersCount; i++) // Process all attached encoders
	{
		Encoder &encoder = *encoders[i];

		if (!encoder.isInterruptAttached())
		{
			continue; // This encoder does not use interrupts
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Process button first
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (encoder.pinB >= 0) // Is button control enabled for this encoder?
		{
			int buttonState = digitalRead(encoder.pinB);
			if (buttonState == 0 && encoder.lastButtonState == 0) // Button pressed
			{
				encoder.lastButtonPressTime = millis();
				encoder.lastButtonState = 1;
				encoder.pressed = false;

				// Start timer for this button to check when it will be held long enough to toggle press event
				encoder.pressTicker.once_ms<int>(encoder.pressBias,
												 tickerHandler, i);
				/*
				 if (e.pressHandler != NULL)
				 {
				 e.pressHandler(e);
				 }
				 */
			}

			if (buttonState == 1 && encoder.lastButtonState == 2) // Button released and has been pressed
			{
				long buttonHeldTime = abs(
					millis() - encoder.lastButtonPressTime);

				// Button released
				if (buttonHeldTime > encoder.longPressDuration) // Long press occurred
				{
					EncoderEvent event(&encoder, EncoderEvent::LONGPRESS, encoder.getValue(),
									   true, buttonHeldTime - encoder.pressBias, encoder.rotated);
					encoder.eventBuffer.put(event);
				}
				//				else // Generate release for long press?
				{
					// Usual press occurred, generate release event
					EncoderEvent event(&encoder, EncoderEvent::RELEASE, encoder.getValue(),
									   true, buttonHeldTime - encoder.pressBias, encoder.rotated);
					encoder.eventBuffer.put(event);
					encoder.rotated = false;
					encoder.pressed = false;
				}
			}
			if (buttonState == 1)
			{
				encoder.lastButtonState = 0;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Process rotation
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int currentState = digitalRead(encoder.pinL) * 0x10 + digitalRead(encoder.pinR);

		if ((currentState == 0x10 || currentState == 0x01) && encoder.lastButtonState > 0)
		{
			encoder.pressed = true;
			tickerHandler(i); // Has been rotated while pressed, call press handler immediately
		}

		if (encoder.lastState == currentState || currentState == 0) // Nothing happens or no rotation
		{
			break;
		}
		if (currentState == 0x11) // Final state reached, fire event
		{
			if (encoder.lastDirection != 0)
			{
				encoder.currentValue += encoder.lastDirection;

				if (encoder.minValueSet && encoder.currentValue < encoder.minValue)
				{
					if (encoder.rotateOver)
						encoder.currentValue = encoder.maxValue;
					else
						encoder.currentValue = encoder.minValue;
				}
				//e.currentValue = e.minValueSet && e.currentValue < e.minValue ? e.minValue : e.currentValue;
				if (encoder.maxValueSet && encoder.currentValue > encoder.maxValue)
				{
					if (encoder.rotateOver)
						encoder.currentValue = encoder.minValue;
					else
						encoder.currentValue = encoder.maxValue;
				}
				//e.currentValue = e.maxValueSet && e.currentValue > e.maxValue ? e.maxValue : e.currentValue;

				// Does not make sence really
				long buttonHeldTime = abs(millis() - encoder.lastButtonPressTime);
				if (!encoder.isPressed())
					buttonHeldTime = 0;

				EncoderEvent::EventName rotationEvent = EncoderEvent::EventName::ROTATERIGHT;
				if (encoder.lastDirection < 0)
					rotationEvent = EncoderEvent::EventName::ROTATELEFT;
				encoder.rotated = true;
				EncoderEvent event(&encoder, rotationEvent,
								   encoder.getValue(), encoder.lastButtonState != 0, buttonHeldTime, encoder.rotated);
				encoder.eventBuffer.put(event);
			}

			encoder.lastState = currentState;
			encoder.lastDirection = 0;
			break;
		}

		if (currentState == 0x01 && encoder.lastState == 0x10)
		{
			encoder.lastDirection = -1;
		}
		if (currentState == 0x10 && encoder.lastState == 0x01)
		{
			encoder.lastDirection = 1;
		}
		encoder.lastState = currentState;
	} // for (encoder in encoders) loop

	inInterruptFlag--;
}

void Encoder::tickerHandler(int num)
{
	Encoder &encoder = *encoders[num];

	if (encoder.pinB >= 0) // Is button control enabled for this encoder?
	{
		int buttonState = digitalRead(encoder.pinB);

		if (buttonState == 0 && encoder.lastButtonState == 1) // Button was pressed and still pressed
		{
			encoder.lastButtonState = 2;

			// Serial.println("New PRESS event created");
			EncoderEvent event(&encoder, EncoderEvent::PRESS, encoder.getValue(),
							   false, 0, false);
			encoder.rotated = false;
			// Serial.println("New PRESS event created:" + event.toString());
			encoder.eventBuffer.put(event);
		}
	}
}

void Encoder::iteration(void) // Call this in main loop for non-interrupt mode
{
	Encoder::interruptHandler();
}

void Encoder::poll(void) // Call this in main loop for non-interrupt mode
{
	Encoder::interruptHandler();
}
