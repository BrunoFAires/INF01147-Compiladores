#ifndef _UTIL_H_
#define _UTIL_H_

#include "pilha.h"
#include "tabela.h"
#include "lex_value.h"

int get_line_number(void);
void increment_line_number();
void yyerror(char const *mensagem);
void exporta (void *arvore);
char *call_funcao(char *nomeFuncao);
// Verifica se identificador foi declarado na pilha, encerrando com erro caso não tenha sido
void verificar_declaracao(pilha_t *topo, lex_value_t *lex_value, natureza_t natureza);
// Verifica se identificador foi declarado na tabela, encerrando com erro caso tenha sido
void verificar_dupla_declaracao(tabela_t *tabela, lex_value_t *lex_value, natureza_t natureza);

#endif //_UTIL_H_