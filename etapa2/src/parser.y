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

blocoComando: '{' corpoBlocoComando'}'
corpoBlocoComando: listaDeComandoSimples | ;
comandosSimples: var | blocoComando | var comandosSimples | blocoComando comandosSimples
listaDeComandoSimples: comandosSimples';' listaDeComandoSimples | comandosSimples';'

var: tipo listaVar
listaVar: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE literal | TK_IDENTIFICADOR',' listaVar | TK_IDENTIFICADOR TK_OC_LE literal',' listaVar
%%