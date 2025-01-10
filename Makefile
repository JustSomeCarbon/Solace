CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAGS=-Wall -ggdb3 -c

solc: solace.o
	$(CC) $(CFLAGS) solace.o

solace.o: solace.c solace.h
	$(CC) $(OBJFLAGS) solace.c

clean:
	rm solc *.o
