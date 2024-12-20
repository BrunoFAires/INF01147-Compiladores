/* Analisador Sintático */

%define parse.error verbose

%{

#include "pilha.h"
#include "errors.h"

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;
extern pilha_t *pilha;
#define MIN_NUM_CHILDREN_VAR 2

%}

%code requires { 
    #include "util.h"
    #include "asd.h" 
    #include "lex_value.h"
    #include "pilha.h"
    #include "tabela.h"
    #include "macros.h"
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
%type<tree> tipo
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

abreEscopoGlobal: /* vazio */ 
{ 
    tabela_t *tabela_g = criar_tabela_vazia(); 
    pilha = criar_pilha(tabela_g);  
}
fechaEscopoGlobal: /* vazio */ { destruir_pilha(pilha); }

abreEscopo: /* vazio */ 
{
    tabela_t *tabela = criar_tabela_vazia(); 
    empilhar(&pilha, tabela);
}
fechaEscopo: /* vazio */ { desempilhar(&pilha); }
 
programa: listaDeFuncao { $$ = $1; arvore = $$; }
| /* vazio */ { $$ = NULL; arvore = $$; }
;

listaDeFuncao: funcaoComParametros fechaEscopo listaDeFuncao { $$ = $1; asd_add_child($$, $3); }
|  funcaoSemParametros fechaEscopo listaDeFuncao { $$ = $1; asd_add_child($$, $3); }
|  funcaoComParametros fechaEscopo { $$ = $1; }
|  funcaoSemParametros fechaEscopo { $$ = $1; }
;

funcaoComParametros: TK_IDENTIFICADOR '=' abreEscopo parametrosFuncao '>' tipo { inserir_entrada(pilha->proximo->tabela, criar_entrada($1->lineno, NAT_FUNCAO, $6->type, $1->value));} blocoComandoFuncao
{ 

    $$ = asd_new($1->value); if($8 != NULL) asd_add_child($$, $8);
    lex_value_free($1);
    asd_free($6);
};
funcaoSemParametros: TK_IDENTIFICADOR '=' abreEscopo '>' tipo {inserir_entrada(pilha->proximo->tabela, criar_entrada($1->lineno, NAT_FUNCAO, $5->type, $1->value));} blocoComandoFuncao
{ 
    $$ = asd_new($1->value); if($7 != NULL) asd_add_child($$, $7);
    lex_value_free($1);
    asd_free($5);
};

tipo: TK_PR_INT { $$ = asd_new_type(INT); } | TK_PR_FLOAT { $$ = asd_new_type(FLOAT); } ;

parametrosFuncao: listaParametrosFuncao { $$ = NULL; };
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo 
{  
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, $4->type, $1->value));
    $$ = NULL;
    lex_value_free($1);
    asd_free($4);
}
| TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao 
{ 
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, $4->type, $1->value));
    $$ = NULL;
    lex_value_free($1);
    asd_free($4);
}
;

literal: TK_LIT_INT { $$ = $1; }
| TK_LIT_FLOAT { $$ = $1; }
;

blocoComandoFuncao: '{' listaDeComandoSimples '}' { $$ = $2; }
| '{' /* vazio */ fechaEscopo '}' { $$ = NULL; } // Não gera AST 
;

blocoComando: '{' abreEscopo listaDeComandoSimples fechaEscopo '}' { $$ = $3; } 
| '{' abreEscopo /* vazio */ fechaEscopo '}' { $$ = NULL; } // Não gera AST 
;

comandosSimples: blocoComando { $$ = $1; }
| condicional { $$ = $1; }
| repeticao { $$ = $1; }
| atribuicao { $$ = $1; }
| chamadaFuncao { $$ = $1; }
| retorno { $$ = $1; }
;

listaDeComandoSimples: comandosSimples';' listaDeComandoSimples
{
    $$ = $1; 
    if ($$ != NULL) { 
        if($3 != NULL) 
            asd_add_child($$, $3); 
    } else $$ = $3;
}
| var';' listaDeComandoSimples
{
    $$ = $1; 
    if ($$ != NULL) { 
        if($3 != NULL) 
            asd_add_child(asd_get_last_node($$, MIN_NUM_CHILDREN_VAR), $3); 
    } else $$ = $3;
}
| comandosSimples';'
{
    $$ = $1;
}
| var';'
{
    $$ = $1;
};

var: tipo listaVar 
{
    $$ = $2;
    atribuir_tipo(pilha->tabela, $1->type);
    asd_free($1);
}; 

listaVar: TK_IDENTIFICADOR
{ 
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, PLACEHOLDER, $1->value));
    $$ = NULL;
    lex_value_free($1);
} // Não gera AST
| TK_IDENTIFICADOR TK_OC_LE literal  
{ 
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, PLACEHOLDER, $1->value));
    $$ = asd_new("<="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, asd_new($3->value));
    lex_value_free($1);
    lex_value_free($3);
}
| TK_IDENTIFICADOR ',' listaVar 
{ 
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, PLACEHOLDER, $1->value));
    $$ = $3;
    lex_value_free($1);
}
| TK_IDENTIFICADOR TK_OC_LE literal',' listaVar 
{ 
    inserir_entrada(pilha->tabela, criar_entrada($1->lineno, NAT_IDENTIFICADOR, PLACEHOLDER, $1->value));
    $$ = asd_new("<="); 
    asd_add_child($$, asd_new($1->value)); 
    asd_add_child($$, asd_new($3->value)); 
    if($5 != NULL)
        asd_add_child($$, $5);
    lex_value_free($1);
    lex_value_free($3);
}
;

atribuicao: TK_IDENTIFICADOR '=' expressao 
{
    verificar_declaracao(pilha, $1, NAT_IDENTIFICADOR);
    verificar_uso_identificador(pilha, $1);
    $$ = asd_new("="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, $3);
    $$->type = buscar_tipo(pilha, $1->value);
    lex_value_free($1);
};

chamadaFuncao: TK_IDENTIFICADOR '(' listaArgumento ')' 
{ 
    verificar_declaracao(pilha, $1, NAT_FUNCAO);
    verificar_uso_identificador_funcao(pilha, $1);
    char *str_call_funcao = call_funcao($1->value); $$ = asd_new(str_call_funcao); asd_add_child($$, $3); free(str_call_funcao);
    lex_value_free($1);
};
listaArgumento: expressao 
{ 
    $$ = $1; 
} 
| expressao',' listaArgumento 
{ 
    $$ = $1; 
    asd_add_child($$, $3); 
}
;

retorno: TK_PR_RETURN expressao
{
    $$ = asd_new("return"); asd_add_child($$, $2);
};

condicional: TK_PR_IF '(' expressao ')' blocoComando TK_PR_ELSE blocoComando
{
    $$ = asd_new("if"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5); 
    if ($7 != NULL) asd_add_child($$, $7);
}
| TK_PR_IF '(' expressao ')' blocoComando
{
    $$ = asd_new("if"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5);
}
;

repeticao: TK_PR_WHILE '(' expressao ')' blocoComando
{
    $$ = asd_new("while"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5);
};

expressao: expressao TK_OC_OR exp1 
{
    $$ = asd_new("|"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp1 
{ 
    $$ = $1; 
}

exp1: exp1 TK_OC_AND exp2
{
    $$ = asd_new("&"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp2 
{ 
    $$ = $1;
}

exp2: exp2 TK_OC_NE exp3
{
    $$ = asd_new("!="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp2 TK_OC_EQ exp3
{
    $$ = asd_new("=="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp3 
{ 
    $$ = $1; 
}
;

exp3: exp3 TK_OC_GE exp4
{
    $$ = asd_new(">="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp3 TK_OC_LE exp4
{
    $$ = asd_new("<="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp3 '>' exp4
{
    $$ = asd_new(">"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp3 '<' exp4
{
    $$ = asd_new("<"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp4 
{ 
    $$ = $1;
}
;

exp4: exp4 '-' termo
{
    $$ = asd_new("-"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| exp4 '+' termo
{
    $$ = asd_new("+"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
| termo 
{ 
    $$ = $1;
}
;

termo:  termo '%' fator
{
    $$ = asd_new("%"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
|  termo '/' fator
{
    $$ = asd_new("/"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
|  termo '*' fator
{
    $$ = asd_new("*"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
}
|  fator 
{ 
    $$ = $1;
}
;

fator: '!' fator 
{ 
    $$ = asd_new("!"); asd_add_child($$, $2);
    $$->type = $2->type;
}
| '-' fator 
{ 
    $$ = asd_new("-"); asd_add_child($$, $2);
    $$->type = $2->type;
}
| '(' expressao ')' 
{   
    $$ = $2;
}
| TK_IDENTIFICADOR 
{ 
    verificar_declaracao(pilha, $1, NAT_IDENTIFICADOR); 
    verificar_uso_identificador(pilha, $1);
    $$ = asd_new($1->value); 
    $$->type = buscar_tipo(pilha, $1->value);
    lex_value_free($1); 
}
| TK_LIT_INT 
{ 
    $$ = asd_new($1->value);
    $$->type = INT;
    lex_value_free($1); 
}
| TK_LIT_FLOAT 
{ 
    $$ = asd_new($1->value);
    $$->type = FLOAT;
    lex_value_free($1); 
}
| chamadaFuncao 
{ 
    $$ = $1; 
}
;

%%
