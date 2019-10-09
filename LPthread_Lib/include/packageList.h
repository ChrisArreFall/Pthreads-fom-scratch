#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Paquete, el mismo puede ser de tres tipos distintos, puede tener una asa
typedef struct paquete {
	int id;
	int tipo;				//0 normal, 1 urgente, 2 radioactivo
	int masa;				//Se definio que es un numero entre 1 y 10 kg
	int lado;  				//0 derecha, 1 izquierda
	int estado;				//0 si aun no ha pasado al lado opuesto, 1 si ya el paquete esta listo
    int pos;
} paquete;

typedef struct node {
   paquete data;
   struct node *next;
} node;

void printList(struct node *head);
void insertFirst(int id, paquete data, struct node *head);
struct node* deleteFirst(struct node *head);
bool isEmpty(struct node *head);
int length(struct node *head);
struct node* find(int id, struct node *head);
struct node* deleteN(int id, struct node *head);
struct node* getAt(int pos, struct node *head);

#endif