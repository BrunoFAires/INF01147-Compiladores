/* Analisador Sintático */

%define parse.error verbose


%{

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;

%}

%code requires { 
    #include "util.h"
    #include "asd.h" 
    #include "lex_value.h"
}

%union {
    lex_value_t *value;
    asd_tree_t *tree;
}

%token<value> TK_PR_INT
%token<value> TK_PR_FLOAT
%token<value> TK_PR_IF
%token<value> TK_PR_ELSE
%token<value> TK_PR_WHILE
%token<value> TK_PR_RETURN
%token<value> TK_OC_LE
%token<value> TK_OC_GE
%token<value> TK_OC_EQ
%token<value> TK_OC_NE
%token<value> TK_OC_AND
%token<value> TK_OC_OR
%token<value> TK_IDENTIFICADOR
%token<value> TK_LIT_INT
%token<value> TK_LIT_FLOAT
%token<value> TK_ERRO
%type<value> literal
%type<value> tipo

%type<tree> programa
%type<tree> listaDeFuncao
%type<tree> funcaoComParametros
%type<tree> funcaoSemParametros
%type<tree> parametrosFuncao
%type<tree> listaParametrosFuncao
%type<tree> blocoComando
%type<tree> comandosSimples
%type<tree> listaDeComandoSimples
%type<tree> var
%type<tree> listaVar
%type<tree> atribuicao
%type<tree> chamadaFuncao
%type<tree> listaArgumento
%type<tree> retorno
%type<tree> condicional
%type<tree> repeticao
%type<tree> expressao
%type<tree> exp1
%type<tree> exp2
%type<tree> exp3
%type<tree> exp4
%type<tree> termo
%type<tree> fator

%%

programa: listaDeFuncao { $$ = $1; arvore = $$; /* asd_print_graphviz(arvore); */ }
| /* vazio */ { $$ = NULL; arvore = $$; }
;

listaDeFuncao: funcaoComParametros listaDeFuncao { $$ = $1; asd_add_child($$, $2); }
|  funcaoSemParametros listaDeFuncao { $$ = $1; asd_add_child($$, $2); }
|  funcaoComParametros { $$ = $1; }
|  funcaoSemParametros { $$ = $1; }
;
funcaoComParametros: TK_IDENTIFICADOR '=' parametrosFuncao '>' tipo blocoComando 
                     { $$ = asd_new($1->value); if($6 != NULL) asd_add_child($$, $6); };
funcaoSemParametros: TK_IDENTIFICADOR '=' '>' tipo blocoComando  
                     { $$ = asd_new($1->value); if($5 != NULL) asd_add_child($$, $5); };

tipo: TK_PR_INT | TK_PR_FLOAT { $$ = NULL; };

parametrosFuncao: listaParametrosFuncao { $$ = NULL; };
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo {  $$ = NULL; }
| TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao { $$ = NULL; }
;

literal: TK_LIT_INT { $$ = $1; }
| TK_LIT_FLOAT { $$ = $1; }
;

blocoComando: '{' listaDeComandoSimples '}'  { $$ = $2; } 
| '{' /* vazio */ '}' { $$ = NULL; } // Não gera AST 
;

comandosSimples: var { $$ = $1; }
| blocoComando { $$ = $1; }
| condicional { $$ = $1; }
| repeticao { $$ = $1; }
| atribuicao { $$ = $1; }
| chamadaFuncao { $$ = $1; }
| retorno { $$ = $1; }
;

listaDeComandoSimples: comandosSimples';' listaDeComandoSimples { $$ = $1; if($$ != NULL) asd_add_child($$, $3); else $$ = $3; }
| comandosSimples';' { $$ = $1; }
;

var: tipo listaVar { $$ = $2; };
listaVar: TK_IDENTIFICADOR { $$ = NULL; } // Não gera AST
| TK_IDENTIFICADOR TK_OC_LE literal  
{ $$ = asd_new("<="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, asd_new($3->value)); }
| TK_IDENTIFICADOR',' listaVar 
{ $$ = $3; }
| TK_IDENTIFICADOR TK_OC_LE literal',' listaVar 
{ $$ = asd_new("<="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, asd_new($3->value)); if($5 != NULL)asd_add_child($$, $5); }
;

atribuicao: TK_IDENTIFICADOR '=' expressao 
{ $$ = asd_new("="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, $3); };

chamadaFuncao: TK_IDENTIFICADOR '(' listaArgumento ')' { char *str_call_funcao = call_funcao($1->value); $$ = asd_new(str_call_funcao); asd_add_child($$, $3); free(str_call_funcao); };
listaArgumento: expressao { $$ = $1; } 
| expressao',' listaArgumento { $$ = $1; asd_add_child($$, $3); }
;

retorno: TK_PR_RETURN expressao { $$ = asd_new("return"); asd_add_child($$, $2); };

condicional: TK_PR_IF '(' expressao ')' blocoComando TK_PR_ELSE blocoComando
             { $$ = asd_new("if"); asd_add_child($$, $3); asd_add_child($$, $5); asd_add_child($$, $7); }
           | TK_PR_IF '(' expressao ')' blocoComando
             { $$ = asd_new("if"); asd_add_child($$, $3); asd_add_child($$, $5); }
           ;

repeticao: TK_PR_WHILE '(' expressao ')' blocoComando
           { $$ = asd_new("while"); asd_add_child($$, $3); asd_add_child($$, $5); };

expressao: expressao TK_OC_OR exp1 { $$ = asd_new("|"); asd_add_child($$, $1); asd_add_child($$, $3); } 
    | exp1 { $$ = $1; }
exp1: exp1 TK_OC_AND exp2 { $$ = asd_new("&"); asd_add_child($$, $1); asd_add_child($$, $3); } 
    | exp2 { $$ = $1; }
exp2: exp2 TK_OC_NE exp3 { $$ = asd_new("!="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp2 TK_OC_EQ exp3 { $$ = asd_new("=="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp3 { $$ = $1; }
    ;
exp3: exp3 TK_OC_GE exp4 { $$ = asd_new(">="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp3 TK_OC_LE exp4 { $$ = asd_new("<="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp3 '>' exp4 { $$ = asd_new(">"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp3 '<' exp4 { $$ = asd_new("<"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp4 { $$ = $1; }
    ;
exp4: exp4 '-' termo { $$ = asd_new("-"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | exp4 '+' termo { $$ = asd_new("+"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | termo { $$ = $1; }
    ;
termo:  termo '%' fator { $$ = asd_new("%"); asd_add_child($$, $1); asd_add_child($$, $3); }
     |  termo '/' fator { $$ = asd_new("/"); asd_add_child($$, $1); asd_add_child($$, $3); }
     |  termo '*' fator { $$ = asd_new("*"); asd_add_child($$, $1); asd_add_child($$, $3); }
     |  fator {$$ = $1;}
     ;
fator: '!' fator { $$ = asd_new("!"); asd_add_child($$, $2); }
     | '-' fator { $$ = asd_new("-"); asd_add_child($$, $2); }
     | '(' expressao ')' { $$ = $2; }
     | TK_IDENTIFICADOR { $$ = asd_new($1->value); }
     | TK_LIT_INT { $$ = asd_new($1->value); }
     | TK_LIT_FLOAT { $$ = asd_new($1->value); }
     | chamadaFuncao { $$ = $1; }
     ;

%%
