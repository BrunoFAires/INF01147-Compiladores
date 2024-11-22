#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"
#include "macros.h"

pilha_t *criarPilha(tabela_t *tabela)
{
    pilha_t *ret = NULL;
    ret = (pilha_t *) malloc(sizeof(pilha_t));
    if (ret != NULL) {
        ret->tabela = tabela;
        ret->proximo = NULL;
    } else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

void destruirPilha(pilha_t *topo)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            pilha_t *temp = aux;
            aux = aux->proximo;
            if (temp->tabela != NULL) destruirTabela(temp->tabela);
            free(temp);
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

// provavelmente passar entrada em vez de valor
int buscar(pilha_t *topo, char *valor)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            if (buscarEntrada(aux->tabela, valor) == 1) return 1;
            aux = aux->proximo;
        }
    } 

    return 0;
}

void empilhar(pilha_t **topo, tabela_t *tabela)
{
    if (topo != NULL && *topo != NULL) {
        pilha_t *nova_entrada = criarPilha(tabela);
        nova_entrada->proximo = *topo;
        *topo = nova_entrada;
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void desempilhar(pilha_t **topo)
{
    if (topo != NULL && *topo != NULL) {
        pilha_t *aux = *topo;
        *topo = (*topo)->proximo;
        destruirTabela(aux->tabela);
        free(aux);
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void printPilha(pilha_t *topo)
{
    if (topo != NULL) {
        printf("Pilha:\n");
        pilha_t *aux = topo;
        while (aux != NULL) {
            printf("Tabela:\n");
            printTabela(aux->tabela);
            printf("-------------------------------------------------------------\n");
            aux = aux->proximo;
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}