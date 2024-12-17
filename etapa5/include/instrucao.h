/* Instruções seguem a linguagem ILOC */

#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#define MAX_LEN 50
#define CTRL 1
#define INDIVIDUAL 0
#define ARG_RIGHT 1
#define ARG_LEFT 0

typedef struct Instrucao
{
    char lbl[MAX_LEN];
    char mnem[MAX_LEN];
    char arg1[MAX_LEN];
    char arg2[MAX_LEN];
    char arg3[MAX_LEN];
    int ctrl;
    int r_arg;
} instrucao_t;

typedef struct Codigo
{
    instrucao_t **instrucoes;
    int num_instrucoes;
} codigo_t;

typedef struct RetornoGera
{
    codigo_t *codigo;
    char *local;
} retorno_gera_t;

void destruir_codigo(codigo_t *codigo);
codigo_t *gera_codigo(char *mnem, char *arg1, char *arg2, char *arg3, int ctrl, int r_arg);
codigo_t *gera_codigo_label(char *lbl);
void concatena_codigo(codigo_t *codigo1, codigo_t *codigo2);

instrucao_t *gera_instrucao(char *mnem, char *arg1, char *arg2, char *arg3, int ctrl, int r_arg);
instrucao_t *gera_instrucao_label(char *lbl);
void destruir_instrucao(instrucao_t *instrucao);
void inserir_instrucao(codigo_t *codigo, instrucao_t *instrucao);

int calcula_num_operandos(instrucao_t *inst);
void exporta_instrucao(instrucao_t *inst);
void exporta_codigo(codigo_t *codigo);

retorno_gera_t *gera_codigo_aritmetico(char *mnem, void *nodo1, void *nodo2, void *nodo3, int ctrl);
void gera_codigo_fluxo_de_controle(void *nodo, char *condicional, char *label_inicio, codigo_t *cod_true, codigo_t *cod_false);

#endif