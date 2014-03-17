CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = hw4

all:	$(TARGET)

$(TARGET):	$(TARGET).o keygen.o crypt.o invkey.o histo.o solve.o util.o cmdread.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c keygen.o crypt.o invkey.o histo.o solve.o util.o cmdread.o

keygen.o:	keygen.c hw4.h
	$(CC) $(CFLAGS) -c keygen.c

crypt.o:	crypt.c hw4.h
	$(CC) $(CFLAGS) -c crypt.c

invkey.o:	invkey.c hw4.h
	$(CC) $(CFLAGS) -c invkey.c

histo.o:	histo.c hw4.h
	$(CC) $(CFLAGS) -c histo.c

solve.o:	solve.c hw4.h
	$(CC) $(CFLAGS) -c solve.c

util.o:		util.c hw4.h
	$(CC) $(CFLAGS) -c util.c

cmdread.o:	cmdread.c hw4.h
	$(CC) $(CFLAGS) -c cmdread.c

$(TARGET).o:	hw4.c
	$(CC) $(CFLAGS) -c hw4.c

clean:
	$(RM) $(TARGET) *.o *~

