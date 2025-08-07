CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAGS=-Wall -ggdb3 -c

anvl: solace.o
	$(CC) $(CFLAGS) solace.o -o anvl

solace.o: solace.c solace.h
	$(CC) $(OBJFLAGS) solace.c

lex.o: lex.c lex.h
	$(CC) $(OBJFLAGS) lex.c

clean:
	rm solc *.o
