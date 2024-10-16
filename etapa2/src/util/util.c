#include <stdio.h>

int line_number = 1;

int get_line_number(void)
{
    return line_number;
}

void increment_line_number()
{
    line_number++;
}

void yyerror(char const *mensagem)
{
    fprintf(stderr, "%s - %d\n", mensagem, get_line_number());
}