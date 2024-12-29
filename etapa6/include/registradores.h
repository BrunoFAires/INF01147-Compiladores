#ifndef _REGISTRADORES_H
#define _REGISTRADORES_H
#include "macros.h"

#include <stdlib.h>

int total_num_regs();
static inline int get_num_reg(char *reg) { return atoi(reg + 1); }
const char *mapeia_registradores(char *reg);

#endif // _REGISTRADORES_H