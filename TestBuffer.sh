cc -o file.o -g -c file.c
cc -o test-buffer.o -g -c test-buffer.c
cc -o FileBuffer -g  file.o test-buffer.o
./FileBuffer
