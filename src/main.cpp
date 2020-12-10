#include <Arduino.h>

#include "Encoder.h"

#define TONE_PIN 15

Encoder encoder(14, 12, 13);
void setup()
{
  Serial.begin(115200);
  Serial.println("\n\nStarting");

  encoder.setMinValue(0);
  encoder.setMaxValue(30000);
  encoder.setRotateOver(false);
  encoder.begin();

  tone(TONE_PIN, (int)encoder.getValue());
  Serial.println("Started, curent value=" + String(encoder.getValue()));
}

void loop()
{

  int freq = 1;
  static bool on = true;
  static int step = 1;

  if (encoder.hasEvent())
  {
    EncoderEvent event = encoder.getEvent();

    // Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Got event: " + event);

    switch (event.getEvent())
    {
    case EncoderEvent::LONGPRESS:
      freq = 1;
      tone(TONE_PIN, freq);
      Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Frequency reset to " + freq + " Hz");
      break;
    case EncoderEvent::PRESS:
      on = !on;
      if (on)
      {
        freq = event.getCurrentValue();
        tone(TONE_PIN, freq);
        Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Sound on. Frequency=" + freq + " Hz");
        encoder.setRotationSuspended(false);
      }
      else
      {
        noTone(TONE_PIN);
        encoder.setRotationSuspended(true);
        Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Sound off.");
      }
      break;
    case EncoderEvent::ROTATERIGHT:
      if (event.isPressed())
      {
        step++;
        if (step > 100)
          step = 100;
        encoder.setStep(step);
        Serial.println(String(round((double)(millis() / 10)) / 100) + " s : New step set to " + step);
      }
      else
      {
        {
          freq = event.getCurrentValue();
          tone(TONE_PIN, freq);

          Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Frequency=" + freq + " Hz");
        }
      }
      break;
    case EncoderEvent::ROTATELEFT:
      if (event.isPressed())
      {
        step--;
        if (step < 1)
          step = 1;
        encoder.setStep(step);
        Serial.println(String(round((double)(millis() / 10)) / 100) + " s : New step set to " + step);
      }
      else
      {
        {
          freq = event.getCurrentValue();
          tone(TONE_PIN, freq);

          Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Frequency=" + freq + " Hz");
        }
      }
      break;
    default:
      break;
    }
  }
  else
  {
    delay(1);
  }

  delay(0);
}

/* 
    Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Got event: " + event);

    if (event.getEvent() == EncoderEvent::LONGPRESS && !event.isRotated())
    {
      encoder.setRotateOver(!encoder.isRotateOver());
      Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Rotate Over now set to " + String(encoder.isRotateOver()));
    }
    if (event.getEvent() == EncoderEvent::ROTATERIGHT && event.isPressed())
    {
      encoder.setMaxValue(encoder.getMaxValue() + 1);
      Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Max value now set to " + String(encoder.getMaxValue()));
    }
    if (event.getEvent() == EncoderEvent::ROTATELEFT && event.isPressed())
    {
      encoder.setMaxValue(encoder.getMaxValue() - 1);
      Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Max value now set to " + String(encoder.getMaxValue()));
    }
 */