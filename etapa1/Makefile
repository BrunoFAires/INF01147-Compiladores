IDIR = include
CC = gcc
CFLAGS = -I$(IDIR)
ODIR = build
BINDIR = bin
SRCDIR = src
LIBS = -lfl

_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o utils.o lex.yy.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: dirs clean run

etapa1: $(OBJ)
	@echo Compilando etapa 1...
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)	

run: etapa1
	@echo Executando etapa 1...
	./$(BINDIR)/etapa1

dirs:
	mkdir -p $(ODIR)
	mkdir -p $(BINDIR)

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS) | dirs
	$(CC) -c -o $@ $< $(CFLAGS)

$(SRCDIR)/%.yy.c: scanner.l
	flex -o $@ $<

clean:
	rm -rf $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BINDIR)/*