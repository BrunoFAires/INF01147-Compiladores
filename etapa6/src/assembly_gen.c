#include "assembly_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela.h"

void generate_asm(codigo_t *codigo, pilha_t *pilha)
{
    if (codigo == NULL) {
        printf("Erro: %s recebeu parâmetro codigo = %p.\n", __FUNCTION__, codigo);
        return;
    }

    entrada_t *entrada = pilha->tabela->entradas[0];
    fprintf(stdout, ".globl\t%s\n", entrada->valor);
    fprintf(stdout, ".type\t%s, @%s\n", entrada->valor, entrada->natureza == NAT_FUNCAO ? "function" : "object");
    fprintf(stdout, "%s:\n", entrada->valor);

    // verificar registradores (?)

    for (int i = 0; i < codigo->num_instrucoes; i++) {
        if (codigo->instrucoes[i] == NULL) {
            printf("Erro: %s recebeu parâmetro codigo->instrucoes[%d] = %p.\n", __FUNCTION__, i, codigo->instrucoes[i]);
            return;
        }

        instrucao_t *instrucao = codigo->instrucoes[i];
        if (instrucao->lbl[0] != '\0') {
            fprintf(stdout, "%s:\n", instrucao->lbl);
        } else if (strcmp(instrucao->mnem, "ret") == 0) {
            fprintf(stdout, "movl %s, %%eax\n", instrucao->arg1);
            fprintf(stdout, "ret\n");
        }
        

    }
    
    return;
}