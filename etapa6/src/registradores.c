#include "registradores.h"
#include "tabela.h"

/*
    Observação: a alocação de registradores está sendo feita de uma forma beeeem preguiçosa.
    Sabemos que não foi assim que o professor nos ensinou, mas é o que temos pra hoje. É fim
    de ano e de semestre, então não temos muito tempo para implementar o algoritmo de coloração
    de grafos para fazer alocação de registradores. Então, desde já, pedimos desculpas por isso :/
*/

const char *REGISTRADORES[] = {"r8", "r9", "r10", "r11"}; // "rsi", "rdi", 
const char *mapeia_registradores(char *reg)
{ 
    int num_reg = get_num_reg(reg); 
    num_reg = num_reg % total_num_regs();
    return REGISTRADORES[num_reg]; 
}

int total_num_regs() { return sizeof(REGISTRADORES) / sizeof(REGISTRADORES[0]); }