#include "MIDIUSB.h"

const int LED_PIN = 9;

bool led = LOW;
bool receiveSysEx = false;

void hi() {
  midiEventPacket_t sysexA = {0x04, 0xF0, 0x69, 0x23};
  MidiUSB.sendMIDI(sysexA);
  midiEventPacket_t sysexB = {0x05, 0xF7, 0x00, 0x00};
  MidiUSB.sendMIDI(sysexB);

  MidiUSB.flush();

  Serial.println("hey :)");
}

void bye() {
  midiEventPacket_t sysexA = {0x04, 0xF0, 0x69, 0x42};
  MidiUSB.sendMIDI(sysexA);
  midiEventPacket_t sysexB = {0x05, 0xF7, 0x00, 0x00};
  MidiUSB.sendMIDI(sysexB);

  MidiUSB.flush();

  Serial.println("bye!");
}

void printRx(midiEventPacket_t rx) {
  Serial.print("Received: ");
  Serial.print(rx.header, HEX);
  Serial.print("-");
  Serial.print(rx.byte1, HEX);
  Serial.print("-");
  Serial.print(rx.byte2, HEX);
  Serial.print("-");
  Serial.println(rx.byte3, HEX);
}

bool handleSysEx(midiEventPacket_t rx) {
  if (rx.header == 0x05) {
    //Serial.println("SYSEX RECEIVED");
    // TODO potential data loss; we only use one byte of the first message for now.
    return true;
  }

  if (rx.byte3 == 0x00) {
    //led = LOW;
  }
  
  if (rx.byte3 == 0x01) {
    //led = HIGH;
  }

  if (rx.byte3 == 0x23) {
    hi();
    led = HIGH;
  }

  if (rx.byte3 == 0x42) {
    led = LOW;
    bye();
  }

  printRx(rx);

  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, led);

  midiEventPacket_t rx;

  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      // printRx(rx);
    }

    if (rx.header == 0x04 || rx.header == 0x05) {
      handleSysEx(rx);
    }
  } while (rx.header != 0);
}