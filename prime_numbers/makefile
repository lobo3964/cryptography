CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = hw7
LinkOptions = -L/home/scf-22/csci551b/openssl/lib -lcrypto 
CompileOption = -I/home/scf-22/csci551b/openssl/include -lm

all:	$(TARGET)

$(TARGET):	$(TARGET).o prime.o traildiv.o millerrabin.o rndsearch.o cmdread.o util.o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c prime.o traildiv.o millerrabin.o rndsearch.o cmdread.o util.o $(LinkOptions) $(CompileOption)

prime.o:	prime.c hw7.h
	$(CC) $(CFLAGS) -c prime.c $(CompileOption)

traildiv.o:	traildiv.c hw7.h
	$(CC) $(CFLAGS) -c traildiv.c $(CompileOption)

millerrabin.o:	millerrabin.c hw7.h
	$(CC) $(CFLAGS) -c millerrabin.c $(CompileOption)

rndsearch.o: rndsearch.c hw7.h
	$(CC) $(CFLAGS) -c rndsearch.c $(CompileOption)

util.o:		util.c hw7.h
	$(CC) $(CFLAGS) -c util.c $(CompileOption)

cmdread.o:	cmdread.c hw7.h
	$(CC) $(CFLAGS) -c cmdread.c $(CompileOption)

$(TARGET).o:	hw7.c hw7.h
	$(CC) $(CFLAGS) -c hw7.c $(CompileOption)

clean:
	$(RM) $(TARGET) *.o *~

