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
            printf("Erro: %s não conseguiu alocar memória para vetor de instruções.\n", __FUNCTION__);
            return NULL;
        }

        memcpy(concat, lista1->instrucoes, sizeof(instrucao_t *) * lista1->num_instrucoes);
        memcpy(concat + lista1->num_instrucoes, lista2->instrucoes, sizeof(instrucao_t *) * lista2->num_instrucoes);

        lista1 = NULL, lista2 = NULL;

        lista_instrucao_t *ret = NULL;
        ret = (lista_instrucao_t *) calloc(1, sizeof(lista_instrucao_t));
        if (ret == NULL) {
            printf("Erro: %s não conseguiu alocar memória para retorno.\n", __FUNCTION__);
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
        if (arg1 != NULL) strncpy(instrucao->arg1, arg1, MAX_LEN);
        if (arg2 != NULL) strncpy(instrucao->arg2, arg2, MAX_LEN);
        if (arg3 != NULL) strncpy(instrucao->arg3, arg3, MAX_LEN);
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
            print_instrucao(lista->instrucoes[i]);
        }
    } else {
        printf("Erro: %s recebeu parâmetro lista = %p.\n", __FUNCTION__, lista);
    }
}

void print_instrucao(instrucao_t *inst) 
{
    int num_operandos = calcula_num_operandos(inst);
    switch (num_operandos)
    {
    case 1:
        fprintf(stdout, "%-*s => %-*s\n", 
            LARG_MNEM, inst->mnem, 
            LARG_ARG,  inst->arg1);
        break;
    case 2:
        fprintf(stdout, "%-*s %-*s => %-*s\n", 
            LARG_MNEM, inst->mnem, 
            LARG_ARG,  inst->arg1,
            LARG_ARG,  inst->arg2);
        break;
    case 3:
        fprintf(stdout, "%-*s %-*s, %-*s => %-*s\n", 
            LARG_MNEM, inst->mnem, 
            LARG_ARG,  inst->arg1,
            LARG_ARG,  inst->arg2, 
            LARG_ARG,  inst->arg3);
        break;
    default:
        break;
    }    
}

int calcula_num_operandos(instrucao_t *inst)
{
    int num_operandos = 0;
    if (inst->arg1[0] != '\0') num_operandos++;
    if (inst->arg2[0] != '\0') num_operandos++;
    if (inst->arg3[0] != '\0') num_operandos++;
    return num_operandos;
}