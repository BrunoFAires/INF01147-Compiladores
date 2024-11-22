#ifndef PILHA_H
#define PILHA_H

#include "tabela.h"

typedef struct Pilha
{
  Tabela tabela;
  Tabela *proximo;
} Pilha;

Pilha *criarPilha();
void destruirPilha(Pilha *topo);

void buscar(Pilha *topo, char *valor);
void empilhar(Pilha *topo, Tabela *tabela);
void desempilhar(Pilha *topo);

#endif