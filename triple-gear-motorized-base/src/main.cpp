#include <Arduino.h>
#include "main.h"


#define SWITCH_PIN 8 // active low
#define POT_PIN A6 // 4th pin, analog 6
#define MOTOR_PIN 10 // 10th pin, D10/A10
#define POT_DEADZONE 10 // values less than this will be the sam as 0

bool enabled = false;

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  analogWrite(MOTOR_PIN, 0);
}

void loop() {
  bool switchValue = !digitalRead(SWITCH_PIN); // pullup when off
  if (switchValue && !enabled)
  {
    // debounce
    delay(10);
    bool switchValue = !digitalRead(SWITCH_PIN);
    if (switchValue)
    {
      onEnabled();
    }
  }
  else if (!switchValue && enabled)
  {
    // debounce
    delay(10);
    bool switchValue = !digitalRead(SWITCH_PIN);
    if (!switchValue)
    {
      onDisabled();
    }
  }

  if (enabled)
  {
    whileEnabled();
  }
}

void onEnabled()
{
  enabled = true;
}

void onDisabled()
{
  enabled = false;
  analogWrite(MOTOR_PIN, 0);
}

void whileEnabled()
{
  uint16_t potValue = analogRead(POT_PIN);
  if (potValue < POT_DEADZONE)
  {
    analogWrite(MOTOR_PIN, 0);
  }
  else
  {
    int motorValue = map(potValue, POT_DEADZONE, 1023, 18, 127);
    analogWrite(MOTOR_PIN, motorValue);
  }
}
