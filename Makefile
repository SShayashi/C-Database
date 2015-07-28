main: main.o datadef.o file.o datamanip.o
	cc -o main -g  main.o file.o datadef.o datamanip.o -lreadline -lcurses
datadef.o:datadef.c
	cc -c -g datadef.c

datamanip.o:datamanip.c
	cc -c -g datamanip.c 

file.o:file.c
	cc -c -g file.c

main.o:main.c
	cc -c -g main.c
	
