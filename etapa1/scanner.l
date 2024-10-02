%{
    #include "../include/tokens.h"
%}

%option yylineno
minusculos [a-z]
maiusculos [A-Z]
digitos [0-9]

%%

\n      { } // new lines
\/{2}.* { } // comments
[ \t]+   { } // spaces and tabs

int     { return TK_PR_INT; }
float   { return TK_PR_FLOAT; }
if      { return TK_PR_IF; }
else    { return TK_PR_ELSE; }
while   { return TK_PR_WHILE; }
return  { return TK_PR_RETURN; }

"-"     { return (char) yytext[0]; }
"!"     { return (char) yytext[0]; }
"*"     { return (char) yytext[0]; }
"/"     { return (char) yytext[0]; }
"%"     { return (char) yytext[0]; }
"+"     { return (char) yytext[0]; }
"<"     { return (char) yytext[0]; }
">"     { return (char) yytext[0]; }
"{"     { return (char) yytext[0]; }
"}"     { return (char) yytext[0]; }
"("     { return (char) yytext[0]; }
")"     { return (char) yytext[0]; }
"="     { return (char) yytext[0]; }
","     { return (char) yytext[0]; }
";"     { return (char) yytext[0]; }

"<="    { return TK_OC_LE; }
">="    { return TK_OC_GE; }
"=="    { return TK_OC_EQ; }
"!="    { return TK_OC_NE; }
"&"    { return TK_OC_AND; }
"|"    { return TK_OC_OR; }

["_"{minusculos}]

debugging.       { return TK_ERRO; } // any other character

%%

int get_line_number(void) {
   return (int) yylineno;
} 