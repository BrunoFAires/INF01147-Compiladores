/* Analisador Sintático - Etapa 2 e 3 */

%define parse.error verbose

%{

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;

%}

%code requires { #include "asd.h" }
// %union { asd_tree_t *tree; double value; };
%define api.value.type { asd_tree_t * }

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
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO

%%

programa: listaDeFuncao | ;

listaDeFuncao: funcaoComParametros listaDeFuncao 
|  funcaoSemParametros listaDeFuncao 
|  funcaoComParametros 
|  funcaoSemParametros
;
funcaoComParametros: TK_IDENTIFICADOR '=' parametrosFuncao '>' tipo blocoComando;
funcaoSemParametros: TK_IDENTIFICADOR '=' '>' tipo blocoComando;

parametrosFuncao: listaParametrosFuncao;
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo | TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao;

tipo: TK_PR_INT | TK_PR_FLOAT;
literal: TK_LIT_INT | TK_LIT_FLOAT;

blocoComando: '{' listaDeComandoSimples '}' | blocoComandoVazio;
blocoComandoVazio: '{' '}';

comandosSimples: var 
| blocoComando 
| condicional 
| repeticao
| atribuicao
| chamadaFuncao
| retorno
;
listaDeComandoSimples: comandosSimples';' listaDeComandoSimples | comandosSimples';';

var: tipo listaVar;
listaVar: TK_IDENTIFICADOR 
| TK_IDENTIFICADOR TK_OC_LE literal 
| TK_IDENTIFICADOR',' listaVar 
| TK_IDENTIFICADOR TK_OC_LE literal',' listaVar
;

atribuicao: TK_IDENTIFICADOR '=' expressao;

chamadaFuncao: TK_IDENTIFICADOR '(' listaArgumento ')';
listaArgumento: expressao | expressao',' listaArgumento;

retorno: TK_PR_RETURN expressao;

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
