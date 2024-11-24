#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"
#include "macros.h"

#define COLOR_BOLD "\e[1m"
#define COLOR_OFF  "\e[m"

pilha_t *criar_pilha(tabela_t *tabela)
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

void destruir_pilha(pilha_t *topo)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            pilha_t *temp = aux;
            aux = aux->proximo;
            if (temp->tabela != NULL) destruir_tabela(temp->tabela);
            free(temp);
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

entrada_t *buscar(pilha_t *topo, char *valor)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            entrada_t *ret = buscar_entrada(aux->tabela, valor);
            if (ret != NULL) return ret;
            aux = aux->proximo;
        }
    } 

    return NULL;
}

void empilhar(pilha_t **topo, tabela_t *tabela)
{
    if (topo != NULL && *topo != NULL) {
        pilha_t *nova_entrada = criar_pilha(tabela);
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
        destruir_tabela(aux->tabela);
        free(aux);
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void print_pilha(pilha_t *topo)
{
    if (topo != NULL) {
        printf(COLOR_BOLD "Pilha:\n" COLOR_OFF);
        pilha_t *aux = topo;
        while (aux != NULL) {
            printf("Tabela:\n");
            print_tabela(aux->tabela);
            printf("-------------------------------------------------------------\n");
            aux = aux->proximo;
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}