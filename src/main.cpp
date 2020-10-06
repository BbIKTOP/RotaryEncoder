#include <Arduino.h>

#include "Encoder.h"

Encoder encoder(12, 14, 13);
void setup()
{
  Serial.begin(115200);
  Serial.println("\n\nStarting");

  encoder.setMinValue(1);
  encoder.setMaxValue(10);
  encoder.setRotateOver(false);
  encoder.begin();

  Serial.println("Started, curent value=" + String(encoder.getValue()));
}

void loop()
{

  if (encoder.hasEvent())
  {
    EncoderEvent event = encoder.getEvent();
    Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Got event: " + event);

    if (event.getEvent() == EncoderEvent::LONGPRESS && !event.isRotated())
    {
      encoder.setRotateOver(!encoder.isRotateOver());
      Serial.println("\nRotate Over now set to " + String(encoder.isRotateOver()) + "\n");
    }
  }
  else
  {
    delay(1);
  }

  delay(0);
}
