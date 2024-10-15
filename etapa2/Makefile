CC = gcc
CFLAGS = -I.
LIBS = -lfl

DEPS = include/util.h include/parser.tab.h
OBJ = src/main.o src/util/util.o src/lex.yy.o src/parser.tab.o

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

src/parser.tab.c include/parser.tab.h: parser.y include/util.h
	bison -d -o src/parser.tab.c parser.y

src/parser.tab.o: src/parser.tab.c
	$(CC) -c -o $@ $< $(CFLAGS)

src/main.o: src/main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

src/lex.yy.c: scanner.l include/parser.tab.h
	flex -o $@ scanner.l

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	find . \( -type f -name "*.o" -o -name "etapa[0-9]" -o -name "lex.yy.c"  -o -name "parser.tab.*"  \) -delete
