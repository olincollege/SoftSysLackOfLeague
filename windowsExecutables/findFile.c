#include "findFile.h"

const char* findFilePath(const char *sDir, const char *searchTerm)
/*

*/
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];
    char *errorString = "Path not found";

    //Specify a file mask. (regex)
    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        //printf("Path not found: [%s]\n", sDir);
        return errorString;
    }

    do
    {
        //Find first file will always return "."
        //and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            //Build up our file path using the passed in
            //[sDir] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            //Check if entity is the item we are looking for
            if(strcmp(fdFile.cFileName, searchTerm) == 0)
            {
                char *finalPath = strdup(sPath);
                //printf("%p\n", *finalPath);
                return finalPath;
            }
            else if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                //printf("Directory: %s\n", sPath);
                const char * result = findFilePath(sPath, searchTerm);
                if (result != errorString) return result;
            }
            //else{
            //  printf("File: %s\n", sPath);
            //}
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind);

    return errorString;
}

void free_anything(int *p){
    free(p);
}
