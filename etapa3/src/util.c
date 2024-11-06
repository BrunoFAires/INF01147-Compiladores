#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CALL "call"

int line_number = 1;

int get_line_number()
{
    return line_number;
}

void increment_line_number()
{
    line_number++;
}

void yyerror(char const *mensagem)
{
    fprintf(stderr, "%s at line %d\n", mensagem, get_line_number());
}

void exporta(void *arvore)
{
    // Implementação futura
    return;
}

char *nome_funcao(char *nomeFuncao)
{
    size_t tamanhoFinal = strlen(CALL) + 1 + strlen(nomeFuncao) + 1;
    // TODO cuidar leak
    char *nomeFinal = malloc(tamanhoFinal);

    if (nomeFinal == NULL) {
        return NULL;
    }

    snprintf(nomeFinal, tamanhoFinal, "%s %s", CALL, nomeFuncao);

    return nomeFinal;
}
