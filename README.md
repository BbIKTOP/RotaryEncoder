# RotaryEncoder
RotaryEncoder ESP Arduino library

## Purpose
To utilize rotary encoder

## Architecture
Encoders generate events. Generated events are stored in the circular buffer. Buffer size can be set by CIRCULAR_FIFO_BUFFER_SIZE (default is 100). Stored events can be retrieved from encoder and processed by the application.
Encoder has an integer value, associated with it. This value can be changed by rotating the encoder. Encoder's value, unlimited by default, can be limited by min and max boundaries. In case these boundaries are set, encoder could be configured to "rotate over", when limit is reached. In this case, continuous rotation will make encoder's value to be set to the opposite value. For example, let's suppose boundaries set to min=3, max=10. In case current value is 3 and "rotate over" set to true, Rotating encoder counterclockwise will set its value to 10 and vice versa.
Current value can be obtained by calling getValue method.
Two usage models are available: using ISR and using poll in the main loop()
Each event is stored in the EncoderEvent object. 

## Encoder Usage

To create an object, use: Encoder::Encoder(int pinL, int pinR, int pinB)
Where:
pinL: pin number connected to the encoder's "Rotate Counterclockwise" pin
pinR: pin number connected to the encoder's "Rotate Clockwise" pin
pinB: pin number connected to the encoder's button pin (optional, button presses would not be processed if omitted)

To start using encoder in ISR mode, use void Encoder::begin(void)

To start using encoder in the poll mode, use void Encoder::beginNoInterrupt(void)
In this case, calling void Encoder::poll(void) will be required in loop() in order to detect and collect events

To stop using encoder, use void Encoder::end(void)

To check for waiting events, use bool Encoder::hasEvent(void)

To get the number of the events currently available in the buffer, use int Encoder::getEventsCount(void)

To retrieve event from buffer, use EncoderEvent Encoder::getEvent(void)
This method will throw an std::underflow_error in case there's no events available. The EncoderEvent object returned, will contain all the information regarding gathered event.

To get current value, use long Encoder::getValue(void)

To prevent jitter, debouncing is applied to the button's events by setting a short delay on event processing. This delay can be changed using long Encoder::setPressBias(long pb). Defaut value is 20ms

long button press generates LONGPRESS event. The threshold time to fire LONGPRESS event can be set by long Encoder::setLongPressDuration(long lpd)
Where lpd is the long press duration threshold, set in millis

To set, get and clear min and max values, use:
long Encoder::setMinValue(long mv)
long Encoder::getMinValue(void)
bool Encoder::isMinValue()
void Encoder::removeMinValue()
long Encoder::setMaxValue(long mv)
long Encoder::getMaxValue(void)
bool Encoder::isMaxValue()
void Encoder::removeMaxValue()

To control "Rotate Over" behaviour, use:
bool Encoder::setRotateOver(bool v)
bool Encoder::isRotateOver(void)

## EncoderEvent Usage

EncoderEvent object holds all the information about collected event. 5 events can be generated by the encoder:
* ROTATELEFT - encoder has been rotated counterclockwise
* ROTATERIGHT - encoder has been rotated clockwise
* PRESS - encoder button has been pressed
* LONGPRESS - encoder button has been pressed and held for a time, equal or greater than LONGPRESS threshold
* RELEASE - encoder button has been released

The event can be obtained by calling the EncoderEvent::EventName EncoderEvent::getEvent()

Two additional flags are available for each event:
* bool EncoderEvent::isPressed() indicates, has encoder's button been pressed during event
* bool EncoderEvent::isRotated() indicates, has encoder been rotated during event

## Usage example

```c++
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
  delay(0);
}
```

