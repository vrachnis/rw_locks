.SUFFIXES: .c .o

LIBSRC = _rw_locks.c
LIBOBJ = ${LIBSRC:.c=.o}

TESTSRC = test.c
TESTOBJ = ${TESTSRC:.c=.o}

LIBFILE= librwlock.a
TARGET = test

CC = gcc
CFLAGS = -Wall -g -lpthread -std=c89
#CFLAGS = -pipe -std=c99 -pedantic -Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wno-unused-parameter -D_XOPEN_SOURCE=600
#DBGFLAGS = -g -fPIC 

ECHO = /bin/echo

all: librwlock target

%.o: %.c
	@${ECHO} " * Compiling " $<
	${CC} -c ${CFLAGS} -o $@ $<

librwlock: ${LIBOBJ}
	@$(ECHO) " * Generating library archive..."
	rm -f ${LIBFILE}
	ar qv ${LIBFILE} ${LIBOBJ}
	ranlib ${LIBFILE}

target: ${TESTOBJ}
	${CC}  ${CFLAGS} -o ${TARGET} ${TESTOBJ} ${LIBFILE}

clean:
	@$(ECHO) " * Cleaning up..."
	@rm -f ${LIBOBJ} ${TESTOBJ} ${LIBFILE} *~ ${TARGET}

run: all
	@$(ECHO) " * Running..."
	@sh -c ./test
