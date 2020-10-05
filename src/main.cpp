#include <Arduino.h>

#include "Encoder.h"

Encoder encoder(12, 14, 13);
void setup()
{
  Serial.begin(115200);
  Serial.println("\n\nStarting");

  encoder.setMinValue(0);
  encoder.setMaxValue(10);
  encoder.setRotateOver(true);
  encoder.begin();

  Serial.println("Started");
}

void loop()
{

  if (encoder.hasEvent())
  {
    EncoderEvent event = encoder.getEvent();
    Serial.println(String(round((double)(millis() / 10)) / 100) + " s : Got event: " + event.toString());
  }
  delay(0);
}


