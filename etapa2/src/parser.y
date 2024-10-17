%define parse.error verbose
%{
int yylex(void);
void yyerror (char const *mensagem);
%}

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

listaDeFuncao: funcaoComParametros listaDeFuncao | funcaoSemParametros listaDeFuncao | funcaoComParametros | funcaoSemParametros
funcaoComParametros: TK_IDENTIFICADOR '=' parametrosFuncao '>' tipo blocoComando;
funcaoSemParametros: TK_IDENTIFICADOR '=' '>' tipo blocoComando;

parametrosFuncao: listaParametrosFuncao;
listaParametrosFuncao: TK_IDENTIFICADOR '<''-' tipo | TK_IDENTIFICADOR '<''-' tipo TK_OC_OR listaParametrosFuncao;

tipo: TK_PR_INT | TK_PR_FLOAT;
literal: TK_LIT_INT | TK_LIT_FLOAT

blocoComando: '{' corpoBlocoComando '}'
corpoBlocoComando: listaDeComandoSimples | ;
comandosSimples: var | blocoComando | var comandosSimples | blocoComando comandosSimples | condicional | repeticao;
listaDeComandoSimples: comandosSimples';' listaDeComandoSimples | comandosSimples';' // talvez colocar o ';' em cada comando simples que for necessário

var: tipo listaVar
listaVar: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE literal | TK_IDENTIFICADOR',' listaVar | TK_IDENTIFICADOR TK_OC_LE literal',' listaVar

/* Comandos de Controle de Fluxo: A linguagem
possui uma construção condicional e uma itera-
tiva para controle estruturado de fluxo. A con-
dicional consiste no token if seguido de uma ex-
pressão entre parênteses e então por um bloco de
comandos obrigatório. O else, sendo opcional,
deve sempre aparecer após o bloco do if, e é se-
guido de um bloco de comandos, obrigatório caso
o else seja empregado. Temos apenas uma cons-
trução de repetição que é o token while seguido
de uma expressão entre parênteses e de um bloco
de comandos. */

condicional: TK_PR_IF '(' TK_LIT_INT ')' blocoComando TK_PR_ELSE blocoComando | TK_PR_IF '(' TK_LIT_INT ')' blocoComando;
repeticao: TK_PR_WHILE '(' TK_LIT_INT ')' blocoComando;

%%