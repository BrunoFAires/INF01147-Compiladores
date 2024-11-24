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
    FLOAT,
    PLACEHOLDER
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

entrada_t *criar_entrada(int linha, natureza_t natureza, simbolo_t tipo_simbolo, char *valor);
void destruir_entrada(entrada_t *entrada);

tabela_t *criar_tabela(entrada_t *entrada);
tabela_t *criar_tabela_vazia();
void destruir_tabela(tabela_t *tabela);

entrada_t *buscar_entrada(tabela_t *tabela, char *valor);
void inserir_entrada(tabela_t *tabela, entrada_t *entrada);

void print_tabela(tabela_t *tabela);

#endif