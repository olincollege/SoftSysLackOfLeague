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

## Getting Started

We need to get a sacrificial laptop that we can test our code on without worrying about messing something important up. We have a plan to get one. We’ll also need a flashdrive. We learn how to set up a bootable live environment and how interacting with Windows from linux works.

## First Steps
1. Create a bootable flash drive.

**Definition of Done**: We are able to connect the flash drive to a computer with Windows 10 and boot to Ubuntu.

1. Compile an executable and run it on Windows.

**Definition of Done**: We are able to compile a hello world program and run it natively on Windows 10.

1. Mount Windows file system from Ubuntu.

**Definition of Done**: We are able to mount the file system of the connected Windows 10 computer from a program running on the Ubuntu live environment.
