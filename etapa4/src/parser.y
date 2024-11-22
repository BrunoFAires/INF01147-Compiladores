/* Analisador Sintático */

%define parse.error verbose

%{

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;
extern void *pilha;
#define MIN_NUM_CHILDREN_VAR 2

%}

%code requires { 
    #include "util.h"
    #include "asd.h" 
    #include "lex_value.h"
    #include "pilha.h"
    #include "tabela.h"
}

%union {
    lex_value_t *lex_value;
    asd_tree_t *tree;
}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token<lex_value> TK_IDENTIFICADOR
%token<lex_value> TK_LIT_INT
%token<lex_value> TK_LIT_FLOAT
%type<lex_value> literal
%token TK_ERRO

%type<tree> programa
%type<tree> listaDeFuncao
%type<tree> funcaoComParametros
%type<tree> funcaoSemParametros
%type<tree> parametrosFuncao
%type<tree> listaParametrosFuncao
%type<tree> blocoComando
%type<tree> blocoComandoFuncao
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
inicio: abreEscopoGlobal programa fechaEscopoGlobal //abertura do escopo global

abreEscopoGlobal: /* vazio */ { tabela_t *tabela_g = criar_tabela_vazia() ; pilha = (pilha_t*) criar_pilha(tabela_g) ; print_pilha(pilha); }
fechaEscopoGlobal: /* vazio */ { destruir_pilha(pilha); }

abreEscopo: /* vazio */ {}
fechaEscopo: /* vazio */ {}
 
programa: listaDeFuncao { $$ = $1; arvore = $$; /* asd_print_graphviz(arvore); */ }
| /* vazio */ { $$ = NULL; arvore = $$; }
;

listaDeFuncao: funcaoComParametros listaDeFuncao { $$ = $1; asd_add_child($$, $2); } //criar a lógica para adicionar as funções no escopo global
|  funcaoSemParametros listaDeFuncao { $$ = $1; asd_add_child($$, $2); }
|  funcaoComParametros { $$ = $1; }
|  funcaoSemParametros { $$ = $1; }
;

funcaoComParametros: TK_IDENTIFICADOR '=' abreEscopo parametrosFuncao '>' tipo blocoComandoFuncao //abertura do escopo da função com o fechamento do mesmo no final do bloco de comando da função; 
                     { $$ = asd_new($1->value); if($7 != NULL) asd_add_child($$, $7); };
funcaoSemParametros: TK_IDENTIFICADOR '=' abreEscopo '>' tipo blocoComandoFuncao  // abrir escopo mesmo quando não há parâmetros? Talvez não abrir aqui e usar blocoComando e não blocoComandoFuncao
                     { $$ = asd_new($1->value); if($6 != NULL) asd_add_child($$, $6); };

tipo: TK_PR_INT | TK_PR_FLOAT;

parametrosFuncao: listaParametrosFuncao { $$ = NULL; };
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo {  $$ = NULL; }
| TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao { $$ = NULL; }
;

literal: TK_LIT_INT { $$ = $1; }
| TK_LIT_FLOAT { $$ = $1; }
;

blocoComandoFuncao: '{' listaDeComandoSimples fechaEscopo '}'  { $$ = $2; } 
| '{' /* vazio */ fechaEscopo '}' { $$ = NULL; } // Não gera AST 
;

blocoComando: '{' abreEscopo listaDeComandoSimples fechaEscopo '}'  { $$ = $3; } 
| '{' /* vazio */ '}' { $$ = NULL; } // Não gera AST 
;

comandosSimples: blocoComando { $$ = $1; }
| condicional { $$ = $1; }
| repeticao { $$ = $1; }
| atribuicao { $$ = $1; }
| chamadaFuncao { $$ = $1; }
| retorno { $$ = $1; }
;

listaDeComandoSimples: comandosSimples';' listaDeComandoSimples { $$ = $1; if ($$ != NULL) { if($3 != NULL) asd_add_child($$, $3); } else $$ = $3; }
| var';' listaDeComandoSimples 
{ $$ = $1; if ($$ != NULL) { if($3 != NULL) asd_add_child(asd_get_last_node($$, MIN_NUM_CHILDREN_VAR), $3); } else $$ = $3; }
| comandosSimples';' { $$ = $1; }
| var';' { $$ = $1; }
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
             { $$ = asd_new("if"); asd_add_child($$, $3); if ($5 != NULL) asd_add_child($$, $5); if ($7 != NULL) asd_add_child($$, $7); }
           | TK_PR_IF '(' expressao ')' blocoComando
             { $$ = asd_new("if"); asd_add_child($$, $3); if ($5 != NULL) asd_add_child($$, $5); }
           ;

repeticao: TK_PR_WHILE '(' expressao ')' blocoComando
           { $$ = asd_new("while"); asd_add_child($$, $3); if ($5 != NULL) asd_add_child($$, $5); };

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
