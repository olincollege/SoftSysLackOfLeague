#include "Keyboard.h"
#include "HID.h"

// runs on the atemega16u
void setup() {
  // serial 1 is incoming from the atmega328p
  Serial1.begin(115200);
  // use the HID library to appear as an HID device
  Keyboard.begin();
  // serial will show up on serial monitor like normal
  Serial.begin(115200);
}

void loop() {
  // wait for bytes from atmega328p
  if (Serial1.available()) {
    // get a byte
    char c = Serial1.read();
    // print for debugging
    Serial.print("USB: ");
    Serial.print(c);
    Serial.println();
    // send a fake keypress that matches the character
    Keyboard.print(c);
  }
}
