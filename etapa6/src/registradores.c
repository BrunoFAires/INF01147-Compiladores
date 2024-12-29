#include "registradores.h"
#include "tabela.h"

const char *REGISTRADORES[] = {"rsi", "rdi", "r8", "r9", "r10", "r11"}; // "ebx" "eax", "ecx", "edx"

const char *mapeia_registradores(char *reg)
{ 
    int num_reg = get_num_reg(reg); 
    num_reg = num_reg < total_num_regs() ? num_reg : 0;
    return REGISTRADORES[num_reg]; 
}

int total_num_regs() { return sizeof(REGISTRADORES) / sizeof(REGISTRADORES[0]); }