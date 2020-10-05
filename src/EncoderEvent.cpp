/*
 * EncoderEvent.cpp
 *
 *  Created on: Mar 20, 2019
 *      Author: viktor
 */

#include "Encoder.h"
#include "EncoderEvent.h"

EncoderEvent::EncoderEvent() : EncoderEvent(nullptr, EventName::NONE, 0, 0, 0)
{
}

EncoderEvent::EncoderEvent(Encoder *encoder, EventName event, int currentValue, bool pressed, int holdTime)
{
    this->encoder = encoder;
    this->event = event;
    this->currentValue = currentValue;
    this->holdTime = holdTime;
    this->pressed = pressed;
}

int EncoderEvent::getCurrentValue()
{
    return (currentValue);
}

void EncoderEvent::setCurrentValue(int currentValue)
{
    this->currentValue = currentValue;
}

Encoder *EncoderEvent::getEncoder()
{
    return (encoder);
}

void EncoderEvent::setEncoder(Encoder *encoder)
{
    this->encoder = encoder;
}

EncoderEvent::EventName EncoderEvent::getEvent()
{
    return (event);
}

void EncoderEvent::setEvent(EncoderEvent::EventName event)
{
    this->event = event;
}

int EncoderEvent::getHoldTime()
{
    return (holdTime);
}

void EncoderEvent::setHoldTime(int holdTime)
{
    this->holdTime = holdTime;
}

bool EncoderEvent::isPressed()
{
    return (pressed);
}

void EncoderEvent::setPressed(bool pressed)
{
    this->pressed = pressed;
}

EncoderEvent::operator String()
{
    return (toString());
}

String EncoderEvent::toString(void)
{
    String res = "";

    switch (event)
    {
    case ROTATELEFT:
        res += "ROTATELEFT";
        break;
    case ROTATERIGHT:
        res += "ROTATERIGHT";
        break;
    case PRESS:
        res += "PRESS";
        break;
    case LONGPRESS:
        res += "LONGPRESS";
        break;
    case RELEASE:
        res += "RELEASE";
        break;
    case NONE:
        res += "NONE";
        break;
    default:
        res += "<UNKNOWN>";
        break;
    }

    res += " event\n";

    res += "current value=" + String(getCurrentValue()) + "\n";
    res += "pressed=" + String(isPressed()) + "\n";
    res += "holdTime=" + String(getHoldTime()) + "\n";

    return (res);
}
