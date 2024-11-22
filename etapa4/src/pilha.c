#include "pilha.h"

pilha_t *criarPilha()
{
    pilha_t *ret = NULL;
    ret = (pilha_t *) malloc(sizeof(pilha_t));
    if (ret != NULL) {
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
            destruirTabela(temp->tabela);
            free(temp);
            aux = aux->proximo;
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void buscar(pilha_t *topo, char *valor)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            buscarEntrada(aux->tabela, valor);
            aux = aux->proximo;
        }
    } else {
        printf("Erro: valor não encontrado (TODO: LANÇAR ERRO)\n");
    }
}

void empilhar(pilha_t *topo, tabela_t *tabela)
{
    if (topo != NULL) {
        pilha_t *nova_entrada = criarPilha();
        nova_entrada->tabela = tabela;
        nova_entrada->proximo = topo;
        topo = nova_entrada;
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void desempilhar(pilha_t *topo)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        topo = topo->proximo;
        destruirTabela(aux->tabela);
        free(aux);
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}

void printPilha(pilha_t *topo)
{
    if (topo != NULL) {
        pilha_t *aux = topo;
        while (aux != NULL) {
            printf("Tabela:\n");
            printTabela(aux->tabela);
            printf("-----------------------------------------\n");
            aux = aux->proximo;
        }
    } else {
        printf("Erro: %s recebeu parâmetro topo = %p.\n", __FUNCTION__, topo);
    }
}