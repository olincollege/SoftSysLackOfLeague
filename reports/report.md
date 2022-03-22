# Lack of League

> Andrew DeCandia, Melissa Kazazic, Elvis Wolcott

## Goal
Our goal is to prepare a flash drive with a program written in C that is able to prevent the target device from opening League of Legends.

## Learning Goals

### Andrew
I want to be able to create a Windows application that interacts with the operating system. I would like to understand how to control Windows automatically to improve my daily use of the operating system after this project. 

### Melissa
Learn enough to be comfortable approaching a similar project again on my own. I’d like to understand how operating systems work at a lower level.

### Elvis
I’m interested in exploring the space of security research in general. I have never done anything similar, so I hope to learn more about how different programs and elements of the operating system interact and how they can be manipulated.
I have realized there is a lot I am excited about on the firmware side with the rubber ducky to explore now that I understand how the different chips and HID interact. I would like to dig deeper into the HID library and/or more complex firmware features in the second phase of the project. I hope to learn enough that I could design a more compact PCB around the same chips and write the firmware from scratch myself in the future.


## Results
We knew that we would need to run one of our programs either on the machine running windows or by booting into a live environment running linux. While it would be possible to do this by manually typing a bunch of commands into a terminal, we worried it would be error prone, slower, and risk detection. Even worse, some league players take their mechanical keyboards or fancy mice with them!
### Rubber Ducky
We built a device that is able to emulate a USB keyboard using an Arduino Uno because they are cheap and readily accessible. The Arduino Uno contains two chips: the Atmega328p which runs your code and the Atmega16u which acts as a serial-to-USB converter so that you can program and send data to the Atmega328p. Normally, the Atmega16u cannot be reprogrammed, so the Arduino’s USB functionality cannot be changed. However, replacing the bootloader on the Atmega16u with [Hood Loader 2](https://github.com/NicoHood/HoodLoader2) makes it possible to program both chips. The process is easiest using a second Arduino Uno to flash the bootloader and we have detailed it in the [rubber ducky README](../arduinoRubberDucky/README.md). 
Unfortunately, theAtemega16u has only 512 bytes of memory, which isn’t even enough to hold the strings for all the commands. Thankfully, the Atmega328p has much more memory, and is connected to the Atmega16u over serial. Normally, the Atmega16u takes data from serial and prints it back out over USB. However, our firmware can instead send keypresses so that the text printed by the main chip appears to be typed on the connected computer.
```c
if (Serial1.available()) {
    // mirror out bytes from serial (incoming from main chip with more memory)
    // get a byte
    char c = Serial1.read();
    // send a fake keypress that matches the character
    Keyboard.print(c);
  }
```
Unfortunately, a simple `Serial.println` will flood the Atmega16u’s serial buffer and result in lost characters. We were able to fix this (and give a nice typing effect for demos) by adding in a small delay between sending bytes.
```c
void send(char* string) {
  for (int i = 0; i < strlen(string); i++) {
    Serial.write(string[i]);
    delay(10); // delay a tiny bit so that the 16u doesn't get flooded
  }
  Serial.write('\n');
  delay(100);
}
```
For special keys, the Keyboard library assigns each key a key code and has macros for using them. When the device is plugged in it immediately sends the keystrokes necessary to open an administrator command prompt on a windows machine. Because of all the Windows UI and animations, it is necessary to throw in some delays. Instead of sending individual keystrokes, we can send key down and key up events separately for entering chords, such as `CTRL+SHIFT+ENTER` which runs a program as administrator.
```c
Keyboard.press(KEY_LEFT_CTRL);
delay(100);
Keyboard.press(KEY_LEFT_SHIFT);
delay(100);
Keyboard.press(KEY_RETURN);
delay(50);
Keyboard.releaseAll();
```
After the command prompt is open, the Atmega16u sends a byte to the 328p indicating it can begin sending the command strings. This allows us to run anything we want quickly and consistently while managing the memory constraints of the Atmega16u.

There were two paths we chose for how we approached the problem of removing League of Legends. One would be through creating a Windows executable that would run Windows-side, and the other by creating an Ubuntu executable that would mount the Windows files. The Windows executable would work if the computer was unlocked, but the Ubuntu executable would work even if the computer was locked. 
### Windows Approach
If the computer was unlocked and we had access to Windows, we could disable League of Legends by using a windows application we wrote to look for and delete a file from within a directory. We used a recursive function to find the complete file path to the specified file, and then deleted the file at the full file path. 
Here is a piece of the recursive function we used where we add to the file path, check our base case, and make the recursive call.
```c
//Build up our file path using the passed in
//[startingDirectory] and the file/foldername we just found:
sprintf(sPath, "%s\\%s", startingDirectory, fdFile.cFileName);

//Check if entity is the item we are looking for
if (strcmp(fdFile.cFileName, searchTerm) == 0) {
char *finalPath = strdup(sPath);
return finalPath;
} else if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
const char * result = findFilePath(sPath, searchTerm);
if (result != errorString) return result;
}
```
In order to find the file path of the file we searched for, we used the `FindFirstFile` and `FindNextFile` functions from the `windows.h` header file as well as the `WIN32_FIND_DATA` structure, which is how we find the name of the file or directory we are currently looking at and determine if it is a directory that needs to be opened. Once we find the full file path, we use the `DeleteFile` function to actually delete the file and check the output of the function to determine if the file was deleted successfully. 
```c
int error = deleteFileAtFilepath(filepath);
if (error == 0) {
puts("File deletion was unsuccessful");
```
When we run the Windows application in the command prompt, we pass two arguments, a starting directory and a file name to be deleted:
```cmd
deleteKeyFiles.exe C: RiotClientServices.exe
```
We intentionally chose to make both arguments inputs instead of constant values stored in the file. The starting directory is an argument so we can try to run the application using the standard download location that Riot uses which is much faster. If that fails we can run the same program with a different argument if League is installed in an unusual file location. The target file is an argument because that allows our application to be used on other games.
This is the output of the application with the above inputs:
```
C:
RiotClientServices.exe
Successfully deleted RiotClientServices.exe at location C:\Riot Games\Riot Client\RiotClientServices.exe
```
One of the most challenging elements of this project was learning how to write functions to be executed on Windows. There are tons of relevant search results, but many of these articles only tell you that you shouldn’t be making .exe files unless you know what you are doing. There was also lots of information about how to make installers for windows applications which is probably helpful for more large scale, professional projects, but not for our use case. It wasn’t until we found a specific stack overflow page on writing Windows functions on Linux that we finally found out what packages were necessary to install and what compiling function to use. Even after finding the mingw-w64 website and official help, it would have taken hours to figure out how to compile the function without the stack overflow page. Then we needed to find the Windows documentation so we could start interacting with the operating system. When we finally got our first Windows function, we then had to deal with Olin’s security features which detected our file as a threat and deleted the unrecognized application.
### Mounting Approach
Another path was to make a Ubuntu executable that would then mount the Windows file system from a live environment, and delete the file we intend to delete. In order for this to be done, we first must find the Windows partition, as the flash drive will have Ubuntu running. Although there are other, much [more complicated solutions to this](https://mirrors.edge.kernel.org/pub/linux/utils/util-linux/v2.21/libblkid-docs/libblkid-Partitions-probing.html) we settled back on using `system(sudo fdisk -l)`, which outputs all of the partition information. In order to take in this information from the terminal to be read later, we created a function called `search_partition()`, which creates a `.txt` file of the system call output. It then finds if the substring for the partition named "Microsoft basic data" is on a particular line, and outputs a pointer to that line. This is because, conveniently, the partitions go line by line. For example:
```
Device             Start        End   Sectors   Size Type
/dev/nvme0n1p1      2048    1085439   1083392   529M Windows recovery environment
/dev/nvme0n1p2   1085440    1288191    202752    99M EFI System
/dev/nvme0n1p3   1288192    1320959     32768    16M Microsoft reserved
/dev/nvme0n1p4   1320960  680766859 679445900   324G Microsoft basic data
/dev/nvme0n1p5 680767488  682233855   1466368   716M Windows recovery environment
/dev/nvme0n1p6 682235904 1000214527 317978624 151.6G Linux filesystem
```
So, when we find the line that states “Microsoft basic data”, it also gives the partition device location at the beginning of the line. This is the code that finds the pointer to this line by using `strstr`:
```c
while ((read = getline(&line, &len, fp)) != -1) {
        point_part = strstr(line,str);
        if (point_part) {
            break;
        }
    }
```
Then, we can get the beginning of the line by splitting it off at the first space. This is done as follows:
```c
// get location name by adding NULL at end of separator
strcpy(partition_location, line);
char *sep_location = strchr(partition_location, sep);
*sep_location = '\0'; // separate with NULL
```
Now, we have a char of the partition location, i.e. “/dev/nvme0n1p4”, which we can now use to be mounted onto the Ubuntu partition, the flash drive.
Originally, we had planned to use the system calls `mount()`, `unlink()`, and `umount()`, which would mount the filesystem, remove the file, and unmount the filesystem, respectively. Mount, however, is currently done through a system call - as `mount()` was only able to mount as Read-Only. However, `unlink()`, and `umount()` work perfectly, according to defined target location based on defaults for League of Legends.
To compare, here is an example of a system call compared to using `ulink()`:
```c
// system call
system("rm -f /mnt/'Riot Games'/'Riot Client'/RiotClientServices.exe");
// ulink
unlink("/mnt/'Riot Games'/'Riot Client'/RiotClientServices.exe")
```
The `umount()` and its respective system call use the same argument, the target location of the mount, so that was easily swapped. As it stands, this Ubuntu executable is able to find the location of the Windows partition of any computer, mount it to the Ubuntu drive, and delete the specified file in order to break League of Legends.

### Finding what to delete
In order to break the game and stop it from launching we had the option to delete one file, delete many files, or completely uninstall the game. We first tried deleting several game files, but found that when the game was launched, it used a separate application that was able to repair League by installing any missing files necessary for the game to run. We could have uninstalled the game completely, but that would have been easier to discover, so we ended up choosing to delete the Riot Client application. This means that no riot games can be launched from the desktop shortcut, and if you find the application in the folder, you are unable to connect to Riot servers. This is ideal because it seems like it will still work if opened from file manager but once you try to login you will realize that it is completely non-functional.

## Reflection
Our final product landed somewhere between the stretch goal we set and the MVP. We don’t currently have a way to stop League from being reinstalled, but we are able to uninstall it every time the computer starts up. If we had more time, we could add more functionality to continue to mess with League of Legends after our attack. We currently have it where every time the computer turns on, it will run our function again and delete the file. While this is very frustrating, if the target wanted to play badly enough, they could delete all remaining files and reinstall Riot and League every time they turn their computer on. One way we could potentially permanently stop them from playing is to make a program that looks to see if someone tries to open League of Legends and shuts off internet access if the game is opened, making them unable to play. The Windows API is huge, and with more time and experimentation, we could likely come up with dozens of ways to stop the game from running. 
### Andrew
The project did accomplish my learning goals. While the Windows functions I wrote were not very complicated, I now have the resources available  to work on more complex applications that could be of use regularly. For example, I could write an application to run on startup that disables Olin’s Symantec Endpoint Protection. I will definitely continue to write Windows applications and improve my knowledge of the Windows API.
### Melissa
I somewhat accomplished my learning goals. By working through this project ideation with my team members, I started strengthening my understanding of how operating systems work at a lower level. This ties in with how I can use C code to find, mount, and mess with a Windows file system from Ubuntu - with the purpose of booting it from Ubuntu. First, I built a lot of my understanding around it based on what would be done through the terminal - a `system()` call - and went through the docs for and replaced all the system calls I could with the appropriate C libraries and functions. I learned a lot through this, especially where to accept defeat (trying to use `mount()`, online forums often said plainly to use system calls instead).
I believe I would be comfortable approaching a project like this again. I built my understanding of reading the docs (as it’s been something I’ve struggled with) and how to problem solve along with what we’ve learned in class, and the many ways we could’ve approached reaching the solution.
### Elvis
My initial learning goal was very broad: I wanted to mess around with security stuff and see what I could do. I met this goal both in the part of the work I took on, but also through learning about the portions of the project Andrew and Melissa were in charge of. I have a much better understanding of linux and windows system calls, interacting with the file system, and how USB peripherals work based on this project. Going forwards I can think of a lot of fun extensions and other areas of interest based on this work.
Midway through the project I found myself excited about the embedded system of the rubber ducky device.  The process of creating it and writing the firmware gave me a much better sense of how bootloaders work and USB Human Interface Devices. Although I didn’t get into advanced features as much as I would have liked, I learned a lot getting it to the current state. I’m confident in my understanding of how the system works and plan to follow through on designing a PCB that incorporates a USB mass storage device and virtual HID device. This would allow the functionality of the Arduino and flash drive to be delivered by a single USB device that appears like a completely normal flash drive.


