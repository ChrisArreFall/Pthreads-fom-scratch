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
                        if(lado == 0){
                            printf("%d paquetes yendo de derecha a izquierda...\n",w);
                            int tamano = list_length(derecho);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                printf("solo existen %d paquetes...\n",tamano);
                                w = tamano;
                            }
                            //Como esto es equidad y hay que pasar w paquetes, entonces agrego los paquetes a una lista  
                            //de tamano w.
                            paquete* listaTemp[w];
                            printf("pasando %d paquetes a una lista temporal\n",w);
                            for(int j = 0;j<w;++j){
                                paquete* paqueteSeleccionado = getPaqueteNode(list_getAt(derecho, j));
                                printf("pasando paquete %d con id %d\n",j,paqueteSeleccionado->id);
                                listaTemp[j] = paqueteSeleccionado;
                                printf("Se ha pasado exitosamente con el id %d\n", listaTemp[j]->id);
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde el final hasta 0 y despues empieza desde el final
                            int posInLista = tamanoBanda-1;
                            printf("Empieza el paso de paquetes!.\n");
                            while(true){
                                //ahora tenemos una lista de tamano w que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros w
                                //paquetes de la lista enlazada y por ser equidad se la lista es de tamano w
                                //si la lista esta vacia, significa que ya pasaron los 3
                                if(paquetesEnviados==w){
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
                                    //Seteo el segmento actual en cero
                                        
                                    banda[listaTemp[posInLista]->pos].estado=0;
                                    //Se mueve el paquete una posicion
                                    listaTemp[posInLista]->pos-=1;
                                    //Se activa el segmento para mostrar
                                    banda[listaTemp[posInLista]->pos].estado=1;
                                    //Se le asigna el paquete
                                    banda[listaTemp[posInLista]->pos].paquete=listaTemp[posInLista];
                                    int tiempoCal = 500;
                                    usleep(tiempoCal);
                                    time+=tiempoCal;
                                    if(listaTemp[posInLista]->pos==0){
                                        list_remove_by_data(&derecho,listaTemp[posInLista]);
                                        banda[listaTemp[posInLista]->pos].paquete->estado=0;
                                        ++paquetesEnviados;
                                        printf("Paquete enviado!\n");
                                    }
                                }
                                ++posInLista;
                                if(posInLista==w){
                                    posInLista = 0;
                                }
                            }
                        }
                        //Si va de izquierda a derecha
                        else{
                            printf("%d paquetes yendo de derecha a izquierda...\n",w);
                            int tamano = list_length(izquierdo);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                printf("solo existen %d paquetes...\n",tamano);
                                w = tamano;
                            }
                            //Como esto es equidad y hay que pasar w paquetes, entonces agrego los paquetes a una lista  
                            //de tamano w.
                            paquete* listaTemp[w];
                            for(int j = 0;j<w;++j){
                                paquete* paqueteSeleccionado = getPaqueteNode(list_getAt(izquierdo, j));
                                printf("pasando paquete %d con id %d\n",j,paqueteSeleccionado->id);
                                listaTemp[j] = paqueteSeleccionado;
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde 0 hasta el final y despues empieza desde 0
                            int posInLista = 0;
                            while(true){
                                //ahora tenemos una lista de tamano w que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros w
                                //paquetes de la lista enlazada y por ser equidad se la lista es de tamano w
                                //si la lista esta vacia, significa que ya pasaron los 3
                                if(paquetesEnviados==w){
                                    break;
                                }
                                //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                                int time = 0;
                                //Cambiar esto para que vaya de izquierda a derecha
                                while(time<1000){
                                    Lthread_yield();
                                    if(banda[listaTemp[posInLista]->pos].paquete->estado==1){
                                        //Seteo el segmento actual en cero
                                        banda[listaTemp[posInLista]->pos].estado=0;
                                        //Se mueve el paquete una posicion
                                        listaTemp[posInLista]->pos+=1;
                                        //Se activa el segmento para mostrar
                                        banda[listaTemp[posInLista]->pos].estado=1;
                                        //Se le asigna el paquete
                                        banda[listaTemp[posInLista]->pos].paquete=listaTemp[posInLista];
                                        int tiempoCal = 500;
                                        usleep(tiempoCal);
                                        time+=tiempoCal;
                                        if(listaTemp[posInLista]->pos==0){
                                            list_remove_by_data(&izquierdo,listaTemp[posInLista]);
                                            banda[listaTemp[posInLista]->pos].paquete->estado=0;
                                        }
                                    }
                                }
                                ++posInLista;
                                if(posInLista==w){
                                    posInLista = 0;
                                }
                            }
                        }
                        break;
                    //Prioridad
                    case 2:

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