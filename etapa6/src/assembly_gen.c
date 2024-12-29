#include "assembly_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela.h"
#include "registradores.h"

#define IMEDIATO 1
#define REGISTRADOR 0

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

    fprintf(stdout, "pushq %%rbp\n");
    fprintf(stdout, "movq %%rsp, %%rbp\n");

    for (int i = 1; i < codigo->num_instrucoes; i++) { // pula primeira instrução (define rfp)
        if (codigo->instrucoes[i] == NULL) {
            printf("Erro: %s recebeu parâmetro codigo->instrucoes[%d] = %p.\n", __FUNCTION__, i, codigo->instrucoes[i]);
            return;
        }
        instrucao_t *instrucao = codigo->instrucoes[i];
        if (instrucao->lbl[0] != '\0') { // labels
            fprintf(stdout, "%s:\n", instrucao->lbl);
        } else if (strcmp(instrucao->mnem, "ret") == 0) { // return
            fprintf(stdout, "movq %%%s, %%rax\n", mapeia_registradores(instrucao->arg1));
        } else if (strcmp(instrucao->mnem, "loadI") == 0) {
            fprintf(stdout, "movq $%s, %%%s\n", instrucao->arg1, mapeia_registradores(instrucao->arg2));
        } else if (strcmp(instrucao->mnem, "storeAI") == 0) {
            // arg3 é o deslocamento aqui!!
            int deslocamento = calculate_offset_asm(atoi(instrucao->arg3));
            fprintf(stdout, "movq %%%s, -%d(%%rbp)\n", mapeia_registradores(instrucao->arg1), deslocamento);
        } else if (strcmp(instrucao->mnem, "or") == 0) {
            generate_arithm_logic_asm("or", instrucao, REGISTRADOR);
        } else if (strcmp(instrucao->mnem, "and") == 0) {
            generate_arithm_logic_asm("and", instrucao, REGISTRADOR);
        } else if (strcmp(instrucao->mnem, "cmp_NE") == 0) {
            generate_comparison_asm("ne", instrucao);
        } else if (strcmp(instrucao->mnem, "cmp_EQ") == 0) {
            generate_comparison_asm("e", instrucao);
        } else if (strcmp(instrucao->mnem, "cmp_GE") == 0) {
            generate_comparison_asm("ge", instrucao);
        } else if (strcmp(instrucao->mnem, "cmp_LE") == 0) {
            generate_comparison_asm("le", instrucao);
        } else if (strcmp(instrucao->mnem, "cmp_GT") == 0) {
            generate_comparison_asm("g", instrucao);
        } else if (strcmp(instrucao->mnem, "cmp_LT") == 0) {
            generate_comparison_asm("l", instrucao);
        } else if (strcmp(instrucao->mnem, "sub") == 0) {
            generate_arithm_logic_asm("sub", instrucao, REGISTRADOR);
        } else if (strcmp(instrucao->mnem, "add") == 0) {
            generate_arithm_logic_asm("add", instrucao, REGISTRADOR);
        } else if (strcmp(instrucao->mnem, "div") == 0) {
            fprintf(stdout, "movq %%%s, %%rax\n", mapeia_registradores(instrucao->arg1));    // rax = r1 
            fprintf(stdout, "div %%%s\n", mapeia_registradores(instrucao->arg2));            // rax = rax / r2, rdx = rax % r2
            fprintf(stdout, "movq %%rax, %%%s\n", mapeia_registradores(instrucao->arg3));    // r3 = quociente
        } else if (strcmp(instrucao->mnem, "mult") == 0) {
            generate_arithm_logic_asm("imul", instrucao, REGISTRADOR);
        } else if (strcmp(instrucao->mnem, "multI") == 0) {
            generate_arithm_logic_asm("imul", instrucao, IMEDIATO);
        } else if (strcmp(instrucao->mnem, "loadAI") == 0) {
            // arg2 é o deslocamento aqui!!
            int deslocamento = calculate_offset_asm(atoi(instrucao->arg2));
            fprintf(stdout, "movq -%d(%%rbp), %%%s\n", deslocamento, mapeia_registradores(instrucao->arg3));
        } else if (strcmp(instrucao->mnem, "cbr") == 0) {
            fprintf(stdout, "test %%%s, %%%s\n", mapeia_registradores(instrucao->arg1), mapeia_registradores(instrucao->arg1));
            fprintf(stdout, "jz %s\n", instrucao->arg3);
            fprintf(stdout, "jmp %s\n", instrucao->arg2);
        } else if (strcmp(instrucao->mnem, "jumpI") == 0) {
            fprintf(stdout, "jmp %s\n", instrucao->arg1);
        } else {
            fprintf(stderr, "Erro: instrução ILOC (%s) a ser traduzida não reconhecida.", instrucao->mnem);
        }
    }

    fprintf(stdout, "popq %%rbp\n");
    fprintf(stdout, "ret\n");
    
    return;
}

void generate_arithm_logic_asm(char *mnem_asm, instrucao_t *inst, int tipo)
{
    if (tipo == REGISTRADOR) {
        const char *arg1 = mapeia_registradores(inst->arg1), *arg2 = mapeia_registradores(inst->arg2), *arg3 = mapeia_registradores(inst->arg3);
        fprintf(stdout, "%s %%%s, %%%s\n", mnem_asm, arg1, arg2);
        fprintf(stdout, "movq %%%s, %%%s\n", arg2, arg3); 
    } else if (tipo == IMEDIATO) {
        const char *arg1 = mapeia_registradores(inst->arg1), *arg3 = mapeia_registradores(inst->arg3);
        fprintf(stdout, "%s $%s, %%%s\n", mnem_asm, inst->arg2, arg1);
        fprintf(stdout, "movq %%%s, %%%s\n", arg1, arg3); 
    } else {
        fprintf(stderr, "Erro: %s recebeu parâmetro tipo = %d.\n", __FUNCTION__, tipo);
    }
}

void generate_comparison_asm(char *setx, instrucao_t *inst)
{
    const char *arg1 = mapeia_registradores(inst->arg1), *arg2 = mapeia_registradores(inst->arg2), *arg3 = mapeia_registradores(inst->arg3);
    fprintf(stdout, "cmpl %%%s, %%%s\n", arg2, arg1);
    fprintf(stdout, "set%s %%al\n", setx);
    fprintf(stdout, "movzx %%al, %%%s\n", arg3);
}