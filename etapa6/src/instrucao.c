#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instrucao.h"
#include "macros.h"
#include "asd.h"
#include "util.h"

#define LARG_MNEM 10
#define LARG_ARG 5

void destruir_codigo(codigo_t *codigo)
{
    if (codigo != NULL) {
        for (int i = 0; i < codigo->num_instrucoes; i++) {
            destruir_instrucao(codigo->instrucoes[i]);
        }
        free(codigo->instrucoes);
        free(codigo);
    }
    else
    {
        printf("Erro: %s recebeu parâmetro codigo = %p.\n", __FUNCTION__, codigo);
    }
}

codigo_t *gera_codigo(char *mnem, char *arg1, char *arg2, char *arg3, int ctrl, int r_arg)
{
    codigo_t *ret = NULL;
    ret = calloc(1, sizeof(codigo_t));
    if (ret != NULL)
    {
        ret->instrucoes = (instrucao_t **)calloc(1, sizeof(instrucao_t *));
        ret->instrucoes[0] = gera_instrucao(mnem, arg1, arg2, arg3, ctrl, r_arg);
        ret->num_instrucoes = 1;
    }
    else
    {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

codigo_t *gera_codigo_label(char *lbl)
{
    instrucao_t *inst_label = gera_instrucao_label(lbl);
    codigo_t *ret = gera_codigo(inst_label->mnem, inst_label->arg1, inst_label->arg2, inst_label->arg3, inst_label->ctrl, inst_label->r_arg);
    strncpy(ret->instrucoes[0]->lbl, inst_label->lbl, MAX_LEN);

    return ret;
}

void concatena_codigo(codigo_t *cod1, codigo_t *cod2)
{
    // Concatena código de cod2 em cod1
    if (cod1 != NULL) {
        if (cod2 == NULL) return;

        int novo_tamanho = cod1->num_instrucoes + cod2->num_instrucoes;
        cod1->instrucoes = (instrucao_t **) realloc(cod1->instrucoes, novo_tamanho * sizeof(instrucao_t *));
        if (cod1->instrucoes == NULL) {
            printf("Erro: %s não conseguiu alocar memória para vetor de instruções.\n", __FUNCTION__);
            return;
        }
        memcpy(cod1->instrucoes + cod1->num_instrucoes, cod2->instrucoes, cod2->num_instrucoes * sizeof(instrucao_t *));
        cod1->num_instrucoes = novo_tamanho;
        cod2 = NULL;
    } else {
        printf("Erro: %s recebeu parâmetro cod1 = %p.\n", __FUNCTION__, cod1);
    }
}

instrucao_t *gera_instrucao(char *mnem, char *arg1, char *arg2, char *arg3, int ctrl, int r_arg)
{
    instrucao_t *instrucao = NULL;
    instrucao = calloc(1, sizeof(instrucao_t));
    if (instrucao != NULL)
    {
        instrucao->lbl[0] = '\0';
        strncpy(instrucao->mnem, mnem, MAX_LEN);
        if (arg1 != NULL)
            strncpy(instrucao->arg1, arg1, MAX_LEN);
        if (arg2 != NULL)
            strncpy(instrucao->arg2, arg2, MAX_LEN);
        if (arg3 != NULL)
            strncpy(instrucao->arg3, arg3, MAX_LEN);
        instrucao->ctrl = ctrl;
        instrucao->r_arg = r_arg;
    }
    else
    {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return instrucao;
}

instrucao_t *gera_instrucao_label(char *lbl) 
{
    instrucao_t *instrucao = NULL;
    instrucao = calloc(1, sizeof(instrucao_t));
    if (instrucao != NULL) {
        strncpy(instrucao->lbl, lbl, MAX_LEN);
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
    if (codigo != NULL && instrucao != NULL)
    {
        codigo->num_instrucoes++;
        codigo->instrucoes = (instrucao_t **)realloc(codigo->instrucoes, codigo->num_instrucoes * sizeof(instrucao_t *));
        codigo->instrucoes[codigo->num_instrucoes - 1] = instrucao;
    }
    else
    {
        printf("Erro: %s recebeu parâmetro tabela = %p / instrução = %p.\n", __FUNCTION__, codigo, instrucao);
    }
}

void exporta_codigo(codigo_t *codigo)
{
    if (codigo != NULL) {
        for (int i = 0; i < codigo->num_instrucoes; i++) {
            exporta_instrucao(codigo->instrucoes[i]);
        }
    }
}

void exporta_instrucao(instrucao_t *inst)
{
    if (inst->lbl[0] != '\0') {
        fprintf(stdout, "%s:\n", inst->lbl);
        return;
    } else if (strcmp(inst->mnem, "ret") == 0) { // ignora ret, que só é usado para geração do assembly
        return;
    }

    fprintf(stdout, "%-*s ", LARG_MNEM, inst->mnem);
    int num_operandos = calcula_num_operandos(inst);
    const char *seta = inst->ctrl ? "->" : "=>";
    switch (num_operandos)
    {
        case 0:
            fprintf(stdout, "\n");
            break;
        case 1:
            fprintf(stdout, "%s %s\n", seta, inst->arg1);
            break;
        case 2:
            fprintf(stdout, "%s %s %s\n", inst->arg1, seta, inst->arg2);
            break;
        case 3:
            if (inst->r_arg) {
                fprintf(stdout, "%s %s %s, %s\n", inst->arg1, seta, inst->arg2, inst->arg3);
            } else {
                fprintf(stdout, "%s, %s %s %s\n", inst->arg1, inst->arg2, seta, inst->arg3);
            }
            break;
        default:
            fprintf(stderr, "Erro: número de operandos inválido (%d) em %s\n", num_operandos, inst->mnem);
            break;
        }
}

int calcula_num_operandos(instrucao_t *inst)
{
    int num_operandos = 0;
    if (inst->arg1[0] != '\0')
        num_operandos++;
    if (inst->arg2[0] != '\0')
        num_operandos++;
    if (inst->arg3[0] != '\0')
        num_operandos++;
    return num_operandos;
}

retorno_gera_t *gera_codigo_aritmetico(char *mnem, void *nodo1, void *nodo2, void *nodo3, int ctrl)
{
    asd_tree_t *nodo_asd_1 = (asd_tree_t *)nodo1;
    asd_tree_t *nodo_asd_2 = (asd_tree_t *)nodo2;
    asd_tree_t *nodo_asd_3 = (asd_tree_t *)nodo3;
    char *local = gera_temp();
    concatena_codigo(nodo_asd_1->codigo, nodo_asd_2->codigo);
    inserir_instrucao(nodo_asd_1->codigo, gera_instrucao(mnem, nodo_asd_1->local, nodo_asd_2->local, local, ctrl, ARG_LEFT));
    
    retorno_gera_t *ret = (retorno_gera_t *)malloc(sizeof(retorno_gera_t));
    if (ret == NULL)
    {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
        return NULL;
    }
    ret->codigo = nodo_asd_1->codigo;
    ret->local = local;

    return ret;
}

void gera_codigo_fluxo_de_controle(void *nodo, char *condicional, char *label_inicio, codigo_t *cod_true, codigo_t *cod_false)
{
    asd_tree_t *nodo_asd = (asd_tree_t *)nodo;
    char *lbl1 = gera_rotulo();
    char *lbl2 = gera_rotulo();
    char *lbl3 = gera_rotulo();

    inserir_instrucao(nodo_asd->codigo, gera_instrucao("cbr", condicional, lbl1, lbl2, CTRL, ARG_RIGHT));
    inserir_instrucao(nodo_asd->codigo, gera_instrucao_label(lbl1));
    concatena_codigo(nodo_asd->codigo, cod_true);
    inserir_instrucao(nodo_asd->codigo, gera_instrucao("jumpI", label_inicio == NULL ? lbl3 : label_inicio, NULL, NULL, CTRL, ARG_LEFT));
    inserir_instrucao(nodo_asd->codigo, gera_instrucao_label(lbl2));
    concatena_codigo(nodo_asd->codigo, cod_false);
    inserir_instrucao(nodo_asd->codigo, gera_instrucao("jumpI", lbl3, NULL, NULL, CTRL, ARG_LEFT));
    inserir_instrucao(nodo_asd->codigo, gera_instrucao_label(lbl3));
}