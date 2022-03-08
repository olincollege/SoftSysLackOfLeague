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

## Progress

So far we have made good progress in writing Windows executable functions. We are using mingw-w64 and the windows.h header file combined with Microsoft's documentation page: https://docs.microsoft.com/en-us/windows/win32/api/. We have successfully learned how to compile functions as Windows executables, and we have successfully implemented a function to read the contents of a directory in windows. With this we can easily find the file path of the specific file we want to modify. 

## Next Steps

1. Create a bootable flash drive.

**Definition of Done**: We are able to connect the flash drive to a computer with Windows 10 and boot to Ubuntu.

2. Write a windows executable that can modify an existing file in the windows file system and get it to run additional executables.

**Definition of Done**: We are able to prove that we have modified an existing executable file by running a separate executable from the modified file that prints something to command prompt. 

3. Mount Windows file system from Ubuntu.

**Definition of Done**: We are able to mount the file system of the connected Windows 10 computer from a program running on the Ubuntu live environment.


### Todoist
** https://docs.microsoft.com/en-us/windows/win32/api/ **
### GitHub
** https://github.com/olincollege/SoftSysLackOfLeague **
