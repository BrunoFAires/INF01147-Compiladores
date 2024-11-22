#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tabela.h"

// TODO: dar free no lex_value depois de copiar dados
entrada_t *criarEntrada(int linha, natureza_t natureza, simbolo_t tipo_simbolo, char *valor)
{
    entrada_t *ret = NULL;
    ret = calloc(1, sizeof(entrada_t));
    if (ret != NULL) {
        ret->linha = linha;
        ret->natureza = natureza;
        ret->tipo_simbolo = tipo_simbolo;
        ret->valor = strdup(valor);
    } else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

void destruirEntrada(entrada_t *entrada)
{
    if (entrada != NULL) {
        free(entrada->valor);
        free(entrada);
    } else {
        printf("Erro: %s recebeu parâmetro entrada = %p.\n", __FUNCTION__, entrada);
    }
}

tabela_t *criarTabela(entrada_t *entrada)
{
    tabela_t *ret = NULL;
    ret = calloc(1, sizeof(tabela_t));
    if (ret != NULL) {
        ret->entradas = (entrada_t **) calloc(1, sizeof(entrada_t *));
        ret->entradas[0] = entrada;
        ret->num_entradas = 1;
    } else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

void destruirTabela(tabela_t *tabela)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            destruirEntrada(tabela->entradas[i]);
        }
        free(tabela->entradas);
        free(tabela);
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}

void buscarEntrada(tabela_t *tabela, char *valor)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            if (strcmp(tabela->entradas[i]->valor, valor) == 0) {
                printf("Valor encontrado: %s\n", valor);
                return;
            }
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}

void inserirEntrada(tabela_t *tabela, entrada_t *entrada)
{
    if (tabela != NULL && entrada != NULL) {
        tabela->num_entradas++;
        tabela->entradas = realloc(tabela->entradas, tabela->num_entradas * sizeof(entrada_t *));   
        tabela->entradas[tabela->num_entradas-1] = entrada;
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p / entrada = %p.\n", __FUNCTION__, tabela, entrada);
    }
}

void printTabela(tabela_t *tabela)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            char *tipo = tabela->entradas[i]->tipo_simbolo == FLOAT ? "FLOAT" : "INT";
            char *natureza = tabela->entradas[i]->natureza == NAT_FUNCAO ? "FUNCAO" : "IDENTIFICADOR";
            printf("Linha: %100d\tValor: %s\tTipo: %s\tNatureza: %s\n", tabela->entradas[i]->linha, tabela->entradas[i]->valor, tipo, natureza);
            free(tipo);
            free(natureza);
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}