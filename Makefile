CC = gcc
CFLAGS = -Wall -g -lpthread
#INCLUDES = -I /path
#LIBS = -L /path

all: test

test: test.o
	$(CC) $(CFLAGS) -o test test.o

test.o: test.c
	$(CC) $(CFLAGS) $(INCLUDES) -c test.c

clean:
	rm -f ask1 ask2 *.o core.*
