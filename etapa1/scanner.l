%{
    #include "../include/tokens.h"
    int line_number = 1;
%}

%option yylineno

%%

\n // need to add space 
[//](.*)\n
[ ]

int     { line_number = yylineno; return TK_PR_INT; }
float   { line_number = yylineno; return TK_PR_FLOAT; }
if      { line_number = yylineno; return TK_PR_IF; }
else    { line_number = yylineno; return TK_PR_ELSE; }
while   { line_number = yylineno; return TK_PR_WHILE; }
return  { line_number = yylineno; return TK_PR_RETURN; }

.       { line_number = yylineno; return TK_ERRO; } // any other character

%%
