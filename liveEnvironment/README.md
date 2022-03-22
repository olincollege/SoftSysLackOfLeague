# Live Environment Setup

We have not included the ISO for the live environment as it is a rather large binary.

It can be created from scratch using a few tools.


1. Download the ISO for the base image. We used Lubuntu as it is lighter weight.
    ```bash
    wget https://cdimage.ubuntu.com/lubuntu/releases/20.04.4/release/lubuntu-20.04.4-desktop-amd64.iso -O ~/Downloads/lubuntu.iso
    ```
1. Plug in your USB drive and remove any other external storage devices. Then, list the storage devices. 
    You should see something starting with `sd` (e.g. `sda`).
    ```bash
    lsblk
    ```

## Option 1: Live Environment Only
1. Wipe the disk and write the ISO. It is good to double check you have the right drive before doing this.
    ```bash
    sudo umount /dev/sdX1
    sudo dd bs=4M if=/home/$USER/Downloads/lubuntu.iso of=/dev/sdX conv=fdatasync status=progress
    ```
    This step might take a few minutes to complete.exa

## Option 2: Live Environment + Storage with mkusb
1. Install mkusb
    ```bash
    sudo add-apt-repository ppa:mkusb/unstable
    sudo apt update
    sudo apt install mkusb usb-pack-efi
    ```
1. Run `mkusb`
    ```bash
    mkusb ~/Downloads/lubuntu.iso
    ```
1. Select `p` for the GUI.
1. Follow the prompts to create a live USB with a `NTFS` formatted `usbdata` partition.


## Option 3: Manual Creation of Live Environment + Storage
> Warning: This method does not seem to consistently work

We need to reformat the drive to have a partition where we can store files and a partition where we can store the operating system.

1. Unmount any partitions which are currently mounted e.g.
    ```bash
    umount /dev/sda1
    ```
1. Open the disk with `parted`
    ```bash
    sudo parted /dev/sdX
    ```
1. In the parted prompt, select the disk
    ```parted
    select /dev/sda
    ```
1. View the partition table
    ```parted
    print
    ```
1. Wipe the drive. If there are errors ignore them.
    ```parted
    mktable gpt
    ```
1. Create a storage partition and os partition.
    ```parted
    mkpart storage ntfs 25MB 10GB
    mkpart os fat32 10GB 15GB
    ```
1. Mark the second partition as a boot partition.
    ```parted
    set 2 boot on
    quit
    ```
1. Install `isohybrid` and modify the ISO for installation to a partition.
    ```bash
    sudo apt-get install syslinux-utils
    isohybrid --partok ~/Downloads/lubuntu.iso
    ```
1. Copy it to the second partition.
    ```bash
    sudo dd bs=4M if=/home/$USER/Downloads/lubuntu.iso of=/dev/sdX2 conv=fdatasync status=progress
    ```

