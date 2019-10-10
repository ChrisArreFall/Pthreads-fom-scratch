#include <unistd.h>

#include "packageList.c"
#include "lpthreads.c"
#include "../include/packageList.h"
typedef struct segmento
{
	paquete paquete;		//paquete que se encuentra en el segmento
	int estado;				//nos dice si hay algo en el segmento, esto seria para desplegar en la gui
} segmento;

void scheduler(struct node* derecho,struct node* izquierdo,struct segmento *banda,int tamanoBanda,int schedulerType,int metodoFlujo, int w){
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
                            int tamano = length(derecho);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                printf("solo existen %d paquetes...\n",tamano);
                                w = tamano;
                            }
                            //Como esto es equidad y hay que pasar w paquetes, entonces agrego los paquetes a una lista  
                            //de tamano w.
                            paquete listaTemp[w];
                            for(int j = 0;j<w;++j){
                                struct node *current = getAt(j, derecho);
                                paquete paqueteSeleccionado = current->data;
                                listaTemp[j] = paqueteSeleccionado;
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde 0 hasta el final y despues empieza desde 0
                            int posInLista = tamanoBanda-1;
                            while(true){
                                //ahora tenemos una lista de tamano w que contiene los paquetes elegidos, por ser de tipo round robin se eligen los primeros w
                                //paquetes de la lista enlazada y por ser equidad se la lista es de tamano w
                                //si la lista esta vacia, significa que ya pasaron los 3
                                if(paquetesEnviados==w){
                                    break;
                                }
                                //Ahora procedemos a pasarlos por la banda dandole un tiempo especifico a cada uno por round robin
                                int time = 0;
                                //Cambiar esto para que vaya de derecha a izquierda
                                while(time<1000){
                                    Lthread_yield();
                                    if(banda[listaTemp[posInLista].pos].paquete.estado==1){
                                        //Seteo el segmento actual en cero
                                        banda[listaTemp[posInLista].pos].estado=0;
                                        //Se mueve el paquete una posicion
                                        listaTemp[posInLista].pos-=1;
                                        //Se activa el segmento para mostrar
                                        banda[listaTemp[posInLista].pos].estado=1;
                                        //Se le asigna el paquete
                                        banda[listaTemp[posInLista].pos].paquete=listaTemp[posInLista];
                                        int tiempoCal = 500;
                                        usleep(tiempoCal);
                                        time+=tiempoCal;
                                        if(listaTemp[posInLista].pos==0){
                                            deleteN(listaTemp[posInLista].id, derecho);
                                            banda[listaTemp[posInLista].pos].paquete.estado=0;
                                        }
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
                            int tamano = length(izquierdo);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                printf("solo existen %d paquetes...\n",tamano);
                                w = tamano;
                            }
                            //Como esto es equidad y hay que pasar w paquetes, entonces agrego los paquetes a una lista  
                            //de tamano w.
                            paquete listaTemp[w];
                            for(int j = 0;j<w;++j){
                                struct node *current = getAt(j, izquierdo);
                                paquete paqueteSeleccionado = current->data;
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
                                    if(banda[listaTemp[posInLista].pos].paquete.estado==1){
                                        //Seteo el segmento actual en cero
                                        banda[listaTemp[posInLista].pos].estado=0;
                                        //Se mueve el paquete una posicion
                                        listaTemp[posInLista].pos+=1;
                                        //Se activa el segmento para mostrar
                                        banda[listaTemp[posInLista].pos].estado=1;
                                        //Se le asigna el paquete
                                        banda[listaTemp[posInLista].pos].paquete=listaTemp[posInLista];
                                        int tiempoCal = 500;
                                        usleep(tiempoCal);
                                        time+=tiempoCal;
                                        if(listaTemp[posInLista].pos==0){
                                            deleteN(listaTemp[posInLista].id, izquierdo);
                                            banda[listaTemp[posInLista].pos].paquete.estado=0;
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
paquete roundRobin(struct node* head){

}
paquete priority(struct node* head){

}
paquete shortFirst(struct node* head){

}
paquete firstInFirstOut(struct node* head){

}
paquete realTime(struct node* head){

}