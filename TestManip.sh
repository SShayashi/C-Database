cc -o data-manip.o -g -c datamanip.c
cc -o test-datamanip.o -g -c test-datamanip.c
cc -o TestDatamanip -g  datamanip.o test-datamanip.o
./TestDatamanip
