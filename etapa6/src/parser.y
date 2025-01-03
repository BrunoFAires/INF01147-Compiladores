/* Analisador Sintático */

%define parse.error verbose

%{

#include "pilha.h"

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
    #include "instrucao.h"
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
inicio: abreEscopoGlobal programa fechaEscopoGlobal

abreEscopoGlobal: /* vazio */ 
{ 
    tabela_t *tabela_g = criar_tabela_vazia(); 
    pilha = criar_pilha(tabela_g);  
}
fechaEscopoGlobal: /* vazio */ { /* destruir_pilha(pilha); */ }

abreEscopoFuncao: /* vazio */ 
{
    tabela_t *tabela = criar_tabela_vazia(); 
    tabela->inicio_deslocamento = 0;
    empilhar(&pilha, tabela);
}
abreEscopoInterno: /* vazio */ 
{
    tabela_t *tabela = criar_tabela_vazia();
    tabela->inicio_deslocamento = buscar_ultimo_deslocamento(pilha) + TAM_ENTRADA;
    empilhar(&pilha, tabela);

}
fechaEscopo: /* vazio */ { desempilhar(&pilha); }
 
programa: listaDeFuncao { 
    $$ = $1; arvore = $$;
    codigo_t *init_rfp = gera_codigo("loadI", RFP_START, "rfp", NULL, INDIVIDUAL, ARG_LEFT);
    concatena_codigo(init_rfp, $$->codigo);
    $$->codigo = init_rfp;
 }
| /* vazio */ { $$ = NULL; arvore = $$; }
;

listaDeFuncao: funcaoComParametros fechaEscopo listaDeFuncao 
{ 
    $$ = $1; 
    asd_add_child($$, $3); 
    if ($$->codigo != NULL) {
        concatena_codigo($$->codigo, $3->codigo);
    }
}
|  funcaoSemParametros fechaEscopo listaDeFuncao 
{ 
    $$ = $1; 
    asd_add_child($$, $3);
    if ($$->codigo != NULL) {
        concatena_codigo($$->codigo, $3->codigo);
    }
}
|  funcaoComParametros fechaEscopo { $$ = $1; }
|  funcaoSemParametros fechaEscopo { $$ = $1; }
;

funcaoComParametros: TK_IDENTIFICADOR '=' abreEscopoFuncao parametrosFuncao '>' tipo { inserir_entrada(pilha->proximo->tabela, criar_entrada($1->lineno, NAT_FUNCAO, $6->type, $1->value)); } blocoComandoFuncao
{ 
    $$ = asd_new($1->value); 
    if($8 != NULL) {
        $$->codigo = $8->codigo;
        asd_add_child($$, $8);
    }
    lex_value_free($1);
    asd_free($6);
};

funcaoSemParametros: TK_IDENTIFICADOR '=' abreEscopoFuncao '>' tipo { inserir_entrada(pilha->proximo->tabela, criar_entrada($1->lineno, NAT_FUNCAO, $5->type, $1->value)); } blocoComandoFuncao
{ 
    $$ = asd_new($1->value); 
    if($7 != NULL) {
        $$->codigo = $7->codigo;
        asd_add_child($$, $7);
    }
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

blocoComandoFuncao: '{' listaDeComandoSimples /* { LOG("PILHA FUNÇÃO"); print_pilha(pilha); } */ '}' { $$ = $2; }
| '{' /* vazio */ '}' { $$ = NULL; } // Não gera AST 
;

blocoComando: '{' abreEscopoInterno listaDeComandoSimples /* { LOG("PILHA BC"); print_pilha(pilha); } */ fechaEscopo '}' { $$ = $3; } 
| '{' abreEscopoInterno /* vazio */ fechaEscopo '}' { $$ = NULL; } // Não gera AST 
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
        if($3 != NULL) {
            concatena_codigo($$->codigo, $3->codigo);
            asd_add_child($$, $3); 
        }
    } else {
        $$ = $3;
    }
}
| var';' listaDeComandoSimples
{
    $$ = $1; 
    if ($$ != NULL) { 
        if($3 != NULL) {
            asd_tree_t *last_node = asd_get_last_node($$, MIN_NUM_CHILDREN_VAR);
            asd_add_child(last_node, $3); 
            if (last_node->codigo != NULL) {
                concatena_codigo(last_node->codigo, $3->codigo);
            } else {
                $$->codigo = $3->codigo;
            }
        }
    } else {
        $$ = $3;
    }
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
    if($5 != NULL) {
        asd_add_child($$, $5);
    }
    lex_value_free($1);
    lex_value_free($3);
}
;

atribuicao: TK_IDENTIFICADOR '=' expressao 
{
    verificar_declaracao(pilha, $1, NAT_IDENTIFICADOR);
    verificar_uso_identificador(pilha, $1);
    entrada_t *entrada = buscar(pilha, $1->value);
    $$ = asd_new("="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, $3);
    $$->type = entrada->tipo_simbolo;

    $$->codigo = $3->codigo;
    inserir_instrucao($$->codigo, gera_instrucao("storeAI", $3->local, "rfp", itoa(entrada->deslocamento), INDIVIDUAL, ARG_RIGHT));
    
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
    // Usado apenas para geração do código assembly
    $$->codigo = $2->codigo;
    inserir_instrucao($$->codigo, gera_instrucao("ret", $2->local, NULL, NULL, INDIVIDUAL, ARG_LEFT));
};

condicional: TK_PR_IF '(' expressao ')' blocoComando TK_PR_ELSE blocoComando
{
    $$ = asd_new("if"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5); 
    if ($7 != NULL) asd_add_child($$, $7);

    $$->codigo = $3->codigo;
    codigo_t *cod_true = $5 != NULL ? $5->codigo : NULL;
    codigo_t *cod_false = $7 != NULL ? $7->codigo : NULL;
    gera_codigo_fluxo_de_controle($$, $3->local, NULL, cod_true, cod_false);
}
| TK_PR_IF '(' expressao ')' blocoComando
{
    $$ = asd_new("if"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5);

    $$->codigo = $3->codigo;
    codigo_t *cod_true = $5 != NULL ? $5->codigo : NULL;
    gera_codigo_fluxo_de_controle($$, $3->local, NULL, cod_true, NULL);
}
;

repeticao: TK_PR_WHILE '(' expressao ')' blocoComando
{
    $$ = asd_new("while"); 
    asd_add_child($$, $3); 
    if ($5 != NULL) asd_add_child($$, $5);

    char *label_inicio = gera_rotulo();
    $$->codigo = gera_codigo_label(label_inicio);
    concatena_codigo($$->codigo, $3->codigo);
    codigo_t *cod_true = $5 != NULL ? $5->codigo : NULL;
    gera_codigo_fluxo_de_controle($$, $3->local, label_inicio, cod_true, NULL);
};

expressao: expressao TK_OC_OR exp1 
{
    $$ = asd_new("|"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("or", $1, $3, $$, INDIVIDUAL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp1 
{ 
    $$ = $1; 
}

exp1: exp1 TK_OC_AND exp2
{
    $$ = asd_new("&"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("and", $1, $3, $$, INDIVIDUAL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp2 
{ 
    $$ = $1;
}

exp2: exp2 TK_OC_NE exp3
{
    $$ = asd_new("!="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_NE", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp2 TK_OC_EQ exp3
{
    $$ = asd_new("=="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_EQ", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
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
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_GE", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp3 TK_OC_LE exp4
{
    $$ = asd_new("<="); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_LE", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp3 '>' exp4
{
    $$ = asd_new(">"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_GT", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
    
}
| exp3 '<' exp4
{
    $$ = asd_new("<"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("cmp_LT", $1, $3, $$, CTRL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
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
    retorno_gera_t *retorno = gera_codigo_aritmetico("sub", $1, $3, $$, INDIVIDUAL); 
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
| exp4 '+' termo
{
    $$ = asd_new("+"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("add", $1, $3, $$, INDIVIDUAL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
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
    retorno_gera_t *retorno = gera_codigo_aritmetico("div", $1, $3, $$, INDIVIDUAL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
}
|  termo '*' fator
{
    $$ = asd_new("*"); asd_add_child($$, $1); asd_add_child($$, $3);
    $$->type = inferir_tipo($1->type, $3->type);
    retorno_gera_t *retorno = gera_codigo_aritmetico("mult", $1, $3, $$, INDIVIDUAL);
    $$->local = retorno->local;
    $$->codigo = retorno->codigo;
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
    $$->codigo = $2->codigo;
    $$->local = gera_temp();

    codigo_t *cod_true = gera_codigo("loadI", "0", $$->local, NULL, INDIVIDUAL, ARG_LEFT);
    codigo_t *cod_false = gera_codigo("loadI", "1", $$->local, NULL, INDIVIDUAL, ARG_LEFT);
    gera_codigo_fluxo_de_controle($$, $2->local, NULL, cod_true, cod_false);
}
| '-' fator 
{ 
    $$ = asd_new("-"); asd_add_child($$, $2);
    $$->type = $2->type;
    $$->local = gera_temp();
    $$->codigo = $2->codigo;
    inserir_instrucao($$->codigo, gera_instrucao("multI", $2->local, "-1", $$->local, INDIVIDUAL, ARG_LEFT));
}
| '(' expressao ')' 
{   
    $$ = $2;
}
| TK_IDENTIFICADOR 
{ 
    verificar_declaracao(pilha, $1, NAT_IDENTIFICADOR); 
    verificar_uso_identificador(pilha, $1);
    entrada_t *entrada = buscar(pilha, $1->value);
    $$ = asd_new($1->value); 
    $$->type = entrada->tipo_simbolo;
    $$->local = gera_temp();     
    $$->codigo = gera_codigo("loadAI", "rfp", itoa(entrada->deslocamento), $$->local, INDIVIDUAL, ARG_LEFT);

    lex_value_free($1);
}
| TK_LIT_INT 
{ 
    $$ = asd_new($1->value);
    $$->type = INT;
    $$->local = gera_temp();
    $$->codigo = gera_codigo("loadI", $1->value, $$->local, NULL, INDIVIDUAL, ARG_LEFT);
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
