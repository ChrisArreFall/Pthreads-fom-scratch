#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <unistd.h>
#include "packageList.h"


//Segmento de una banda, el largo de la banda nos dira cuantos segmentos deberia tener la misma
typedef struct segmento segmento;

//scheduler(derecho, izquierdo,*banda,selectedScheduler, metodoFlujo, w);
void scheduler(struct node* derecho,struct node* izquierdo,struct segmento* banda,int tamanoBanda ,int schedulerType,int metodoFlujo, int w);
paquete roundRobin(struct node* head);
paquete priority(struct node* head);
paquete shortFirst(struct node* head);
paquete firstInFirstOut(struct node* head);
paquete realTime(struct node* head);

#endif