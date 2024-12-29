#ifndef _ASSEMBLY_GEN_H
#define _ASSEMBLY_GEN_H

#include "instrucao.h"
#include "pilha.h"

void generate_asm(codigo_t *codigo, pilha_t *pilha);
void generate_arithm_logic_asm(char *mnem_asm, instrucao_t *inst, int tipo);
void generate_comparison_asm(char *setx, instrucao_t *inst);

#endif // _ASSEMBLY_GEN_H