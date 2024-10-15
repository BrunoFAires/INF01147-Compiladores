CC = gcc
CFLAGS = -I.
LIBS = -lfl

# Files and dependencies
DEPS = include/util.h parser.tab.h
OBJ = src/main.o src/util/util.o src/lex.yy.o parser.tab.o

# Target to build the executable
etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Rule to generate parser.tab.c and parser.tab.h using Bison
parser.tab.c parser.tab.h: parser.y include/util.h
	bison -d parser.y

# Rule to compile parser.tab.c into parser.tab.o
parser.tab.o: parser.tab.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to compile main.c, depending on parser.tab.h and other headers
src/main.o: src/main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to compile lex.yy.c using Flex, depending on parser.tab.h
src/lex.yy.c: scanner.l parser.tab.h
	flex -o $@ scanner.l

# Rule to compile object files for other source files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean rule to remove generated files
clean:
	find . \( -type f -name "*.o" -o -name "etapa[0-9]" -o -name "lex.yy.c"  -o -name "parser.tab.*"  \) -delete
