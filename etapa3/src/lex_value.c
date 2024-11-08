#include "lex_value.h"

lex_value_t *lex_value_new(int lineno, lex_type type, char *value)
{
    lex_value_t *ret = NULL;
    ret = calloc(1, sizeof(lex_value_t));
    if (ret != NULL) {
        ret->lineno = lineno;
        ret->type = type;
        ret->value = strdup(value);
    }

    return ret;
}

void lex_value_free(lex_value_t *l_value)
{
    if (l_value != NULL) {
        free(l_value->value);
        l_value->value = NULL;
        free(l_value);
        l_value = NULL;
    } else {
        printf("Erro: %s recebeu parâmetro value = %p.\n", __FUNCTION__, l_value);
    }
}