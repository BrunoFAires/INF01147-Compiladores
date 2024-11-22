#ifndef TABELA_H
#define TABELA_H

typedef enum Natureza
{
    NAT_IDENTIFICADOR,
    NAT_FUNCAO
} natureza_t;

typedef enum TipoSimbolo
{
    INT,
    FLOAT
} simbolo_t;

typedef struct Entrada
{
    int linha;
    natureza_t natureza;
    simbolo_t tipo_simbolo;
    char *valor;
} entrada_t;

typedef struct Tabela
{
    entrada_t **entradas;
    int num_entradas;
} tabela_t;

entrada_t *criarEntrada(int linha, natureza_t natureza, simbolo_t tipo_simbolo, char *valor);
void destruirEntrada(entrada_t *entrada);

tabela_t *criarTabela(entrada_t *entrada);
void destruirTabela(tabela_t *tabela);

int buscarEntrada(tabela_t *tabela, char *valor);
void inserirEntrada(tabela_t *tabela, entrada_t *entrada);

void printTabela(tabela_t *tabela);

#endif