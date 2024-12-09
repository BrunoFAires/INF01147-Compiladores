#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "errors.h"

#define CALL "call"

int line_number = 1;
int num_rotulo = 0, num_temp = 0;

int get_line_number()
{
    return line_number;
}

void increment_line_number()
{
    line_number++;
}

void yyerror(char const *mensagem)
{
    fprintf(stderr, "%s na linha %d\n", mensagem, get_line_number());
}

void exporta(void *arvore)
{
    if (arvore == NULL)
    {
        return;
    }

    asd_tree_t *tree = (asd_tree_t *)arvore;
    fprintf(stdout, "%p [label=\"%s\"]; \n", tree, tree->label);

    int i = 0;
    for (i = 0; i < tree->number_of_children; i++)
    {
        fprintf(stdout, "%p, %p\n", tree, tree->children[i]);
        exporta(tree->children[i]);
    }

    // Libera memória ao final da exportação
    // free(tree->children);
    // tree->children = NULL;
    // free(tree->label);
    // tree->label = NULL;
    // free(tree);
    // tree = NULL;

    return;
}

char *call_funcao(char *nomeFuncao)
{
    size_t tamanhoFinal = strlen(CALL) + 1 + strlen(nomeFuncao) + 1;
    char *nomeFinal = malloc(tamanhoFinal);

    if (nomeFinal == NULL)
    {
        return NULL;
    }

    snprintf(nomeFinal, tamanhoFinal, "%s %s", CALL, nomeFuncao);

    return nomeFinal;
}

void atribuir_tipo(tabela_t *tabela, simbolo_t type)
{
    for (int i = 0; i < tabela->num_entradas; i++)
    {
        if (tabela->entradas[i]->tipo_simbolo == PLACEHOLDER)
        {
            tabela->entradas[i]->tipo_simbolo = type;
        }
    }
}

void verificar_uso_identificador(pilha_t *pilha, lex_value_t *identificador)
{
    entrada_t *entrada = buscar(pilha, identificador->value);

    if (entrada->natureza == NAT_FUNCAO)
    {
        fprintf(stderr, "semantic error: uso indevido do identificador '%s' (função) na linha %d, declarado na linha %d\n", identificador->value, identificador->lineno, entrada->linha);
        exit(ERR_FUNCTION);
    }
}

void verificar_uso_identificador_funcao(pilha_t *pilha, lex_value_t *identificador)
{
    entrada_t *entrada = buscar(pilha, identificador->value);

    if (entrada->natureza == NAT_IDENTIFICADOR)
    {
        fprintf(stderr, "semantic error: uso indevido do identificador '%s' (variável) na linha %d, declarado na linha %d\n", identificador->value, identificador->lineno, entrada->linha);
        exit(ERR_VARIABLE);
    }
}

void verificar_declaracao(pilha_t *topo, lex_value_t *lex_value, natureza_t natureza)
{
    if (buscar(topo, lex_value->value) == NULL) {
        fprintf(stderr, "semantic error: identificador '%s' (%s) na linha %d não declarado\n", lex_value->value, natureza == NAT_FUNCAO ? "função" : "variável", lex_value->lineno);
        exit(ERR_UNDECLARED);
    }
}

simbolo_t inferir_tipo(simbolo_t tipo1, simbolo_t tipo2)
{
    if (tipo1 == tipo2) return tipo1;
    if (tipo1 == FLOAT || tipo2 == FLOAT) return FLOAT;

    return PLACEHOLDER;
}

simbolo_t buscar_tipo(pilha_t *topo, char *valor)
{
    entrada_t *entrada = buscar(topo, valor);

    if (entrada == NULL) {
        return PLACEHOLDER;
    }

    return entrada->tipo_simbolo;
}

char *_gera_string(char identificador, int *num){
    int tamanho = snprintf(NULL, 0, "r%d", *num) + 1;
    char *resultado = (char *) malloc(tamanho * sizeof(char));
    if (resultado == NULL) {
        printf("Erro: %s não conseguiu alocar memória.\n", __FUNCTION__);
        return NULL;
    }
    sprintf(resultado, "%c%d", identificador, *num);

    return resultado;
}

char *gera_temp()
{
    char *temp = _gera_string('r', &num_temp);
    num_temp++;
    return temp;
}

char *gera_rotulo()
{
    char *rot = _gera_string('L', &num_rotulo);
    num_rotulo++;
    return rot;
}

void libera_se_alocado(void *ponteiro)
{
    if (ponteiro != NULL) {
        free(ponteiro);
        ponteiro = NULL;
    }
}