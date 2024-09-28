%{
	#include "tokens.h"
    #include "./include/util.h"
%}

%%
\/\/.*
int return TK_PR_INT;
float return TK_PR_FLOAT;
if return TK_PR_IF;
else return TK_PR_ELSE;
while return TK_PR_WHILE;
return return TK_PR_RETURN;
\- return 45;
\! return 33;
\* return 42;
\/ return 47;
\% return 37;
\+ return 43;
\< return 60;
\> return 62;
\{ return 123;
\} return 125;
\( return 40;
\) return 41;
\= return 61; 
\, return 44;
\; return 59;
\<= return TK_OC_LE;
\>= return TK_OC_GE;
\== return TK_OC_EQ;
\!= return TK_OC_NE;
\& return TK_OC_AND;
\| return TK_OC_OR;
\/n increment_line_number();
([(a-z_)]+[0-9]*)+ return TK_IDENTIFICADOR;
[0-9]+ return TK_LIT_INT;
[0-9]*\.[0-9]+ return TK_LIT_FLOAT;
.* return TK_ERRO;
%%

/* Todo
    - Verificar se a expressão regular que captura os erros léxicos está correto. 
        -- Da forma como está retorna erro ao inserir: "int a = 0;"(sem as aspas) por exemplo.
    - Incremento do contador de linha deve ser realizado somente ao capturar /n?
    - Criar cenários de testes para verificar a corretude das expressões regulares.
        -- Obs: nas próximas etapar podemos aplicar a metodologia de tdd.
*/