#include "asd.h"
#include "instrucao.h"
#include "pilha.h"
#include "macros.h"
#include "assembly_gen.h"
#include <stdio.h>

extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *arvore = NULL;
pilha_t *pilha = NULL;

int main(int argc, char **argv)
{
  int ret = yyparse();
  if (arvore != NULL) {
    if (arvore->codigo != NULL) {
      // exporta_codigo(arvore->codigo);
      generate_asm(arvore->codigo, pilha);
      destruir_codigo(arvore->codigo);
    }
    asd_free(arvore);
  }
  if (pilha != NULL) {
    destruir_pilha(pilha);
  }
  yylex_destroy();
  
  return ret;
}
