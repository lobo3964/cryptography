CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = hw1

all:	$(TARGET)

$(TARGET):	$(TARGET).o hex.o base64.o util.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c hex.o base64.o util.o

util.o:	util.c hw1.h
	$(CC) $(CFLAGS) -c util.c

base64.o: base64.c hw1.h
	$(CC) $(CFLAGS) -c base64.c
	
hex.o:	hex.c hw1.h
	$(CC) $(CFLAGS) -c hex.c

$(TARGET).o:	hw1.c hw1.h
	$(CC) $(CFLAGS) -c hw1.c


clean:
	$(RM) $(TARGET) *.o *~

