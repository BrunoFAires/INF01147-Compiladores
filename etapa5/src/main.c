#include "asd.h"
#include "instrucao.h"
#include "pilha.h"
#include "macros.h"

#include <stdio.h>
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *arvore = NULL;
pilha_t *pilha = NULL;
void exporta(void *arvore);
int main(int argc, char **argv)
{
  int ret = yyparse();
  // print_codigo(arvore->codigo);
  if (arvore != NULL)
    asd_free(arvore);
  yylex_destroy();
  return ret;
}
