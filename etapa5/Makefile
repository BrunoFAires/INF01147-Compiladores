IDIR = include
CC = gcc
CFLAGS = -I$(IDIR) -DENABLE_LOGS #  -Wall -g
ODIR = build
SDIR = src
LOGDIR = logs
LIBS = -lfl

_DEPS = util.h parser.tab.h asd.h lex_value.h pilha.h tabela.h macros.h instrucao.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o util.o lex.yy.o parser.tab.o asd.o lex_value.o pilha.o tabela.o instrucao.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: dirs clean run

etapa5: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

run: etapa5
	@echo Executando etapa 5...
	./etapa5

# Bison
B_DEPS = $(SDIR)/parser.y $(SDIR)/util.c $(IDIR)/util.h $(SDIR)/asd.c $(IDIR)/asd.h $(SDIR)/lex_value.c $(IDIR)/lex_value.h $(SDIR)/pilha.c $(IDIR)/pilha.h $(SDIR)/tabela.c $(IDIR)/tabela.h $(IDIR)/macros.h $(SDIR)/instrucao.c $(IDIR)/instrucao.h
 $(IDIR)/parser.tab.h: $(B_DEPS) | dirs
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