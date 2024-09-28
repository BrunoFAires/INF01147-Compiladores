CC=gcc
CFLAGS=-I.
DEPS = src/tokens.h include/util.h
OBJ = src/main.o src/util/util.o src/lex.yy.o
LIBS = -lfl

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

src/lex.yy.c: scanner.l
	flex -o src/lex.yy.c scanner.l