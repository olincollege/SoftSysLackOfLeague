#include <stdio.h>
#include <windows.h>

int ListDirectoryContents(const char *sDir)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    //Specify a file mask. (regex)
    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("Path not found: [%s]\n", sDir);
        return 0;
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

            //Check if entity is file or folder
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                printf("Directory: %s\n", sPath);
                ListDirectoryContents(sPath);
            }
            else{
                printf("File: %s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind);

    return 1;
}

int main() {
    ListDirectoryContents("C:\\");
}
