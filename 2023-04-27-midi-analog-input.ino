#include <Arduino.h>
#include "MIDIUSB.h"

const int INPUT_PIN = A0;
const int LED_PIN = 3;

const byte MIDI_CHANNEL = 0;
const byte MIDI_CC = 74;

int sensor_value = 0;
int old_sensor_value = 0; 

int t = 0;
int old_t = 0;

const int numReadings = 10;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

void readSensor() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(INPUT_PIN);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  sensor_value = map(average, 0, 1023, 127, 0);
}

void sendValue(int value) {
  //Serial.println(value);

  byte channel = MIDI_CHANNEL;
  byte control = MIDI_CC;

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, (byte) value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  readSensor();
  old_sensor_value = sensor_value;
}

void loop() {
  t = millis();
  int delta = t - old_t;

  readSensor();

  if (sensor_value != old_sensor_value) {
    sendValue(sensor_value);
    old_sensor_value = sensor_value;

    int led_value = map(sensor_value, 0, 127, 0, 255);
    analogWrite(LED_PIN, led_value);
  }
}

