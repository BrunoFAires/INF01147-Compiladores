#ifndef TABELA_H
#define TABELA_H

#include "lex_value.h"

typedef enum Natureza
{
  FUNCAO,
  IDENTIFICADOR
} Natureza;

typedef enum TipoDado
{
  FLOAT,
  INTEIRO
} TipoDado;

typedef struct Entrada
{
  int linha;
  Natureza natureza;
  TipoDado tipoDado;
  lex_value_t value;
} Entrada;

typedef struct Tabela
{
  Entrada Entrada;
  Entrada *proxima;
} Tabela;

Tabela *criarTabela();
void destruirTabela(Tabela *table);

void buscarEntrada(Tabela *tabela, char *valor);
void inserir(Tabela *tabela, Entrada *entrada);

void destruirEntrada(Entrada *entrada);
Entrada *criarEntrada();

#endif