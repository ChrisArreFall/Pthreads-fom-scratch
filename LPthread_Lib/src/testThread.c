#include "../include/lpthreads.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h>
/*	Para la prueba de los threads, se ha creado una simulacion de una Banda de paquetes
 Esta banda tiene las siguientes caracteristicas:
 -Los calendarizadores deben ser indicados en el momento de que los hilos son creados.
 -Los paquetes pueden ir de izquierda a derecha o viceversa.

 -El flujo de los paquetes se controlará con algunos de los siguientes algoritmos:

		1) Equidad: Parámetro W que indica cuantos paquetes deben de pasar de cada lado.
		2) Letrero: consiste en un letrero (izquierda - derecha) que indica el sentido de la banda.
		3) Aleatorio: no hay control de flujo
 
 - Archivo de configuracion de cada banda debera contener: 

		✓ Método de control de flujo.
		✓ La fuerza de la banda
		✓ El largo de la banda
		✓ Distribución media de generación de paquetes
		✓ El tiempo que el letrero cambie
		✓ El parámetro W
		✓ Porcentaje de paquetes radioactivos
		✓ Porcentaje de paquetes urgentes

 */

//Paquete, el mismo puede ser de tres tipos distintos, puede tener una asa
typedef struct
{
	int id;
	int tipo;				//0 normal, 1 urgente, 2 radioactivo
	int masa;				//Se definio que es un numero entre 1 y 10 kg
	int lado;  				//0 derecha, 1 izquierda
	int estado;				//0 si aun no ha pasado al lado opuesto, 1 si ya el paquete esta listo
} paquete;


//Segmento de una banda, el largo de la banda nos dira cuantos segmentos deberia tener la misma
typedef struct
{
	paquete paquete;		//paquete que se encuentra en el segmento
	int estado;				//nos dice si hay algo en el segmento, esto seria para desplegar en la gui
} segmento;

//Lista enlazada para el manejo de paquetes
struct Node { 
    paquete paquete; 		//El paquete 
    struct Node* next; 		//El siguiente paquete en la lista
}; 

void bandaTransoportadora(int numBanda){
	//Primero parseo el archivo de configuracion 
	int metodoFlujo;
	int fuerzaBanda;
	int largoBanda;
	int distMediaGenPaq;
	int tiempoLetrero;
	int w;
	int porcPaqRad;
	int porcPaqUrg;
	
	//Si es la banda 1
	if(numBanda==1){
		//parsear paquete e igualar a las variables anteriores
	}
	//Si es la banda 2
	else if(numBanda==2){
		//parsear paquete e igualar a las variables anteriores
	}
	//Si no es la banda 3
	else{
		//parsear paquete e igualar a las variables anteriores
	}
	//Variable temporal que se tiene que cambiar por un while(true)
	int temp = 30;
	int tiempoEntreCreacionPaquetes=1;

	//Banda que se abstrae como una lista
	segmento banda[largoBanda];
	//Que tienen un estado asociado el cual nos dice si alguien la esta ocupando
	int estadoBanda = 0;

	struct Node* head = NULL;
	head = (struct Node*)malloc(sizeof(struct Node));

	int numeroPaquetesTotal = 0;
	while(temp > 0){
		--temp;
		//Primero bajo el contador del tiempo entre creacion de paquetes
		--tiempoEntreCreacionPaquetes;
		//Si ya se acabo el periodo, se tienen que crear mas paquetes
		if(tiempoEntreCreacionPaquetes==0){
			//Reseteo la variable en cinco segundos
			tiempoEntreCreacionPaquetes = 5;
			//Creo paquetes
			int numPaquetes; 
			struct Node* temp = head;
			while(temp->next!=NULL){
				temp = temp->next;
			}
			for(numPaquetes = abs(rand()) % 8;numPaquetes>0;--numPaquetes){
				//Creo nodo temporal
				struct Node* paqueteN = NULL;
				paqueteN = (struct Node*)malloc(sizeof(struct Node));
				//Creo paquete 
				paquete paqueteTemp = { .id = numeroPaquetesTotal, .tipo = (abs(rand()) % 3), .masa = (abs(rand()) % 10) + 1, .lado = (abs(rand()) % 2), .estado = 0};
				//Agrego paquete al nodo
				paqueteN->paquete = paqueteTemp;
				//Lo agrego a la lista enlazada
				temp->next = paqueteN;
				//Me posiciono en el nuevo nodo
				temp = temp->next;
				++numeroPaquetesTotal;
			}
		}

		//Pregunta: El manejo de threads de los paquetes como seria? Si cada paquete es un thread eso significa que ellos tomarian turnos entre todos, pero los paquetes son por banda por lo que
		//se tendria que manejar los threads por conjunto(banda), sin embargo es implicaria que los threads de las bandas nunca se haria yeild a la otra banda porque solo se pasa entre los threads
		//de la banda.	

		//Idea: En vez de threads usar un struct, en donde se simula el calendarizado y nos permite solo tener 3 threads principales que se ejecutan con el yield normal.

		 

		int cantPaq;
		int velocidad;  //


		Lthread_yield();
		sleep(1);
	}
	return;
}

void thread1(){
	int i;
	for ( i = 0; i < 5; ++ i ){
		printf( "Hey, I'm thread #1: %d\n", i );
		Lthread_yield();
	}
	return;
}

void fibonacchi(){
	int i;
	int fib[2] = { 0, 1 };
	
	//sleep( 2 );
	printf( "fibonacchi(0) = 0\nfibonnachi(1) = 1\n" );
	for( i = 2; i < 15; ++ i ){
		Lmutex_trylock();
		int nextFib = fib[0] + fib[1];
		printf( "fibonacchi(%d) = %d\n", i, nextFib );
		fib[0] = fib[1];
		fib[1] = nextFib;
		if(nextFib==34){
			Lthread_end();
		}
		Lmutex_unlock();
		Lthread_yield();
	}
}

void squares(){
	int i;
	
	//sleep( 5 );
	for ( i = 0; i < 10; ++ i ){
		printf( "%d*%d = %d\n", i, i, i*i );
		Lthread_yield();
	}
}

int main(){
	initLpthreads();
	Lmutex_init();

	srand(time(0)); 
	
	Lthread_create( &thread1 );
	Lthread_create( &fibonacchi );
	Lthread_create( &fibonacchi );

	Lthread_wait();
	
	return 0;
}
