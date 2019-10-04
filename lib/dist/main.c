#include <stdio.h>
#include "dist/dist.h"
int main() {

    srand(time(NULL));

    for (int i =0; i < 200; ++i){
        CANTIDAD_PAQS cant=getCantidadPaquetes();
        printf("%d paquetes a la %s\n",cant.cantidad, (cant.lado==IZQUIERDA? "IZQUIERDA":"DERECHA"));
       // printf("%d\n",cant.cantidad);
 }

    return 0;
    return 0;
}