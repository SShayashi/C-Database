cc -o file.o -g -c file.c
cc -o datadef.o -g -c datadef.c
cc -o datamanip.o -g -c datamanip.c
cc -o test-datamanip.o -g -c test-datamanip.c

cc  -o test -g test-datamanip.o file.o datadef.o datamanip.o 
./test
