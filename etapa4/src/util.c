#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "asd.h"
#include "errors.h"

#define CALL "call"

int line_number = 1;

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
    if (arvore == NULL) {
        return;
    }
    
    asd_tree_t *tree = (asd_tree_t *) arvore;
    fprintf(stdout, "%p [label=\"%s\"]; \n", tree, tree->label);
    
    int i = 0;
    for (i = 0; i < tree->number_of_children; i++) {
        fprintf(stdout, "%p, %p\n", tree, tree->children[i]);
        exporta(tree->children[i]);
    }

    // Libera memória ao final da exportação
    free(tree->children);
    tree->children = NULL;
    free(tree->label);
    tree->label = NULL;
    free(tree);
    tree = NULL;

    return;
}

char *call_funcao(char *nomeFuncao)
{
    size_t tamanhoFinal = strlen(CALL) + 1 + strlen(nomeFuncao) + 1;
    char *nomeFinal = malloc(tamanhoFinal);

    if (nomeFinal == NULL) {
        return NULL;
    }

    snprintf(nomeFinal, tamanhoFinal, "%s %s", CALL, nomeFuncao); // qual é o motivo disso?

    return nomeFinal;
}

void verificar_declaracao(pilha_t *topo, lex_value_t *lex_value, natureza_t natureza)
{
    if (buscar(topo, lex_value->value) == NULL) {
        fprintf(stderr, "semantic error: identificador %s (%s) na linha %d não declarado\n", lex_value->value, natureza == NAT_FUNCAO ? "função" : "variável", lex_value->lineno);
        exit(ERR_UNDECLARED);
    }
}

void verificar_dupla_declaracao(tabela_t *tabela, lex_value_t *lex_value, natureza_t natureza)
{
    entrada_t *ret = buscar_entrada(tabela, lex_value->value);
    if (ret != NULL) {
        fprintf(stderr, "semantic error: identificador %s (%s) na linha %d já declarado na linha %d\n", lex_value->value, natureza == NAT_FUNCAO ? "função" : "variável", lex_value->lineno, ret->linha);
        exit(ERR_DECLARED);
    }
}
