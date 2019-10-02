#include <malloc.h>
#include <ucontext.h>

#define MAX_THREADS 10

typedef struct
{
	ucontext_t context;     // Almacena el contexto
	int active;             // 0 si no esta activo, 1 si lo esta
} lpthread;


static lpthread lpthreadList[ MAX_THREADS ];   //Ese 10 seria la cantidad de threads permitidos

// El indice del thread actual
static int currentlpthread = -1;
// Esto nos indica si estamos en el main thread o en un hijo
static int inLpthread = 0;
// Numero de lpthreads activos
static int numLpthreads = 0;

/*
 * El contexto principal (main context)
 */
static ucontext_t mainContext;


/*
 * Inicializa todos los threads en inactivo
 */
void initLpthreads(){
	int i;
	for ( i = 0; i < MAX_THREADS; ++ i )
	{
		lpthreadList[i].active = 0;
	}
	return;
}

/*
 * Basicamente esto camba del main thread a los hijos y viceversa
 */
void Lthread_yield(){
	//Entonces si nos encontramos en un lpthread nos vamos al main thread
	if ( inLpthread ){
		//Nos cambiamos al contexto principal
		LF_DEBUG_OUT( "inLpthread debug: Lpthread %d en proceso de yield", currentlpthread );
        //Usando swapcontext
		swapcontext( &lpthreadList[currentlpthread].context, &mainContext );
	}
	// Si no, esto significa que nos encontramos en el main thread 
	else{
		if ( numLpthreads == 0 ){
            return;
        }
		//Procedemos a guardar el thread actual
		currentlpthread = (currentlpthread + 1) % numLpthreads;
		
		LF_DEBUG_OUT( "Cambiando al thread: %d.", currentlpthread );
		inLpthread = 1;
        //Nos cambiamos al siguiente contexto 
		swapcontext( &mainContext, &lpthreadList[ currentlpthread ].context );
		inLpthread = 0;
        //Posteriormente nos volvemos a cambiar al thread principal
		LF_DEBUG_OUT( "Lpthread %d se ha cambiado al thread principal.", currentlpthread );
		
		if ( lpthreadList[currentlpthread].active == 0 )
		{
			LF_DEBUG_OUT( "Lpthread %d ha terminado.\n", currentlpthread );
			//Limpiar significa liberar el stack
            LF_DEBUG_OUT( "Iniciando limpieza.\n");
			free( lpthreadList[currentlpthread].context.uc_stack.ss_sp );
			LF_DEBUG_OUT( "Limpieza terminada.\n");
            //Intercambiar el ultimo lpthread con el actual vacio
			-- numLpthreads;
			if ( currentlpthread != numLpthreads )
			{
				lpthreadList[ currentlpthread ] = lpthreadList[ numLpthreads ];
			}
			lpthreadList[ numLpthreads ].active = 0;		
		}
	}
	return;
}
