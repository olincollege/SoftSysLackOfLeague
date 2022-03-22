#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sys/mount.h>

// global values
char partitions_file[15] = "partitions.txt";
char keyword[21] = "Microsoft basic data";
char mount_command[50] = "sudo mount ";

// name of location on drive
char target_location[6]= "/mnt";

// if certain files were renamed, could be redone. for now, we know this is the
// default location through the code and Riot Games downloads.
char file_to_delete[55];

char partition_location[50];

// function to search for the Device name of partition
int search_partition(char *fname, char *str) {
    // set up file
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *point_part;
    char sep = ' ';
    // open file, check for error
    if((fp = fopen(fname, "r")) == NULL) {
        perror("Error opening file");
        return(-1);
    }
    // search line by line for keyword substring
    while ((read = getline(&line, &len, fp)) != -1) {
        point_part = strstr(line,str);
        if (point_part) {
            break;
        }
    }
    // make sure to close file
    if(fp) {
		fclose(fp);
	}
    printf("found substring at address %p\n", point_part);
    printf("substring pointer: %p\n", line);
    printf("substring line: %s\n", line);
    // get location name by adding NULL at end of separator
    strcpy(partition_location, line);
    char *sep_location = strchr(partition_location, sep);
    *sep_location = '\0'; // separate with NULL
    printf("partition location: %s\n", partition_location);
    return(0);
}

// get proper name of file location to be deleted
void fix_file_location() {
    // determine delete location from globals
    char location_file_to_delete[51] = "/'Riot Games'/'Riot Client'/RiotClientServices.exe";
    strcpy(file_to_delete, target_location);
    strcat(file_to_delete, location_file_to_delete);
    // doing this will also free unused chars space used
}

int main() {
    // terminal function to check for partitions
    char command[50] = "sudo fdisk -l > ";
    strcat(command, partitions_file);
    system(command);
    // next, search for partition
    search_partition(partitions_file, keyword);
    // start to mount partition
    // make sure directory exists first, should exist, but just in case
    struct stat st = {0};
    if (stat(target_location, &st) == -1) {
        mkdir(target_location, 0777);
    }
    // mount command
    strcat(mount_command, partition_location); // source
    strcat(mount_command, " ");
    strcat(mount_command, target_location); // target
    printf("mount command: %s\n", mount_command);
    system(mount_command);
    // print contents to terminal
    system("cd /mnt && ls");
    system("cd /mnt/'Riot Games'/'Riot Client' && ls");
    // delete proper file
    fix_file_location();
    //if(remove(file_to_delete) != 0) {
    //    // force through terminal command if needed
    //    system("rm -f /mnt/'Riot Games'/'Riot Client'/RiotClientServices.exe")
    //    perror( "Error deleting file through remove" );
    //}
    //else {
    //    puts( "File successfully deleted with remove");
    //}
    // unmount at the end
    system("sudo umount /mnt");
    return(0);
}