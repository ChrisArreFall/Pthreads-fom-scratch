//
// Created by allan on 3/10/19.
//

#ifndef DIST_DIST_H
#define DIST_DIST_H

#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef enum{
    IZQUIERDA,DERECHA
} LADO_BANDA;

typedef struct{
    int cantidad;
    LADO_BANDA lado;

} CANTIDAD_PAQS;


float square(float val){
    return val*val;
}

//funcion normal (campana gauss)
float normal(float x, float u, float sigma){
    return (float) (1.0f/sqrt(2*M_PI*square(sigma))*exp(-square(x-u)/(2.0*square(sigma))));

}
//poner -> srand(time(NULL)); en el main

float getRandom(float min, float max){
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

CANTIDAD_PAQS getCantidadPaquetes( ){
    float randVal = getRandom(-0.5f,.5f);
    CANTIDAD_PAQS cant;
    cant.lado= randVal<0 ? IZQUIERDA : DERECHA;
    cant.cantidad=(int) (5.f*normal(randVal,0,0.2f));
    if (cant.cantidad==0) cant.cantidad=1;
    return cant;
}



#endif //DIST_DIST_H
