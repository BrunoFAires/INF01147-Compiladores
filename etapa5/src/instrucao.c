#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrucao.h"
#include "macros.h"

#define LARG_MNEM 10
#define LARG_ARG  5

void destruir_lista(lista_instrucao_t *lista)
{
    if (lista != NULL) {
        for (int i = 0; i < lista->num_instrucoes; i++) {
            destruir_instrucao(lista->instrucoes[i]);
        }
        free(lista->instrucoes);
        free(lista);
    } else {
        printf("Erro: %s recebeu parâmetro lista = %p.\n", __FUNCTION__, lista);
    }
}

lista_instrucao_t *criar_lista(instrucao_t *instrucao)
{
    lista_instrucao_t *ret = NULL;
    ret = calloc(1, sizeof(lista_instrucao_t));
    if (ret != NULL) {
        ret->instrucoes = (instrucao_t **) calloc(1, sizeof(instrucao_t *));
        ret->instrucoes[0] = instrucao;
        ret->num_instrucoes = 1;
    }
    else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

lista_instrucao_t *concatenar_listas(lista_instrucao_t *lista1, lista_instrucao_t *lista2)
{
    if (lista1 != NULL && lista2 != NULL) {
        LOG("concatenando listas %p e %p", lista1, lista2);
        int novo_tamanho = lista1->num_instrucoes + lista2->num_instrucoes;
        instrucao_t **concat = (instrucao_t **) malloc(sizeof(instrucao_t *) * novo_tamanho);

        if (concat == NULL) {
            printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
            return NULL;
        }

        memcpy(concat, lista1->instrucoes, sizeof(instrucao_t *) * lista1->num_instrucoes);
        memcpy(concat + lista1->num_instrucoes, lista2->instrucoes, sizeof(instrucao_t *) * lista2->num_instrucoes);

        // Por algum motivo, dar free aqui dá erro 'free(): double free detected in tcache 2' ao liberar a lista concatenada
        // e os mnemonicos da lista concatenada ficam com lixo de memória :/
        // destruir_lista(lista1);
        // destruir_lista(lista2);

        lista_instrucao_t *ret = NULL;
        ret = (lista_instrucao_t *) calloc(1, sizeof(lista_instrucao_t));
        if (ret == NULL) {
            printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
            free(concat);
            return NULL;
        }
        ret->instrucoes = concat;
        ret->num_instrucoes = novo_tamanho;

        return ret;
    } else {
        printf("Erro: %s recebeu parâmetro lista1 = %p / lista2 = %p.\n", __FUNCTION__, lista1, lista2);
    }

    return NULL;
}

instrucao_t *gera_instrucao(char *mnem, char *arg1, char *arg2, char *arg3)
{
    instrucao_t *instrucao = NULL;
    instrucao = calloc(1, sizeof(instrucao_t));
    if(instrucao != NULL) {
        LOG("criando instrucao %s %s %s %s", mnem, arg1, arg2, arg3);
        strncpy(instrucao->mnem, mnem, MAX_LEN);
        strncpy(instrucao->arg1, arg1, MAX_LEN);
        strncpy(instrucao->arg2, arg2, MAX_LEN);
        if (arg3 != NULL)
            strncpy(instrucao->arg3, arg3, MAX_LEN);
    } else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return instrucao;
}

void destruir_instrucao(instrucao_t *instrucao)
{
     if (instrucao != NULL) {
        free(instrucao);
    } else {
        printf("Erro: %s recebeu parâmetro instrução = %p.\n", __FUNCTION__, instrucao);
    }
}

void inserir_instrucao(lista_instrucao_t *lista, instrucao_t *instrucao)
{
    if (lista != NULL && instrucao != NULL) {
        LOG("inserindo instrucao %s %s %s %s", instrucao->mnem, instrucao->arg1, instrucao->arg2, instrucao->arg3);
        lista->num_instrucoes++;
        lista->instrucoes = (instrucao_t **) realloc(lista->instrucoes, lista->num_instrucoes * sizeof(instrucao_t *));  
        lista->instrucoes[lista->num_instrucoes-1] = instrucao;
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p / instrução = %p.\n", __FUNCTION__, lista, instrucao);
    }
}

void print_codigo(lista_instrucao_t *lista) 
{
    if (lista != NULL) {
        for (int i = 0 ; i < lista->num_instrucoes; i++) {
            printf("%-*s%-*s%-*s%-*s\n", 
                    LARG_MNEM, lista->instrucoes[i]->mnem, 
                    LARG_ARG,  lista->instrucoes[i]->arg1, 
                    LARG_ARG,  lista->instrucoes[i]->arg2, 
                    LARG_ARG,  lista->instrucoes[i]->arg3);
        }
    } else {
        printf("Erro: %s recebeu parâmetro lista = %p.\n", __FUNCTION__, lista);
    }
}