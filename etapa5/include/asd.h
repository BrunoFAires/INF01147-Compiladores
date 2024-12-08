#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "tabela.h"
#include "instrucao.h"

typedef struct asd_tree {
  char *label;
  int number_of_children;
  simbolo_t type;
  struct asd_tree **children;
  lista_instrucao_t *codigo;
  char *local;
} asd_tree_t;

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label);

asd_tree_t *asd_new_type(const simbolo_t type);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_get_last_node, retorna último nodo com filhos.
 */
asd_tree_t* asd_get_last_node(asd_tree_t *tree, int min_number_of_children);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);
#endif //_ARVORE_H_
