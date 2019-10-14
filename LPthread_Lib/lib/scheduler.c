#include <unistd.h>

#include "packageList.c"
#include "lpthreads.c"
#include "../include/packageList.h"
typedef struct segmento
{
	paquete* paquete;		//paquete que se encuentra en el segmento
	int estado;				//nos dice si hay algo en el segmento, esto seria para desplegar en la gui
} segmento;

void scheduler(list_node *derecho,list_node *izquierdo,struct segmento *banda,int tamanoBanda,int schedulerType,int metodoFlujo, int w){
    //primero revisamos el controlador de flujo
    printf("Iniciando el scheduler...\n");
    int lado = 0;
    switch (metodoFlujo){
        //Equidad
        case 1:
            printf("Controlador de flujo elegido es Equidad...\n");
            //como es equidad se ejecuta dos veces uno para la derecha otro para la izquierda
            for(int i=0;i<2;++i){
                printf("Paquetes van en direccion: %d\n",i);
                switch (schedulerType){
                    //Round robin
                    case 1:
                        printf("Se esta usando el scheduler de round robin...\n");
                        int paquetesEnviados = 0;
                        //Si va de derecha a izquierda
                        int wtemp = w;
                        if(lado == 0){
                            printf("%d paquetes yendo de derecha a izquierda...\n",wtemp);
                            list_All(derecho);
                            int tamano = list_length(derecho);
                            //si hay menos paquetes que wtemp entonces tenemos que desfasar 
                            if(tamano<wtemp){
                                printf("Solo existen %d paquetes...\n",tamano);
                                wtemp = tamano;
                            }
                            else{
                                printf("Existen %d paquetes...\n",tamano);
                            }
                            //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista  
                            //de tamano wtemp.
                            paquete* listaTemp[wtemp];
                            printf("metiendo %d paquetes a una lista temporal\n",wtemp);
                            for(int j = 0;j<wtemp;++j){
                                paquete* paqueteSeleccionado = &list_getAt(derecho, j)->data;
                                printf("metiendo paquete %d con id %d\n",j,paqueteSeleccionado->id);
                                listaTemp[j] = paqueteSeleccionado;
                                printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id,j);
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                            int posInLista = 0;
                            printf("Empieza el while.\n");
                            while(true){
                                //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                                //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                                //si la lista esta vacia, significa que ya pasaron los 3
                                if(paquetesEnviados==wtemp){
                                    printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                    break;
                                }
                                //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                                int time = 0;
                                //Cambiar esto para que vaya de derecha a izquierda
                                printf("Empieza el tiempo del round robin!.\n");
                                while(time<1000){
                                    //Lthread_yield();
                                    printf("Empieza el paso de paquetes!.\n");
                                    //Se mueve el paquete una posicion
                                    printf("El paquete %d esta en la posicion %d.\n",listaTemp[posInLista]->id,listaTemp[posInLista]->pos);
                                    listaTemp[posInLista]->pos-=1;
                                    printf("Se mueve el paquete %d a la posicion %d.\n",listaTemp[posInLista]->id,listaTemp[posInLista]->pos);
                                    //Se activa el segmento para mostrar
                                    printf("Se pone el estado en 1.\n");
                                    banda[listaTemp[posInLista]->pos].estado=1;
                                    //Se le asigna el paquete
                                    banda[listaTemp[posInLista]->pos].paquete=listaTemp[posInLista];
                                    int tiempoCal = 500;
                                    usleep(tiempoCal*1000);
                                    time+=tiempoCal;
                                    if(listaTemp[posInLista]->pos==0){
                                        list_remove_by_data(&derecho,*listaTemp[posInLista]);
                                        banda[listaTemp[posInLista]->pos].paquete->estado=0;
                                        ++paquetesEnviados;
                                        printf("Paquete enviado!\n");
                                    }
                                }
                                ++posInLista;
                                if(posInLista==wtemp){
                                    posInLista = 0;
                                }
                            }
                            printf("Lista despues de enviados\n");
                            list_All(derecho);
                            lado = 1;
                        }
                        //Si va de izquierda a derecha
                        else{
                            wtemp = w;
                            printf("%d paquetes yendo de izquierda a derecha...\n",wtemp);
                            list_All(izquierdo);
                            int tamano = list_length(izquierdo);
                            //si hay menos paquetes que wtemp entonces tenemos que desfasar 
                            if(tamano<wtemp){
                                printf("Solo existen %d paquetes...\n",tamano);
                                wtemp = tamano;
                            }
                            else{
                                printf("Existen %d paquetes...\n",tamano);
                            }
                            //Como esto es equidad y hay que pasar wtemp paquetes, entonces agrego los paquetes a una lista  
                            //de tamano wtemp.
                            paquete* listaTemp[wtemp];
                            printf("metiendo %d paquetes a una lista temporal\n",wtemp);
                            for(int j = 0;j<wtemp;++j){
                                paquete* paqueteSeleccionado = &list_getAt(izquierdo, j)->data;
                                printf("metiendo paquete %d con id %d\n",j,paqueteSeleccionado->id);
                                listaTemp[j] = paqueteSeleccionado;
                                printf("Se ha metido exitosamente con el id %d en la posicion %d\n", listaTemp[j]->id,j);
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                            int posInLista = 0;
                            printf("Empieza el while.\n");
                            while(true){
                                //ahora tenemos una lista de tamano wtemp que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros wtemp
                                //paquetes de la lista enlazada y por ser equidad se la lista es de tamano wtemp
                                //si la lista esta vacia, significa que ya pasaron los 3
                                if(paquetesEnviados==wtemp){
                                    printf("Ya se enviaron la cantidad de paquetes requeridos!.\n");
                                    break;
                                }
                                //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                                int time = 0;
                                //Cambiar esto para que vaya de derecha a izquierda
                                printf("Empieza el tiempo del round robin!.\n");
                                while(time<1000){
                                    //Lthread_yield();
                                    printf("Empieza el paso de paquetes!.\n");
                                    //Se mueve el paquete una posicion
                                    printf("El paquete %d esta en la posicion %d.\n",listaTemp[posInLista]->id,listaTemp[posInLista]->pos);
                                    listaTemp[posInLista]->pos+=1;
                                    printf("Se mueve el paquete %d a la posicion %d.\n",listaTemp[posInLista]->id,listaTemp[posInLista]->pos);
                                    //Se activa el segmento para mostrar
                                    printf("Se pone el estado en 1.\n");
                                    banda[listaTemp[posInLista]->pos].estado=1;
                                    //Se le asigna el paquete
                                    banda[listaTemp[posInLista]->pos].paquete=listaTemp[posInLista];
                                    int tiempoCal = 500;
                                    usleep(tiempoCal*1000);
                                    time+=tiempoCal;
                                    if(listaTemp[posInLista]->pos==(tamanoBanda-1)){
                                        list_remove_by_data(&izquierdo,*listaTemp[posInLista]);
                                        banda[listaTemp[posInLista]->pos].paquete->estado=0;
                                        ++paquetesEnviados;
                                        printf("Paquete enviado!\n");
                                    }
                                }
                                ++posInLista;
                                if(posInLista==wtemp){
                                    posInLista = 0;
                                }
                            }
                            printf("Lista despues de enviados\n");
                            list_All(izquierdo);
                        }
                        break;
                    //Prioridad
                    case 2:
                        if(lado==0){
                            int tamanoLista = list_length(derecho);
                            paquete *listaTemp[tamanoLista];
                            if(1){

                            }
                        }
                        else{

                        }
                        
                        break;
                    //El mas corto
                    case 3:

                        break;
                    //FIFO
                    case 4:

                        break;
                    //Real time
                    case 5:

                        break;
                    
                    default:
                        break;
                    }
            }
        //Letrero
        case 2:
            //int letrero = 0;
            for(int i = 0; i < 2; ++i){
                int time = 0;
                while(time<5){
                    //

                }
            }
            break;
        //Aleatorio
        case 3:
            /* code */
            break;         
        default:
            break;
    }

}