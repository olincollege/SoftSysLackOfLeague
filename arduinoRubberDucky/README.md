# Arduino Rubber Ducky

While there are projects that allow an Arduino to operate as a full "Rubber Ducky" clone and use ducky scripts, we have not attempted to incorporate that functionality. We are using a more bare-metal approach writing firmware for the two microcontrollers.

## Installation

You will need two Arduino Unos (or clones) and a few jumper cables. The official instruction have another methods, but we haven't tested them.

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) if you don't have it.

1. Program the first Arduino with the `BottloaderInstallation` sketch.

1. Disconnect the Arduino and wire it up as shown here: ![wiring digram for flashing 16u](https://github.com/NicoHood/HoodLoader2/wiki/pictures/ArduinoISP.jpg). This connects the programming header from the Atmega328p on the first board to the programming header on the Atemega16u of the second board.

1. Plug the Arduino in. Wait about 30 seconds and the LED should begin blinking rapidly indicating a successful installation.

1. Under `Files > Preferences > Additional Board Manager URLs` in the Arduino IDE add `https://raw.githubusercontent.com/NicoHood/HoodLoader2/master/package_NicoHood_HoodLoader2_index.json`.

1. Under the board manager install the HoodLoader2 boards.

1. Download and extract the latest release of the [HID library](https://github.com/NicoHood/HID/releases/tag/2.8.3).

1. Copy the library into the Arduino libraries folder (`$/Arduino/libraries` on linux).

1. Set the port for your board in the IDE and select the `HoodLoader2 16u`. Upload the RubberDucky sketch.

## Uploading a Payload

Now we can write Arduino programs and anything they print will be typed for us. Programs should be uploaded with the board set to `HoodLoader2 Uno`. Putting the payload on the 328p allows it to interact with GPIO and access much more memory.