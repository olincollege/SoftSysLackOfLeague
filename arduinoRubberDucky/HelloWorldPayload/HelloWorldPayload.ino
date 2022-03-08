// hello world rubber ducky program
// runs on the atemega328p
char string[] = "Hello World!";

void setup() {
  // serial to atmega16u (for USB)
  Serial.begin(115200); 
}

void loop() {
  // print the string character by character
  for (int i = 0; i < strlen(string); i++) {
    Serial.write(string[i]);
    delay(200);
  }

  while (true) {}; // don't type anything else for now
}
