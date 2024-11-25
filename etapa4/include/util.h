#ifndef _UTIL_H_
#define _UTIL_H_

#include "pilha.h"
#include "lex_value.h"
#include "asd.h"

int get_line_number(void);
void increment_line_number();
void yyerror(char const *mensagem);
void exporta (void *arvore);
char *call_funcao(char *nomeFuncao);
void atribuir_tipo(tabela_t *tabela, simbolo_t type);
void verificar_uso_expressao(asd_tree_t *nodo, pilha_t *pilha);
void verificar_uso_identificador(pilha_t *pilha, char *identificador);
char *remover_prefixo_call(char *valor);
void verificar_declaracao(pilha_t *topo, lex_value_t *lex_value, natureza_t natureza);

#endif //_UTIL_H_