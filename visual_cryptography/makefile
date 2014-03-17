CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = hw2
LinkOptions = -L/home/scf-22/csci551b/openssl/lib -lcrypto
CompileOption = -I/home/scf-22/csci551b/openssl/include

all:	$(TARGET)

$(TARGET):	$(TARGET).o decrypt.o merge.o encrypt.o cmdread.o stream.o util.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c decrypt.o merge.o encrypt.o cmdread.o stream.o util.o $(LinkOptions) $(CompileOption)

decrypt.o:	decrypt.c hw2.h
	$(CC) $(CFLAGS) -c decrypt.c $(CompileOption)

merge.o:	merge.c hw2.h
	$(CC) $(CFLAGS) -c merge.c $(CompileOption)

encrypt.o:	encrypt.c hw2.h
	$(CC) $(CFLAGS) -c encrypt.c $(CompileOption)

cmdread.o:	cmdread.c hw2.h
	$(CC) $(CFLAGS) -c cmdread.c $(CompileOption)

stream.o:	stream.c hw2.h
	$(CC) $(CFLAGS) -c stream.c $(CompileOption)

util.o:		util.c hw2.h
	$(CC) $(CFLAGS) -c util.c $(CompileOption)

$(TARGET).o:	hw2.c hw2.h
	$(CC) $(CFLAGS) -c hw2.c $(CompileOption)


clean:
	$(RM) $(TARGET) *.o *~

