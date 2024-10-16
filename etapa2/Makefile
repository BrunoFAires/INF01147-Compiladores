IDIR = include
CC = gcc
CFLAGS = -I$(IDIR)
ODIR = build
SDIR = src
LIBS = -lfl

_DEPS = util.h parser.tab.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o util.o lex.yy.o parser.tab.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: dirs clean run[1-7]

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run2: etapa2
	@echo Executando etapa 2...
	./etapa2

run1: etapa1
	@echo Executando etapa 1...
	./etapa1

# Bison
$(IDIR)/parser.tab.h: $(SDIR)/parser.y $(IDIR)/util.h
	bison -d -o $(SDIR)/parser.tab.c $<
	mv $(SDIR)/parser.tab.h $(IDIR)

# Flex
$(SDIR)/lex.yy.c: $(SDIR)/scanner.l $(IDIR)/parser.tab.h
	flex -o $@ $<

# Gerar .o para parser
$(ODIR)/parser.tab.o: $(SDIR)/parser.tab.c $(IDIR)/parser.tab.h
	$(CC) -c -o $@ $< $(CFLAGS)

# Gerar .o para src
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | dirs
	$(CC) -c -o $@ $< $(CFLAGS)

dirs:
	mkdir -p $(ODIR)

clean:
	rm -rf $(ODIR)/*.o $(SDIR)/lex.yy.* $(IDIR)/lex.yy.* $(SDIR)/parser.tab.* $(IDIR)/parser.tab.* etapa[1-7]