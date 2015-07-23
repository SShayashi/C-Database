cc -o file.o -g -c file.c
cc -o test-file.o -g -c test-file.c
cc -o FileTest -g  file.o test-file.o
./FileTest
