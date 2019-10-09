#include "scheduler.h"

void scheduler(struct node* derecho,struct node* izquierdo,struct segmento *banda,int schedulerType,int metodoFlujo, int w){
    //primero revisamos el controlador de flujo
    switch (metodoFlujo){
        int lado = 0;
        //Equidad
        case 1:
            //como es equidad se ejecuta dos veces uno para la derecha otro para la izquierda
            for(int i=0;i<2;++i){
                switch (schedulerType){
                    //Round robin
                    case 1:
                        int paquetesEnviados = 0;
                        //Si va de derecha a izquierda
                        if(lado == 0){
                            int tamano = length(derecho);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                paquetesEnviados = w-tamano;
                            }
                            //Finalmente tenemos que empezar con el calendarizador de tipo round robin
                            //Round robin le da un tiempo igual para que cada paquete vaya de un lado a otro
                            //por lo que en este caso el tiempo que le vamos a asignar es de 500ms
                            //en el caso de que un paquete dure mas de eso en pasar, tendra que almacenar la posicion 
                            //y cuando vuelva a ser su turno seguira de donde esta

                            //Va iterando la lista desde 0 hasta el final y despues empieza desde 0
                            int posInLista = 0;
                            while(true){
                                struct node *current = getAt(posInLista, derecho);
                                paquete paqueteSeleccionado = current->data;
                                
                            }
                            
                        }
                        //Si va de izquierda a derecha
                        else{
                            int tamano = length(izquierdo);
                            //si hay menos paquetes que w entonces tenemos que desfasar 
                            if(tamano<w){
                                paquetesEnviados = w-tamano;
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
            /* code */
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