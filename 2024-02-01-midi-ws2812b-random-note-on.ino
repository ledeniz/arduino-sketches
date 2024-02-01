// triggers a random LED fade on a WS2812B strip when receving a NOTE ON midi message 

#include "Timer.h"
#include "MIDIUSB.h"
#include "FastLED.h"

#define NUM_LEDS  20
#define DATA_PIN  6
#define INTERVAL  10
#define DARKEST   50
#define BRIGHTEST 100

Timer timer(MILLIS);
CRGB leds[NUM_LEDS];

void setup() {
  delay(3000); // allows reprogramming
  
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.show();

  timer.start();
  
  Serial.begin(9600);
  Serial.println("START");
}

void loop() {
  receiveMidi();
  fadeLeds();
}

void receiveMidi() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();

    if (rx.byte1 == 0x90) {
      leds[random(0, NUM_LEDS)] = CRGB(
        random(DARKEST, BRIGHTEST),
        random(DARKEST, BRIGHTEST),
        random(DARKEST, BRIGHTEST)
      );
    }
  } while (rx.header != 0);
}

void fadeLeds() {
  if (timer.read() < INTERVAL) { return; }
  timer.stop();
  timer.start();

  for (int i=0; i<NUM_LEDS; i++) {
    unsigned short r = leds[i].r;
    unsigned short g = leds[i].g;
    unsigned short b = leds[i].b;

    if (r > 0) { r--; }
    if (g > 0) { g--; }
    if (b > 0) { b--; }

    leds[i] = CRGB(r, g, b);
  }

  FastLED.show();
}
