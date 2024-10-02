CC=gcc
CFLAGS=-I.
DEPS = tokens.h util.h
OBJ = main.o util.o lex.yy.o
LIBS = -lfl

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

lex.yy.c: scanner.l
	flex -o lex.yy.c scanner.l

clean:
	find . \( -type f -name "*.o" -o -name "etapa[0-9]" -o -name "lex.yy.c" \) -delete