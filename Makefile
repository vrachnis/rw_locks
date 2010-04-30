CC = gcc
CFLAGS = -Wall -g -lpthread
INCLUDES = -I .
LIBS = -L .

all: test

test: test.o
	$(CC) $(CFLAGS) -o test test.o

test.o: test.c
	$(CC) $(CFLAGS) $(INCLUDES) -c test.c

run:
	sh -c ./test

clean:
	rm -f test *.o core.* *~
