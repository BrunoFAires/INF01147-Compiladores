/* Analisador Sintático */

%define parse.error verbose

%{

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;

%}

%code requires { 
    #include "asd.h" 
    #include "lex_value.h"
}

%union {
    lex_value_t *value;
    asd_tree_t *tree;
}

// TODO: valor para tokens além de identificador e literal??
// Terminais 
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

// Não terminais
%type<tree> programa
%type<tree> listaDeFuncao
%type<tree> funcaoComParametros
%type<tree> funcaoSemParametros
%type<tree> parametrosFuncao
%type<tree> listaParametrosFuncao
%type<tree> tipo
%type<tree> blocoComando
%type<tree> blocoComandoVazio
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

programa: listaDeFuncao { $$ = $1; }
| /* vazio */ { $$ = NULL; }
;

listaDeFuncao: funcaoComParametros listaDeFuncao {}
|  funcaoSemParametros listaDeFuncao 
|  funcaoComParametros 
|  funcaoSemParametros
;
funcaoComParametros: TK_IDENTIFICADOR '=' parametrosFuncao '>' tipo blocoComando;
funcaoSemParametros: TK_IDENTIFICADOR '=' '>' tipo blocoComando;

parametrosFuncao: listaParametrosFuncao;
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo | TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao;

tipo: TK_PR_INT | TK_PR_FLOAT;
literal: TK_LIT_INT {$$ = $1;}
| TK_LIT_FLOAT {$$ = $1;};

blocoComando: '{' listaDeComandoSimples '}'  { $$ = NULL;}; //Ver se nao deveria ser $$ = $2
| blocoComandoVazio { $$ = NULL;};

blocoComandoVazio: '{' '}' { $$ = NULL;};

comandosSimples: var {$$ = $1;}
| blocoComando {$$ = $1;}
| condicional {$$ = $1;}
| repeticao {$$ = $1;}
| atribuicao {$$ = $1;}
| chamadaFuncao {$$ = $1;}
| retorno {$$ = $1;}
;
listaDeComandoSimples: comandosSimples';' listaDeComandoSimples {asd_add_child($1, $3); ; asd_print_graphviz($$); asd_free($$);};
| comandosSimples';' {$$ = $1; };

var: tipo listaVar { $$ = $2; asd_print($$); asd_print_graphviz($$); asd_free($$);}

listaVar: TK_IDENTIFICADOR { $$ = NULL; }
| TK_IDENTIFICADOR TK_OC_LE literal  { $$ = asd_new("<="); asd_add_child($$, asd_new($2->value)); asd_add_child($$, asd_new($3->value)); }
| TK_IDENTIFICADOR',' listaVar { $$ = $3; }
| TK_IDENTIFICADOR TK_OC_LE literal',' listaVar {  $$ = asd_new("<="); asd_add_child($$, asd_new($2->value)); asd_add_child($$, asd_new($3->value));  asd_add_child($$, $5);}
;

atribuicao: TK_IDENTIFICADOR '=' expressao;

chamadaFuncao: TK_IDENTIFICADOR '(' listaArgumento ')';
listaArgumento: expressao | expressao',' listaArgumento;

retorno: TK_PR_RETURN expressao { $$ = $2; };

condicional: TK_PR_IF '(' expressao ')' blocoComando TK_PR_ELSE blocoComando 
           | TK_PR_IF '(' expressao ')' blocoComando;
repeticao: TK_PR_WHILE '(' expressao ')' blocoComando;

// Quanto mais embaixo, maior a precedência (mais perto das folhas da árvore de derivação)
expressao: expressao TK_OC_OR exp1 | exp1;
exp1: exp1 TK_OC_AND exp2 | exp2;
exp2: exp2 TK_OC_NE exp3 
    | exp2 TK_OC_EQ exp3 
    | exp3
    ;
exp3: exp3 TK_OC_GE exp4 
    | exp3 TK_OC_LE exp4 
    | exp3 '>' exp4 
    | exp3 '<' exp4 
    | exp4
    ;
exp4: exp4 '-' termo 
    | exp4 '+' termo 
    | termo
    ;
termo: termo '%' fator
     |  termo '/' fator 
     |  termo '*' fator
     |  fator
     ;
fator: '!' fator
     |  '-' fator
     | '(' expressao ')'
     | TK_IDENTIFICADOR | TK_LIT_INT | TK_LIT_FLOAT | chamadaFuncao
     ;

%%
