#ifndef PILHA_H
#define PILHA_H

#include "tabela.h"

typedef struct Pilha
{
  tabela_t *tabela;
  struct Pilha *proximo;
} pilha_t;

pilha_t *criarPilha();
void destruirPilha(pilha_t *topo);

void buscar(pilha_t *topo, char *valor);
void empilhar(pilha_t *topo, tabela_t *tabela);
void desempilhar(pilha_t *topo);

void printPilha(pilha_t *topo);

#endif