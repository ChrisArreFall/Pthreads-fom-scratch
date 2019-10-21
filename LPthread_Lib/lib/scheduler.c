#include <unistd.h>

#include "packageList.c"
#include "lpthreads.c"
#include "../include/packageList.h"

typedef struct banda
{
    paquete paquete; //paquete que se encuentra en el segmento
    int posPaquete;
    int tamano;
    int id;
} banda;

void scheduler(list_node *derecho, list_node *izquierdo, struct banda *banda, int tamanoBanda, int schedulerType, int metodoFlujo, int w, int timeC)
{
    //primero revisamos el controlador de flujo
    printf("Iniciando el Controlador de flujo...\n");
    printf("Scheduler elegido es %d!\n", schedulerType);
    int lado = 0;
    int posInLista = 0;
    switch (metodoFlujo)
    {
    //Equidad
    case 1:
        printf("Controlador de flujo elegido es Equidad...\n");
        //como es equidad se ejecuta dos veces uno para la derecha otro para la izquierda
        while (lado < 2)
        {
            printf("Paquetes van en direccion: %d\n", lado);
            int paquetesEnviados = 0;
            //Si va de derecha a izquierda
            int wtemp = w;
            if (lado == 0)
            {
                printf("%d paquetes yendo de derecha a izquierda...\n", wtemp);
                int tamano = list_length(derecho);
                //si hay menos paquetes que wtemp entonces tenemos que desfasar
                if (tamano < wtemp)
                {
                    printf("Solo existen %d paquetes...\n", tamano);
                    wtemp = tamano;
                }
                else
                {
                    printf("Existen %d paquetes...\n", tamano);
                }
                //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                //de tamano wtemp.
                paquete *listaTemp[wtemp];
                printf("metiendo %d paquetes a una lista temporal\n", wtemp);
                list_All(derecho);
                if (schedulerType == 2)
                {
                    printf("Ordenando...\n");
                    bubbleSort(derecho, 0);
                    list_All(derecho);
                }
                if (schedulerType == 3)
                {
                    printf("Ordenando...\n");
                    bubbleSort(derecho, 1);
                    list_All(derecho);
                }

                for (int j = 0; j < wtemp; ++j)
                {
                    paquete *paqueteSeleccionado = &list_getAt(derecho, j)->data;
                    printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                    listaTemp[j] = paqueteSeleccionado;
                    printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                }
                printf("Iniciando el scheduler...\n");
                if (schedulerType == 1)
                {
                    printf("Scheduler elegido es round robin!\n");
                    //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                    //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                    //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                    //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion
                    //y cuando vuelva a ser su turno seguira de donde esta

                    //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                    posInLista = 0;
                    printf("Empieza el while.\n");
                    while (true)
                    {
                        //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                        //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                        //si la lista esta vacia, significa que ya pasaron los 3
                        if (paquetesEnviados == wtemp)
                        {
                            printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                            break;
                        }
                        //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                        int time = 0;
                        //Cambiar esto para que vaya de derecha a izquierda
                        printf("Empieza el tiempo del round robin!.\n");
                        while (time < 1000)
                        {
                            //Lthread_yield();
                            printf("Empieza el paso de paquetes!.\n");
                            //Se mueve el paquete una posicion
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            listaTemp[posInLista]->pos -= 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            //Se activa el segmento para mostrar
                            printf("Se pone el estado en 1.\n");
                            //Se le asigna el paquete
                            banda->paquete = *listaTemp[posInLista];
                            banda->posPaquete = listaTemp[posInLista]->pos;
                            int tiempoCal = 500;
                            usleep(tiempoCal * 1000);
                            time += tiempoCal;
                            if (listaTemp[posInLista]->pos == 0)
                            {
                                list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                ++paquetesEnviados;
                                printf("Paquete enviado!\n");
                                break;
                            }
                        }
                        ++posInLista;
                        if (posInLista == wtemp)
                        {
                            posInLista = 0;
                        }
                    }
                }
                else if (schedulerType == 2 || schedulerType == 3 || schedulerType == 4)
                {
                    if (schedulerType == 2)
                        printf("Scheduler elegido es prioridad!\n");
                    if (schedulerType == 3)
                        printf("Scheduler elegido es mas corto primero!\n");
                    if (schedulerType == 4)
                        printf("Scheduler elegido es fifo!\n");
                    posInLista = 0;
                    printf("Empieza el while.\n");
                    while (paquetesEnviados != wtemp)
                    {
                        printf("Empieza el paso del paquete!.\n");
                        printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                        //Pongo el primer paquete en la primera posicion de la banda
                        listaTemp[posInLista]->pos -= 1;
                        printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                        while (listaTemp[posInLista]->pos > 0)
                        {
                            //Se activa el segmento para mostrar
                            printf("Se pone el estado en 1.\n");
                            banda->paquete = *listaTemp[posInLista];
                            banda->posPaquete = listaTemp[posInLista]->pos;
                            int tiempoCal = 500;
                            usleep(tiempoCal * 1000);
                            //Se mueve el paquete una posicion
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            listaTemp[posInLista]->pos -= 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                        }
                        list_remove_by_data(&derecho, *listaTemp[posInLista]);
                        //banda[listaTemp[posInLista]->pos].paquete->estado=0;
                        ++paquetesEnviados;
                        printf("Paquete enviado!\n");
                        ++posInLista;
                    }
                    printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                }
                else if (schedulerType == 5)
                {
                    printf("Scheduler elegido es tiempo real!\n");

                    //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                    posInLista = 0;
                    printf("Empieza el while.\n");
                    while (true)
                    {

                        //si la lista esta vacia, significa que ya pasaron los 3
                        if (paquetesEnviados == wtemp)
                        {
                            printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                            break;
                        }
                        //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por timeC
                        int time = 0;
                        //Cambiar esto para que vaya de derecha a izquierda
                        printf("Empieza el tiempo del timeC!.\n");
                        while (time < timeC)
                        {
                            //Lthread_yield();
                            printf("Empieza el paso de paquetes!.\n");
                            //Se mueve el paquete una posicion
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            listaTemp[posInLista]->pos -= 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            //Se activa el segmento para mostrar
                            printf("Se pone el estado en 1.\n");
                            banda->paquete = *listaTemp[posInLista];
                            banda->posPaquete = listaTemp[posInLista]->pos;
                            int tiempoCal = 500;
                            usleep(tiempoCal * 1000);
                            time += tiempoCal;
                            if (listaTemp[posInLista]->pos == 0)
                            {
                                printf("Eliminando paquete con id %d\n", listaTemp[posInLista]->id);
                                list_All(derecho);
                                printf("---\n");
                                list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                list_All(derecho);
                                ++paquetesEnviados;
                                printf("Paquete enviado!\n");
                                break;
                            }
                        }
                        ++posInLista;
                        if (posInLista == wtemp)
                        {
                            posInLista = 0;
                        }
                    }
                }
                printf("Lista despues de enviados\n");
                list_All(derecho);
            }
            //Si va de izquierda a derecha
            else
            {
                printf("%d paquetes yendo de izquierda a derecha...\n", wtemp);
                list_All(izquierdo);
                int tamano = list_length(izquierdo);
                //si hay menos paquetes que wtemp entonces tenemos que desfasar
                if (tamano < wtemp)
                {
                    printf("Solo existen %d paquetes...\n", tamano);
                    wtemp = tamano;
                }
                else
                {
                    printf("Existen %d paquetes...\n", tamano);
                }
                //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                //de tamano wtemp.
                paquete *listaTemp[wtemp];
                printf("metiendo %d paquetes a una lista temporal\n", wtemp);
                for (int j = 0; j < wtemp; ++j)
                {
                    paquete *paqueteSeleccionado = &list_getAt(izquierdo, j)->data;
                    printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                    listaTemp[j] = paqueteSeleccionado;
                    printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                }
                switch (schedulerType)
                {
                //Round robin
                case 1:
                    printf("Scheduler elegido es round robin!\n");
                    //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                    //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                    //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                    //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion
                    //y cuando vuelva a ser su turno seguira de donde esta

                    //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                    posInLista = 0;
                    printf("Empieza el while.\n");
                    while (true)
                    {
                        //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                        //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                        //si la lista esta vacia, significa que ya pasaron los 3
                        if (paquetesEnviados == wtemp)
                        {
                            printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                            break;
                        }
                        //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                        int time = 0;
                        //Cambiar esto para que vaya de derecha a izquierda
                        printf("Empieza el tiempo del round robin!.\n");
                        while (time < 1000)
                        {
                            //Lthread_yield();
                            printf("Empieza el paso de paquetes!.\n");
                            //Se mueve el paquete una posicion
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            listaTemp[posInLista]->pos += 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            //Se activa el segmento para mostrar
                            printf("Se pone el estado en 1.\n");
                            banda->paquete = *listaTemp[posInLista];
                            banda->posPaquete = listaTemp[posInLista]->pos;
                            int tiempoCal = 500;
                            usleep(tiempoCal * 1000);
                            time += tiempoCal;
                            if (listaTemp[posInLista]->pos == (tamanoBanda - 1))
                            {
                                printf("Eliminando paquete con id %d\n", listaTemp[posInLista]->id);
                                list_All(izquierdo);
                                list_remove_by_data(&izquierdo, *listaTemp[posInLista]);

                                ++paquetesEnviados;
                                printf("Paquete enviado!\n");
                            }
                        }
                        ++posInLista;
                        if (posInLista == wtemp)
                        {
                            posInLista = 0;
                        }
                    }
                default:
                    break;
                }
                printf("Lista despues de enviados\n");
                list_All(izquierdo);
            }
            lado++;
        }
        printf("Done\n");
        break;
    //Letrero
    case 2:
        //int letrero = 0;
        int tamanoD = list_length(derecho);
        paquete *listaTempD[tamanoD];
        int tamanoI = list_length(izquierdo);
        paquete *listaTempI[tamanoI];
        while (lado < 2)
        {
            int timeL = 0;
            printf("Controlador de flujo elegido es Letrero...\n");
            //como es equidad se ejecuta dos veces uno para la derecha otro para la izquierda
            while (timeL < 5)
            {
                if (lado == 0)
                {
                    printf("Paquetes yendo de derecha a izquierda por un tiempo de 5 segundos...\n");
                    list_All(derecho);
                    //si hay menos paquetes que wtemp entonces tenemos que desfasar
                    printf("Existen %d paquetes...\n", tamanoD);
                    //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                    //de tamano wtemp.
                    printf("metiendo %d paquetes a una lista temporal\n", tamanoD);
                    for (int j = 0; j < tamanoD; ++j)
                    {
                        paquete *paqueteSeleccionado = &list_getAt(derecho, j)->data;
                        printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                        listaTemp[j] = paqueteSeleccionado;
                        printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                    }
                    printf("Iniciando el scheduler...\n");
                    if (schedulerType == 1)
                    {
                        printf("Scheduler elegido es round robin!\n");
                        //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                        //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                        //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                        //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion
                        //y cuando vuelva a ser su turno seguira de donde esta

                        //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (true)
                        {
                            //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                            //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                            //si la lista esta vacia, significa que ya pasaron los 3
                            if (paquetesEnviados == wtemp)
                            {
                                printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                break;
                            }
                            //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                            int time = 0;
                            //Cambiar esto para que vaya de derecha a izquierda
                            printf("Empieza el tiempo del round robin!.\n");
                            while (time < 1000)
                            {
                                //Lthread_yield();
                                printf("Empieza el paso de paquetes!.\n");
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                //Se le asigna el paquete
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                time += tiempoCal;
                                timeL += tiempoCal;
                                if (listaTemp[posInLista]->pos == 0)
                                {
                                    list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                    ++paquetesEnviados;
                                    printf("Paquete enviado!\n");
                                    break;
                                }
                            }
                            ++posInLista;
                            if (posInLista == wtemp)
                            {
                                posInLista = 0;
                            }
                        }
                    }
                    else if (schedulerType == 2 || schedulerType == 3 || schedulerType == 4)
                    {
                        if (schedulerType == 2)
                            printf("Scheduler elegido es prioridad!\n");
                        if (schedulerType == 3)
                            printf("Scheduler elegido es mas corto primero!\n");
                        if (schedulerType == 4)
                            printf("Scheduler elegido es fifo!\n");
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (paquetesEnviados != wtemp)
                        {
                            printf("Empieza el paso del paquete!.\n");
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            //Pongo el primer paquete en la primera posicion de la banda
                            listaTemp[posInLista]->pos -= 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            while (listaTemp[posInLista]->pos > 0)
                            {
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                timeL += tiempoCal;
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            }
                            list_remove_by_data(&derecho, *listaTemp[posInLista]);
                            //banda[listaTemp[posInLista]->pos].paquete->estado=0;
                            ++paquetesEnviados;
                            printf("Paquete enviado!\n");
                            ++posInLista;
                        }
                        printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                    }
                    else if (schedulerType == 5)
                    {
                        printf("Scheduler elegido es tiempo real!\n");

                        //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (true)
                        {

                            //si la lista esta vacia, significa que ya pasaron los 3
                            if (paquetesEnviados == wtemp)
                            {
                                printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                break;
                            }
                            //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por timeC
                            int time = 0;
                            //Cambiar esto para que vaya de derecha a izquierda
                            printf("Empieza el tiempo del timeC!.\n");
                            while (time < timeC)
                            {
                                //Lthread_yield();
                                printf("Empieza el paso de paquetes!.\n");
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                time += tiempoCal;
                                timeL += tiempoCal;
                                if (listaTemp[posInLista]->pos == 0)
                                {
                                    printf("Eliminando paquete con id %d\n", listaTemp[posInLista]->id);
                                    list_All(derecho);
                                    printf("---\n");
                                    list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                    list_All(derecho);
                                    ++paquetesEnviados;
                                    printf("Paquete enviado!\n");
                                    break;
                                }
                            }
                            ++posInLista;
                            if (posInLista == wtemp)
                            {
                                posInLista = 0;
                            }
                        }
                    }
                    printf("Lista despues de enviados\n");
                    list_All(derecho);
                }
                else
                {
                    printf("Paquetes yendo de izquierda a derecha por un tiempo de 5 segundos...\n");
                    list_All(izquierdo);
                    //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                    //de tamano wtemp.
                    printf("metiendo %d paquetes a una lista temporal\n", tamanoI);
                    for (int j = 0; j < tamanoI; ++j)
                    {
                        paquete *paqueteSeleccionado = &list_getAt(izquierdo, j)->data;
                        printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                        listaTemp[j] = paqueteSeleccionado;
                        printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                    }
                    printf("Iniciando el scheduler...\n");
                }
                ++lado;
            }
            timeL = 0;
        }
        break;
    //Aleatorio
    case 3:
        while (lado < 2)
        {
            int time = 0;
            printf("Controlador de flujo elegido es Aleatorio...\n");
            //Como es aleatorio, pasaran n paquetes donde n es un numero que es calculado aleatoriamente
            int n = (abs(rand()) % 5);
            int paquetesEnviados = 0;
            while (paquetesEnviados < n)
            {
                if (lado == 0)
                {
                    printf("%d paquetes yendo de derecha a izquierda...\n", n);
                    list_All(derecho);
                    int tamano = list_length(derecho);
                    //si hay menos paquetes que wtemp entonces tenemos que desfasar
                    if (tamano < n)
                    {
                        printf("Solo existen %d paquetes...\n", tamano);
                        n = tamano;
                    }
                    else
                    {
                        printf("Existen %d paquetes...\n", tamano);
                    }
                    //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                    //de tamano wtemp.
                    paquete *listaTemp[n];
                    printf("metiendo %d paquetes a una lista temporal\n", n);
                    for (int j = 0; j < n; ++j)
                    {
                        paquete *paqueteSeleccionado = &list_getAt(derecho, j)->data;
                        printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                        listaTemp[j] = paqueteSeleccionado;
                        printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                    }
                    printf("Iniciando el scheduler...\n");

                    printf("Iniciando el scheduler...\n");
                    if (schedulerType == 1)
                    {
                        printf("Scheduler elegido es round robin!\n");
                        //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                        //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                        //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                        //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion
                        //y cuando vuelva a ser su turno seguira de donde esta

                        //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (true)
                        {
                            //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                            //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                            //si la lista esta vacia, significa que ya pasaron los 3
                            if (paquetesEnviados == wtemp)
                            {
                                printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                break;
                            }
                            //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                            int time = 0;
                            //Cambiar esto para que vaya de derecha a izquierda
                            printf("Empieza el tiempo del round robin!.\n");
                            while (time < 1000)
                            {
                                //Lthread_yield();
                                printf("Empieza el paso de paquetes!.\n");
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                //Se le asigna el paquete
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                time += tiempoCal;
                                if (listaTemp[posInLista]->pos == 0)
                                {
                                    list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                    ++paquetesEnviados;
                                    printf("Paquete enviado!\n");
                                    break;
                                }
                            }
                            ++posInLista;
                            if (posInLista == wtemp)
                            {
                                posInLista = 0;
                            }
                        }
                    }
                    else if (schedulerType == 2 || schedulerType == 3 || schedulerType == 4)
                    {
                        if (schedulerType == 2)
                            printf("Scheduler elegido es prioridad!\n");
                        if (schedulerType == 3)
                            printf("Scheduler elegido es mas corto primero!\n");
                        if (schedulerType == 4)
                            printf("Scheduler elegido es fifo!\n");
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (paquetesEnviados != wtemp)
                        {
                            printf("Empieza el paso del paquete!.\n");
                            printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            //Pongo el primer paquete en la primera posicion de la banda
                            listaTemp[posInLista]->pos -= 1;
                            printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            while (listaTemp[posInLista]->pos > 0)
                            {
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                            }
                            list_remove_by_data(&derecho, *listaTemp[posInLista]);
                            //banda[listaTemp[posInLista]->pos].paquete->estado=0;
                            ++paquetesEnviados;
                            printf("Paquete enviado!\n");
                            ++posInLista;
                        }
                        printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                    }
                    else if (schedulerType == 5)
                    {
                        printf("Scheduler elegido es tiempo real!\n");

                        //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                        posInLista = 0;
                        printf("Empieza el while.\n");
                        while (true)
                        {

                            //si la lista esta vacia, significa que ya pasaron los 3
                            if (paquetesEnviados == wtemp)
                            {
                                printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                break;
                            }
                            //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por timeC
                            int time = 0;
                            //Cambiar esto para que vaya de derecha a izquierda
                            printf("Empieza el tiempo del timeC!.\n");
                            while (time < timeC)
                            {
                                //Lthread_yield();
                                printf("Empieza el paso de paquetes!.\n");
                                //Se mueve el paquete una posicion
                                printf("El paquete %d esta en la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                listaTemp[posInLista]->pos -= 1;
                                printf("Se mueve el paquete %d a la posicion %d.\n", listaTemp[posInLista]->id, listaTemp[posInLista]->pos);
                                //Se activa el segmento para mostrar
                                printf("Se pone el estado en 1.\n");
                                banda->paquete = *listaTemp[posInLista];
                                banda->posPaquete = listaTemp[posInLista]->pos;
                                int tiempoCal = 500;
                                usleep(tiempoCal * 1000);
                                time += tiempoCal;
                                if (listaTemp[posInLista]->pos == 0)
                                {
                                    printf("Eliminando paquete con id %d\n", listaTemp[posInLista]->id);
                                    list_All(derecho);
                                    printf("---\n");
                                    list_remove_by_data(&derecho, *listaTemp[posInLista]);
                                    list_All(derecho);
                                    ++paquetesEnviados;
                                    printf("Paquete enviado!\n");
                                    break;
                                }
                            }
                            ++posInLista;
                            if (posInLista == wtemp)
                            {
                                posInLista = 0;
                            }
                        }
                    }
                    printf("Lista despues de enviados\n");
                    list_All(derecho);
                }
                else
                {
                    printf("%d paquetes yendo de izquierda a derecha...\n", n);
                    list_All(izquierdo);
                    int tamano = list_length(izquierdo);
                    //si hay menos paquetes que wtemp entonces tenemos que desfasar
                    if (tamano < n)
                    {
                        printf("Solo existen %d paquetes...\n", tamano);
                        n = tamano;
                    }
                    else
                    {
                        printf("Existen %d paquetes...\n", tamano);
                    }
                    //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista
                    //de tamano wtemp.
                    paquete *listaTemp[n];
                    printf("metiendo %d paquetes a una lista temporal\n", n);
                    for (int j = 0; j < n; ++j)
                    {
                        paquete *paqueteSeleccionado = &list_getAt(izquierdo, j)->data;
                        printf("metiendo paquete %d con id %d\n", j, paqueteSeleccionado->id);
                        listaTemp[j] = paqueteSeleccionado;
                        printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id, j);
                    }
                    printf("Iniciando el scheduler...\n");
                }
                ++lado;
            }
        }
        break;
    default:
        break;
    }
    return;
}