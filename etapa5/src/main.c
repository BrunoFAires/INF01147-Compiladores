#include "pilha.h"
#include "tabela.h"
#include "macros.h"

#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
pilha_t *pilha = NULL;
void exporta (void *arvore);
int main (int argc, char **argv)
{
  int ret = yyparse(); 
  yylex_destroy();
  return ret;
}
