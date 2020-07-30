cc = gcc
TARGET = 20151523

$(TARGET) : 20151523.o
	$(cc) -o 20151523.out 20151523.c
20151523.o : 20151523.c 20151523.h
	$(cc) -c 20151523.c
clean :
	rm *.o
	rm *.out
