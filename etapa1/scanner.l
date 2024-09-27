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
\/n increment_line_number();
[(a-z_)]+[0-9]* return TK_IDENTIFICADOR;
%%