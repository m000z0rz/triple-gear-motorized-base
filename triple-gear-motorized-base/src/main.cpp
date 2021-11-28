#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "main.h"

#define LED_RING_PIN 19
#define LED_RING_COUNT 16

#define SWITCH_PIN 20 // active low
#define POT_PIN 9 // Analog 9
#define MOTOR_PIN 10

bool enabled = false;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel ledRing(LED_RING_COUNT, LED_RING_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ledRing.begin();
  ledRing.show();

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  analogWrite(MOTOR_PIN, 0);

  // put your setup code here, to run once:
}

void loop() {
  bool switchValue = digitalRead(SWITCH_PIN);
  if (!switchValue && !enabled)
  {
    // enabling?
    delay(10);
    if (!switchValue)
    {
      onEnabled();
    }
  }
  else if (switchValue && enabled)
  {
    // disabling?
    delay(10);
    if (switchValue)
    {
      onDisabled();
    }
  }

  if (enabled)
  {
    whileEnabled();
  }
  // put your main code here, to run repeatedly:
}

void onEnabled()
{
  enabled = true;

}

void onDisabled()
{
  enabled = false;
  ledRing.clear();
  ledRing.show();
}

void whileEnabled()
{
  int potValue = analogRead(POT_PIN);
  analogWrite(MOTOR_PIN, map(potValue, 0, 1023, 0, 1023));
  
  uint16_t hue = 65536L * 3/4; // halfway between blue and magenta (purple) 
  uint32_t purple = ledRing.gamma32(ledRing.ColorHSV(hue, 255, 128));
  //theaterChase(purple, 60);
  theaterChaseRainbow(60);
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      ledRing.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<ledRing.numPixels(); c += 3) {
        ledRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      ledRing.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      ledRing.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<ledRing.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / ledRing.numPixels();
        uint32_t color = ledRing.gamma32(ledRing.ColorHSV(hue)); // hue -> RGB
        ledRing.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      ledRing.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}