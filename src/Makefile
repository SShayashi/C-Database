main: main.o datadef.o file.o datamanip.o microdb.h
	cc -o ../bin/main -g  main.o file.o datadef.o datamanip.o -lreadline -lcurses
datadef.o:datadef.c microdb.h
	cc -c -g datadef.c

datamanip.o:datamanip.c microdb.h
	cc -c -g datamanip.c

file.o:file.c microdb.h
	cc -c -g file.c

main.o:main.c microdb.h
	cc -c -g main.c

clean:
	rm -rf main.o file.o datamanip.o datadef.o
