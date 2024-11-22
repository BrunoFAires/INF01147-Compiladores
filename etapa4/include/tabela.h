#ifndef TABELA_H
#define TABELA_H

#include "lex_value.h"

typedef struct Tabela
{
  lex_value_t **entradas;
  int num_entradas;
} tabela_t;

tabela_t *criarTabela(lex_value_t *entrada);
void destruirTabela(tabela_t *tabela);

void buscarEntrada(tabela_t *tabela, char *valor);
void inserirEntrada(tabela_t *tabela, lex_value_t *entrada);

void printTabela(tabela_t *tabela);

#endif