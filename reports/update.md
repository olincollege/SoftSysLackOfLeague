# Lack of League

> Andrew DeCandia, Melissa Kazazic, Elvis Wolcott

## Goal

Our goal is to prepare a flash drive with a program written in C that is able to prevent the target device from opening League of Legends.

### MVP

Create a program that is able to run on a live USB and mount the system's drives to locate and remove a file.

### Stretch Goal

The program additionally places an executable onto the system that will prevent reinstalling League of Legends.

## Learning Goals

### Andrew

I want to be able to create a program for Windows that interacts with the operating system. I’d like to understand more about how Windows and Windows programs function.

### Melissa

Learn enough to be comfortable approaching a similar project again on my own. I’d like to understand how operating systems work at a lower level.

### Elvis

I’m interested in exploring the space of security research in general. I have never done anything similar, so I hope to learn more about how different programs and elements of the operating system interact and how they can be manipulated.

I have realized there is a lot I am excited about on the firmware side with the rubber ducky to explore now that I understand the how the different chips and HID interact. I would like to dig deeper into the HID library and/or more complex firmware features in the second phase of the project. I hope to learn enough that I could design a more compact PCB around the same chips and write the firmware from scratch myself in the future.

## Progress

For development, we secured two Arduino Unos, an old laptop for testing, and a bootable linux drive.

So far we have made good progress in writing Windows executable functions. We are using mingw-w64 and the windows.h header file combined with Microsoft's documentation page: https://docs.microsoft.com/en-us/windows/win32/api/. We have successfully learned how to compile functions as Windows executables, and we have successfully implemented a function to read the contents of a directory in windows. With this we can easily find the file path of the specific file we want to modify.

Another path for our project is to boot up Ubuntu using the flash drive, and the first step would be to mount the file system from there. With using `system()` to use a command to obtain the partition information, and redirecting its output to a `.txt` file to be read by the program, we are able to search for the required partition address for the "Windows basic data" partition, mount it, and read its contents. This has all been successfully implemented. From then on, we would be able to manipulate what is on the user's Windows partition for our project plan.

We realized the being able to automate interactions with the target system could come in useful for installing our payload or bypassing prompts. We created a MVP of a "rubber ducky" like device using an Arduino and our own firmware.

On all fronts, we are nearing MVP and ready to aim for our stretch goals.
## Work in Progress

1. Program to search the windows filesystem for a specified file and delete it - **Andrew DeCandia**

**Definition of Done**: We are able to produce an executable that when run an a Windows 10 machine finds and deletes a specified file.

2. Write a windows executable that can modify an existing file in the windows file system and get it to run additional executables. - **Andrew DeCandia**

**Definition of Done**: We are able to prove that we have modified an existing executable file by running a separate executable from the modified file that prints something to command prompt. 

3. Mirror windows version functionality on live environment (linux) version.

**Definition of Done**: The program running on the linux live environment should meet the *Definition of Done* for each feature of the windows program. - **Melissa Kazazic**

4. HID firmware from scratch.

**Definition of Done**: Without using the HID library, the Arduino rubber ducky is able to be detected as an HID device by a computer. - **Elvis Wolcott**

### Todoist
**https://todoist.com/app/project/2285409471**
### GitHub
**https://github.com/olincollege/SoftSysLackOfLeague**
