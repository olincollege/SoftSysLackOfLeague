#include "findFile.h"

const char* findFilePath(const char *startingDirectory, const char *searchTerm)
/*
Finds the complete file path of searchTerm from startingDirectory

Args:
    startingDirectory: a str of the desired starting directory as you would
    read it from file explorer

    searchTerm: The name of the file you want to delete including its file type
    for example, RiotClientServices.exe

Returns:
    A const char * to the full file path stored in allocated memory. MUST FREE.
*/
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
    char *errorString = "Path not found";

    //Specify a file mask. (regex)
    sprintf(sPath, "%s\\*.*", startingDirectory);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        //printf("Path not found: [%s]\n", startingDirectory);
        return errorString;
    }

    do {
        //Find first file will always return "."
        //and ".." as the first two directories.
        if (strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0) {
            //Build up our file path using the passed in
            //[startingDirectory] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", startingDirectory, fdFile.cFileName);

            //Check if entity is the item we are looking for
            if (strcmp(fdFile.cFileName, searchTerm) == 0) {
                char *finalPath = strdup(sPath);
                //printf("%p\n", *finalPath);
                return finalPath;
            } else if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY) {
                //printf("Directory: %s\n", sPath);
                const char * result = findFilePath(sPath, searchTerm);
                if (result != errorString) return result;
            }
        }
    } while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind);

    return errorString;
}

void free_anything(int *p) {
    free(p);
}
