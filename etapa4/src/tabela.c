#include "tabela.h"

tabela_t *criarTabela(lex_value_t *entrada)
{
    tabela_t *ret = NULL;
    ret = calloc(1, sizeof(tabela_t));
    if (ret != NULL) {
        ret->entradas = (lex_value_t **) malloc(sizeof(lex_value_t *));
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
            lex_value_free(tabela->entradas[i]);
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
            if (strcmp(tabela->entradas[i]->value, valor) == 0) {
                printf("Valor encontrado: %s\n", valor);
                return;
            }
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}

void inserirEntrada(tabela_t *tabela, lex_value_t *entrada)
{
    if (tabela != NULL && entrada != NULL) {
        tabela->num_entradas++;
        tabela->entradas = realloc(tabela->entradas, tabela->num_entradas * sizeof(lex_value_t *));   
        tabela->entradas[tabela->num_entradas-1] = entrada;
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p / entrada = %p.\n", __FUNCTION__, tabela, entrada);
    }
}

void printTabela(tabela_t *tabela)
{
    if (tabela != NULL) {
        for (int i = 0; i < tabela->num_entradas; i++) {
            char *tipo = tabela->entradas[i]->type == FLOAT ? "FLOAT" : "INT";
            char *natureza = tabela->entradas[i]->nature == FUNCTION_N ? "FUNCAO" : "IDENTIFICADOR";
            printf("Linha: %100d\tValor: %s\tTipo: %s\tNatureza: %s\n", tabela->entradas[i]->lineno, tabela->entradas[i]->value, tipo, natureza);
            free(tipo);
            free(natureza);
        }
    } else {
        printf("Erro: %s recebeu parâmetro tabela = %p.\n", __FUNCTION__, tabela);
    }
}