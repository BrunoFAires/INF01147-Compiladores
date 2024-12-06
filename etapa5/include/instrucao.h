#ifndef INSTRUCAO_H
#define INSTRUCAO_H

typedef struct Instrucao
{
    char *mnem;
    char *arg1;
    char *arg2;
    char *arg3;
} instrucao_t;

typedef struct ListaInstrucao
{
    instrucao_t **instrucoes;
    int num_instrucoes;
} lista_instrucao_t;


void destruir_lista(lista_instrucao_t *lista);
lista_instrucao_t *criar_lista(instrucao_t *instrucao);
lista_instrucao_t *concatenar_listas(lista_instrucao_t *lista1, lista_instrucao_t *lista2);

instrucao_t *gera_instrucao(char *mnem, char *arg1, char *arg2, char *arg3);
void destruir_instrucao(instrucao_t *instrucao);
void inserir_instrucao(lista_instrucao_t *lista, instrucao_t *instrucao);

void print_codigo(lista_instrucao_t *lista);

#endif