#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Paquete, el mismo puede ser de tres tipos distintos, puede tener una asa
typedef struct paquete paquete;
typedef struct list_node list_node;

paquete* getPaqueteNode(list_node *temp);



/* linked list */
list_node* list_create(paquete *data);
void list_destroy(list_node **list);
list_node* list_insert_after(list_node *node, paquete *data);
list_node* list_insert_beginning(list_node *list, paquete *data);
list_node* list_insert_end(list_node *list, paquete *data);
void list_remove(list_node **list, list_node *node);
void list_remove_by_data(list_node **list, paquete *data);
list_node* list_find_node(list_node *list, list_node *node);
list_node* list_find_by_data(list_node *list, paquete *data);
list_node* list_find(list_node *list, int(*func)(list_node*,void*), paquete *data);
int list_length(list_node *list);
list_node* list_getAt(list_node *list, int pos);
#endif