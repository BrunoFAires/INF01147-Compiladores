#ifndef _LEX_VALUE_H_
#define _LEX_VALUE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum nature
{
  FUNCTION_N,
  IDENTIFIER_N,
  PLACEHOLDER
} nature_t;

typedef enum lex_type {
    INT,
    FLOAT,
    IDENTIFIER
} lex_type_t;

typedef struct lex_value
{
    int lineno;
    lex_type_t type;
    nature_t nature;
    char *value;
} lex_value_t;

lex_value_t *lex_value_new(int lineno, lex_type_t type, nature_t nature, char *value);
void lex_value_free(lex_value_t *value);

#endif // _LEX_VALUE_H_