#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <unistd.h>
#include "packageList.h"


//Segmento de una banda, el largo de la banda nos dira cuantos segmentos deberia tener la misma
typedef struct segmento segmento;

//scheduler(derecho, izquierdo,*banda,selectedScheduler, metodoFlujo, w);
void scheduler(list_node *derecho,list_node *izquierdo,struct segmento* banda,int tamanoBanda ,int schedulerType,int metodoFlujo, int w);


#endif