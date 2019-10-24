#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <unistd.h>
#include "packageList.h"


//Segmento de una banda, el largo de la banda nos dira cuantos segmentos deberia tener la misma
typedef struct banda banda;

//scheduler(derecho, izquierdo,*banda,selectedScheduler, metodoFlujo, w);
void scheduler(int id,int fuerza,int tiempoletrero,list_node *derecho,list_node *izquierdo,struct banda* banda,int tamanoBanda ,int schedulerType,int metodoFlujo, int w, int timeC);


#endif