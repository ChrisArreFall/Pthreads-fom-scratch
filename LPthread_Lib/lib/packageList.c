#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct paquete {
	int id;
	int tipo;				//0 normal, 1 urgente, 2 radioactivo
   int prioridad;       //0-10 prioridad
	int masa;				//Se definio que es un numero entre 1 y 10 kg
	int lado;  				//0 derecha, 1 izquierda
	int estado;				//0 si aun no ha pasado al lado opuesto, 1 si ya el paquete esta listo
   int pos;
} paquete;

typedef struct list_node {
	paquete data;
	struct list_node *next;
} list_node;


paquete* getPaqueteNode(list_node *temp){
   return &temp->data;
}
/* Creates a list (node) and returns it
 * Arguments: The data the list will contain or NULL to create an empty
 * list/node
 */
list_node* list_create(paquete data){
	list_node *l = malloc(sizeof(list_node));
	if (l != NULL) {
		l->next = NULL;
		l->data = data;
	}
	return l;
}



/* Creates a list node and inserts it after the specified node
 * Arguments: A node to insert after and the data the new node will contain
 */
list_node* list_insert_after(list_node *node, paquete data)
{
	list_node *new_node = list_create(data);
	if (new_node) {
		new_node->next = node->next;
		node->next = new_node;
	}
	return new_node;
}

/* Creates a new list node and inserts it in the beginning of the list
 * Arguments: The list the node will be inserted to and the data the node will
 * contain
 */
list_node* list_insert_beginning(list_node *list, paquete data)
{
	list_node *new_node = list_create(data);
	if (new_node != NULL) { new_node->next = list; }
	return new_node;
}

/* Creates a new list node and inserts it at the end of the list
 * Arguments: The list the node will be inserted to and the data the node will
 * contain
 */
void list_insert_end(list_node *head, paquete data)
{
   list_node *new_node = list_create(data);
	if (new_node != NULL) {
		for(list_node *it = head; it != NULL; it = it->next) {
			if (it->next == NULL) {
				it->next = new_node;
            printf("El paquete %d se ha agregado.\n",it->next->data.id);
				break;
			}
		}
	}
}

/* Removes a node from the list
 * Arguments: The list and the node that will be removed
 */
void list_remove(list_node **list, list_node *node)
{
	list_node *tmp = NULL;
	if (list == NULL || *list == NULL || node == NULL) return;

	if (*list == node) {
		*list = (*list)->next;
		free(node);
		node = NULL;
	} else {
		tmp = *list;
		while (tmp->next && tmp->next != node) tmp = tmp->next;
		if (tmp->next) {
			tmp->next = node->next;
			free(node);
			node = NULL;
		}
	}
}

/* Completely destroys a list
 * Arguments: A pointer to a pointer to a list
 */
void list_destroy(list_node **list)
{
	if (list == NULL) return;
	while (*list != NULL) {
		list_remove(list, *list);
	}
}



/* Find an element in a list by the pointer to the element
 * Arguments: A pointer to a list and a pointer to the node/element
 */
list_node* list_find_node(list_node *list, list_node *node)
{
	while (list) {
		if (list == node) break;
		list = list->next;
	}
	return list;
}

/* Finds an elemt in a list by the data pointer
 * Arguments: A pointer to a list and a pointer to the data
 */
list_node* list_find_by_data(list_node *list, paquete data)
{
	while (list) {
		if (list->data.id == data.id) break;
		list = list->next;
	}
	return list;
}

/* Removes an element from a list by comparing the data pointers
 * Arguments: A pointer to a pointer to a list and the pointer to the data
 */
void list_remove_by_data(list_node **list, paquete data)
{
	if (list == NULL || *list == NULL || &data == NULL) return;
	list_remove(list, list_find_by_data(*list, data));
}

/* Finds an element in the list by using the comparison function
 * Arguments: A pointer to a list, the comparison function and a pointer to the
 * data
 */
list_node* list_find(list_node *list, int(*func)(list_node*,paquete), paquete data)
{
	if (!func) return NULL;
	while(list) {
		if (func(list, data)) break;
		list = list->next;
	}
	return list;
}


int list_length(list_node *list){
   int len = 0;
   while (1) {
		if (list->next == NULL) break;
		list = list->next;
      ++len;
	}
	return len;
}

void list_All(list_node *list){
   int len = 0;
   while (list) {
		if (list->next == NULL) break;
		list = list->next;
      printf("Elemento %d con id %d\n",len,list->data.id);
      ++len;
	}
}

list_node* list_getAt(list_node *list, int pos){
	for(int i = 0; i <= pos;++i) {
		if (list->next == NULL) break;
		list = list->next;
	}
	return list;
}