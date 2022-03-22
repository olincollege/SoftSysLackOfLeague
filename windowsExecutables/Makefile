directorySearch.exe: directorySearch.c
	i686-w64-mingw32-gcc -o directorySearch.exe directorySearch.c

helloWorld.exe: helloWorld.c
	i686-w64-mingw32-gcc -o helloWorld.exe helloWorld.c

findFile.o: findFile.c findFile.h
	i686-w64-mingw32-gcc -c -g findFile.c findFile.h

deleteKeyFiles.exe: deleteKeyFiles.c findFile.o
	i686-w64-mingw32-gcc -g -o deleteKeyFiles.exe findFile.o deleteKeyFiles.c