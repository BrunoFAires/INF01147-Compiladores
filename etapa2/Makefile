CC=gcc
CFLAGS=-I.
DEPS = src/tokens.h include/util.h parser.tab.h
OBJ = src/main.o src/util/util.o src/lex.yy.o
LIBS = -lfl

parser.tab.c: parser.y 
	bison -d src/parser.tab.c parser.y

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

src/lex.yy.c: scanner.l parser.tab.h
	flex -o src/lex.yy.c scanner.l

clean:
	find . \( -type f -name "*.o" -o -name "etapa[0-9]" -o -name "lex.yy.c"  -o -name "parser.tab.*"  \) -delete