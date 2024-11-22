#ifndef _UTIL_H_
#define _UTIL_H_

int get_line_number(void);
void increment_line_number();
void yyerror(char const *mensagem);
void exporta (void *arvore);
char *call_funcao(char *nomeFuncao);

#endif //_UTIL_H_