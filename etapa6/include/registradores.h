#ifndef _REGISTRADORES_H
#define _REGISTRADORES_H

#include <stdlib.h>

const char *REGISTRADORES[] = {"esi", "edi", "r8d", "r9d", "r10d", "r11d"}; // "ebx" "eax", "ecx", "edx"
static inline size_t num_regs() { return sizeof(REGISTRADORES) / sizeof(REGISTRADORES[0]); }

#endif // _REGISTRADORES_H