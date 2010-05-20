# CC = gcc
# INCLUDES = -I ./
# LIBS = -L .

# SRCS=_rw_locks.c test.c
# OBJ=$(SRCS:..c=.o)
# TARGET=test

# all: test

# test: test.o mylib
# 	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(TARGET) $(OBJ) 

# ..c.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $<

# test.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c test.c

# mylib: _rw_locks.o
# 	ar rcs librwlock.a _rw_locks.o

# _rw_locks.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c _rw_locks.c

# clean:
# 	rm -f test *.o core.* *~ *.a

#####################################################################################

.SUFFIXES: .c .o

LIBSRC = _rw_locks.c
LIBOBJ = ${LIBSRC:.c=.o}

TESTSRC = test.c
TESTOBJ = ${TESTSRC:.c=.o}

LIBFILE= librwlock.a
TARGET = test

CC = gcc
CFLAGS = -Wall -g -lpthread #-lrwlock
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
	@rm -f ${LIBOBJ} ${TESTOBJ} ${LIBFILE} *~

run:
	sh -c ./test

# lines: clean
# 	@cloc .