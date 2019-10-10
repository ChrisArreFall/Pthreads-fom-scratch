#ifndef LL_H
#define LL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Paquete, el mismo puede ser de tres tipos distintos, puede tener una asa
typedef struct paquete paquete;
typedef struct node node;

paquete getPaqueteNode(struct node *temp);

void printList(struct node *head);
void insertFirst(int id, paquete data, struct node *head);
struct node* deleteFirst(struct node *head);
int isEmpty(struct node *head);
int length(struct node *head);
struct node* find(int id, struct node *head);
struct node* deleteN(int id, struct node *head);
struct node* getAt(int pos, struct node *head);

#endif