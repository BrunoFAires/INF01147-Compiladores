#ifndef PILHA_H
#define PILHA_H

#include "tabela.h"

typedef struct Pilha
{
    tabela_t *tabela;
    struct Pilha *proximo;
} pilha_t;

pilha_t *criar_pilha(tabela_t *tabela);
void destruir_pilha(pilha_t *topo);

entrada_t *buscar(pilha_t *topo, char *valor);
void empilhar(pilha_t **topo, tabela_t *tabela);
void desempilhar(pilha_t **topo);

void print_pilha(pilha_t *topo);

#endif