#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrucao.h"
#include "macros.h"
#include "asd.h"
#include "util.h"

#define LARG_MNEM 10
#define LARG_ARG  5

void destruir_codigo(codigo_t *codigo)
{
    if (codigo != NULL) {
        for (int i = 0; i < codigo->num_instrucoes; i++) {
            destruir_instrucao(codigo->instrucoes[i]);
        }
        free(codigo->instrucoes);
        free(codigo);
    } else {
        printf("Erro: %s recebeu parâmetro codigo = %p.\n", __FUNCTION__, codigo);
    }
}

codigo_t *gera_codigo(char *mnem, char *arg1, char *arg2, char *arg3)
{
    codigo_t *ret = NULL;
    ret = calloc(1, sizeof(codigo_t));
    if (ret != NULL) {
        ret->instrucoes = (instrucao_t **) calloc(1, sizeof(instrucao_t *));
        ret->instrucoes[0] = gera_instrucao(mnem, arg1, arg2, arg3);
        ret->num_instrucoes = 1;
    }
    else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

codigo_t *concatena_codigo(codigo_t *codigo1, codigo_t *codigo2)
{
    if (codigo1 != NULL && codigo2 != NULL) {
        int novo_tamanho = codigo1->num_instrucoes + codigo2->num_instrucoes;
        instrucao_t **concat = (instrucao_t **) malloc(sizeof(instrucao_t *) * novo_tamanho);
        if (concat == NULL) {
            printf("Erro: %s não conseguiu alocar memória para vetor de instruções.\n", __FUNCTION__);
            return NULL;
        }

        memcpy(concat, codigo1->instrucoes, sizeof(instrucao_t *) * codigo1->num_instrucoes);
        memcpy(concat + codigo1->num_instrucoes, codigo2->instrucoes, sizeof(instrucao_t *) * codigo2->num_instrucoes);

        codigo1 = NULL, codigo2 = NULL;

        codigo_t *ret = NULL;
        ret = (codigo_t *) calloc(1, sizeof(codigo_t));
        if (ret == NULL) {
            printf("Erro: %s não conseguiu alocar memória para retorno.\n", __FUNCTION__);
            free(concat);
            return NULL;
        }
        ret->instrucoes = concat;
        ret->num_instrucoes = novo_tamanho;

        return ret;
    } else {
        printf("Erro: %s recebeu parâmetro codigo1 = %p / codigo2 = %p.\n", __FUNCTION__, codigo1, codigo2);
    }

    return NULL;
}

instrucao_t *gera_instrucao(char *mnem, char *arg1, char *arg2, char *arg3)
{
    instrucao_t *instrucao = NULL;
    instrucao = calloc(1, sizeof(instrucao_t));
    if(instrucao != NULL) {
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

void inserir_instrucao(codigo_t *codigo, instrucao_t *instrucao)
{
    if (codigo != NULL && instrucao != NULL) {
        codigo->num_instrucoes++;
        codigo->instrucoes = (instrucao_t **) realloc(codigo->instrucoes, codigo->num_instrucoes * sizeof(instrucao_t *));  
        codigo->instrucoes[codigo->num_instrucoes-1] = instrucao;
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p / instrução = %p.\n", __FUNCTION__, codigo, instrucao);
    }
}

void exporta_codigo(codigo_t *codigo) 
{
    if (codigo != NULL) {
        for (int i = 0 ; i < codigo->num_instrucoes; i++) {
            exporta_instrucao(codigo->instrucoes[i]);
        }
    } else {
        printf("Erro: %s recebeu parâmetro codigo = %p.\n", __FUNCTION__, codigo);
    }
}

void exporta_instrucao(instrucao_t *inst) 
{
    int num_operandos = calcula_num_operandos(inst);
    switch (num_operandos)
    {
    case 1:
        fprintf(stdout, "%-*s => %s\n", 
            LARG_MNEM, inst->mnem, 
            inst->arg1);
        break;
    case 2:
        fprintf(stdout, "%-*s %s => %s\n", 
            LARG_MNEM, inst->mnem, 
            inst->arg1,
            inst->arg2);
        break;
    case 3:
        fprintf(stdout, "%-*s %s, %s => %s\n", 
            LARG_MNEM, inst->mnem, 
            inst->arg1,
            inst->arg2, 
            inst->arg3);
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

retorno_gera_t *gera_codigo_aritmetico(char *mnem, void *nodo1, void *nodo2, void *nodo3)
{
    asd_tree_t *nodo_asd_1 = (asd_tree_t *) nodo1;
    asd_tree_t *nodo_asd_2 = (asd_tree_t *) nodo2;
    asd_tree_t *nodo_asd_3 = (asd_tree_t *) nodo3;
    char *local = gera_temp();
    codigo_t *codigo = concatena_codigo(concatena_codigo(nodo_asd_1->codigo, nodo_asd_2->codigo), gera_codigo(mnem, nodo_asd_1->local, nodo_asd_2->local, local));
    retorno_gera_t *ret = (retorno_gera_t *) malloc(sizeof(retorno_gera_t));
    if (ret == NULL) {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
        return NULL;
    }
    ret->codigo = codigo;
    ret->local = local;

    return ret;
}