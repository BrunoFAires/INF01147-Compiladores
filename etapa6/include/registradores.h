#ifndef _REGISTRADORES_H
#define _REGISTRADORES_H
#include "macros.h"

#include <stdlib.h>

const char *REGISTRADORES[] = {"rsi", "rdi", "r8", "r9", "r10", "r11"}; // "ebx" "eax", "ecx", "edx"
static inline int num_regs();
static inline int num_reg(char *reg) { return atoi(reg + 1); }
const static inline char *mapeia_registradores(char *reg) { return REGISTRADORES[num_reg(reg)]; }

#endif // _REGISTRADORES_H