#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tabela.h"
#include "macros.h"
#include "errors.h"

entrada_t *criar_entrada(int linha, natureza_t natureza, simbolo_t tipo_simbolo, char *valor)
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

void destruir_entrada(entrada_t *entrada)
{
    if (entrada != NULL) {
        free(entrada->valor);
        free(entrada);
    } else {
        printf("Erro: %s recebeu parâmetro entrada = %p.\n", __FUNCTION__, entrada);
    }
}

tabela_t *criar_tabela(entrada_t *entrada)
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

tabela_t *criar_tabela_vazia()
{
    tabela_t *ret = NULL;
    ret = calloc(1, sizeof(tabela_t));
    if (ret != NULL) {
        ret->entradas = NULL;
        ret->num_entradas = 0;
    } else {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
    }

    return ret;
}

void destruir_tabela(tabela_t *tabela)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            destruir_entrada(tabela->entradas[i]);
        }
        free(tabela->entradas);
        free(tabela);
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}

entrada_t *buscar_entrada(tabela_t *tabela, char *valor)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            if (strcmp(tabela->entradas[i]->valor, valor) == 0) {
                return tabela->entradas[i];
            }
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }

    return NULL;
}

void inserir_entrada(tabela_t *tabela, entrada_t *entrada)
{
    if (tabela != NULL && entrada != NULL) {
        entrada_t *ret = buscar_entrada(tabela, entrada->valor);
        if (ret != NULL) {
            fprintf(stderr, "semantic error: identificador '%s' (%s) na linha %d já declarado na linha %d\n", entrada->valor, entrada->natureza == NAT_FUNCAO ? "função" : "variável", entrada->linha, ret->linha);
            exit(ERR_DECLARED);
        }
        tabela->num_entradas++;
        entrada->deslocamento = tabela->num_entradas; // Com as simplificações, considera-se que todas as entradas tem tamanho 1
        tabela->entradas = realloc(tabela->entradas, tabela->num_entradas * sizeof(entrada_t *));   
        tabela->entradas[tabela->num_entradas-1] = entrada;
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p / entrada = %p.\n", __FUNCTION__, tabela, entrada);
    }
}

void print_tabela(tabela_t *tabela)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            char *tipo;
            switch (tabela->entradas[i]->tipo_simbolo)
            {
                case INT:
                    tipo = "INT";
                    break;
                case FLOAT:
                    tipo = "FLOAT";
                    break;
                case PLACEHOLDER:
                    tipo = "PLACEHOLDER";
                    break;
                default:
                    tipo = "ERRO MUITO ESTRANHO :(";
                    break;
            }
            char *natureza = tabela->entradas[i]->natureza == NAT_FUNCAO ? "FUNCAO" : "IDENTIFICADOR";
            printf("Linha: %3d\tValor: %s\tTipo: %s\tNatureza: %s\n", tabela->entradas[i]->linha, tabela->entradas[i]->valor, tipo, natureza);
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}