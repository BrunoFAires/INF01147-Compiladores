#include "pilha.h"
#include "macros.h"

#include "instrucao.h"

#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
pilha_t *pilha = NULL;
void exporta (void *arvore);
int main (int argc, char **argv)
{
  // int ret = yyparse(); 
  // yylex_destroy();
  // return ret;

  instrucao_t *inst1 = gera_instrucao("store", "r1", "r2", NULL);
  instrucao_t *inst2 = gera_instrucao("add", "r1", "r2", "r3");
  instrucao_t *inst3 = gera_instrucao("cstoreAO", "r1", "r2", "r3");

  lista_instrucao_t *lista1 = criar_lista(inst1);
  inserir_instrucao(lista1, inst2);
  inserir_instrucao(lista1, inst3);

  print_codigo(lista1);

  instrucao_t *inst4 = gera_instrucao("store", "r1", "r2", NULL);
  instrucao_t *inst5 = gera_instrucao("sub", "r1", "r3", "r6");
  instrucao_t *inst6 = gera_instrucao("cstoreA1", "r1", "r6", "r9");
  instrucao_t *inst7 = gera_instrucao("cstore", "r0", "r6", "r9");

  lista_instrucao_t *lista2 = criar_lista(inst4);
  inserir_instrucao(lista2, inst5);
  inserir_instrucao(lista2, inst6);
  inserir_instrucao(lista2, inst7);

  print_codigo(lista2);

  lista_instrucao_t *lista3 = concatenar_listas(lista1, lista2);

  print_codigo(lista3);

  destruir_lista(lista3);

  return 0;
}
