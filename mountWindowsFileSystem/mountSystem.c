#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// global values
char partitions_file[] = "partitions.txt";
char keyword[] = "Microsoft basic data";
char mount_command[50] = "sudo mount ";
char partition_location[50];

// function to search for the Device name of partition
int search_partition(char *fname, char *str) {
    FILE *fp;
    char * line = NULL;
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

int main() {
    // terminal function to check for partitions
    char command[50];
    strcpy(command, "sudo fdisk -l > ");
    strcat(command, partitions_file);
    system(command);
    // next, search for partition
    search_partition(partitions_file, keyword);
    // finally, mount onto drive
    strcat(mount_command, partition_location);
    strcat(mount_command, " /mnt");
    printf("mount command: %s\n", mount_command);
    system(mount_command);
    // print contents to terminal
    system("cd /mnt && ls");

    // unmount at the end
    system("sudo umount /mnt");
    return(0);
}