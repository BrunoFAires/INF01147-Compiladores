IDIR = include
CC = gcc
CFLAGS = -I$(IDIR)
ODIR = build
SDIR = src
LOGDIR = logs
LIBS = -lfl

_DEPS = util.h parser.tab.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o util.o lex.yy.o parser.tab.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: dirs clean run

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: etapa2
	@echo Executando etapa 2...
	./etapa2

# Bison
$(IDIR)/parser.tab.h: $(SDIR)/parser.y $(IDIR)/util.h | dirs
	bison -d -v -o $(SDIR)/parser.tab.c $<
	mv $(SDIR)/parser.tab.h $(IDIR)
	mv $(SDIR)/parser.output $(LOGDIR)

# Flex
$(SDIR)/lex.yy.c: $(SDIR)/scanner.l $(IDIR)/parser.tab.h
	flex -o $@ $<

# Gerar .o para parser
$(ODIR)/parser.tab.o: $(SDIR)/parser.tab.c $(IDIR)/parser.tab.h
	$(CC) -c -o $@ $< $(CFLAGS)

# Gerar .o para src
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

dirs:
	mkdir -p $(ODIR)
	mkdir -p $(LOGDIR)

clean:
	rm -rf $(ODIR)/*.o $(SDIR)/lex.yy.* $(IDIR)/lex.yy.* $(SDIR)/parser.tab.* $(IDIR)/parser.tab.* $(LOGDIR)/* etapa[1-7]