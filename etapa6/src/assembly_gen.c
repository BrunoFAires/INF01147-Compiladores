#include "assembly_gen.h"
#include <stdio.h>
#include <stdlib.h>

void generate_asm(codigo_t *codigo)
{
    if (codigo == NULL) {
        printf("Erro: %s recebeu parâmetro codigo = %p.\n", __FUNCTION__, codigo);
        return;
    }
    
    return;
}