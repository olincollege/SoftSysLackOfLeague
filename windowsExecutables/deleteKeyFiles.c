#include "findFile.h"


int deleteFileAtFilepath(const char *filePath){
    return DeleteFile(filePath);
}




int main(int argc, char *argv[]) {

    if (argc != 3) { // the name of the function is counted as an arg
        puts("Usage: deleteKeyFiles.exe [starting directory] [file name]");
        return 1;
    }
    char *startDirectory = argv[1];
    char *fileName = argv[2];

    puts(startDirectory);
    puts(fileName);

    const char *filepath = findFilePath(startDirectory, fileName);
    int error = deleteFileAtFilepath(filepath);
    if (error == 0){
        puts("File deletion was unsuccessful");
        return 0;
    }

    printf("Successfully deleted file %s at location %s\n", fileName, filepath);
    //printf("%p\n",*filePath);
    free_anything(filepath);
    return 1;
}
