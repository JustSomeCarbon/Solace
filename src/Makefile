CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAGS=-Wall -ggdb3 -c

solc: solace.o
	$(CC) $(CFLAGS) solace.o -o solc

solace.o: solace.c solace.h
	$(CC) $(OBJFLAGS) solace.c

lex.o: lex.c lex.h
	$(CC) $(OBJFLAGS) lex.c

clean:
	rm solc *.o
