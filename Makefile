main: main.o datadef.o file.o datamanip.o microdb.h
	cc -o main -g  main.o file.o datadef.o datamanip.o -lreadline -lcurses
datadef.o:datadef.c microdb.h
	gcc -c -g datadef.c

datamanip.o:datamanip.c microdb.h
	gcc -c -g datamanip.c 

file.o:file.c microdb.h
	gcc -c -g file.c

main.o:main.c microdb.h
	gcc -c -g main.c
	
