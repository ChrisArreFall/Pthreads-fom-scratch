#include <malloc.h>
#include <ucontext.h>

#define MAX_THREADS 10
#define THREAD_STACK 1024*1024

#define	LF_NOERROR	0
#define	LF_MAXTHREADS	1
#define LF_MALLOCERROR	2
#define LF_CLONEERROR	3
#define	LF_INTHREAD	4
#define LF_SIGNALERROR	5

#ifdef LF_DEBUG
#define LF_DEBUG_OUT( string, arg ) fprintf( stderr, "liblpthreads debug: " string "\n", arg )
#else
#define LF_DEBUG_OUT( string, arg )
#endif

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
		
		if ( lpthreadList[currentlpthread].active == 0 ){
			LF_DEBUG_OUT( "Lpthread %d ha terminado.\n", currentlpthread );
			//Limpiar significa liberar el stack
            //LF_DEBUG_OUT( "Iniciando limpieza.\n");
			free( lpthreadList[currentlpthread].context.uc_stack.ss_sp );
			//LF_DEBUG_OUT( "Limpieza terminada.\n");
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



/*
 * Nos dice el estado del lpthread, especificamente cuando termino para poder librar los recursos.
 */
static void Lthread_start( void (*func)(void) )
{
	lpthreadList[currentlpthread].active = 1;
	func();
	lpthreadList[currentlpthread].active = 0;
	
	// Concede el control, pero como el estado activo esta en 0, entonces devuelve el control al thread principal
	Lthread_yield();
}

int Lthread_create( void (*func)(void) )
{
	if ( numLpthreads == MAX_THREADS ){
		return LF_MAXTHREADS;
	} 
	
	//Agrega la nueva funcion al final de la lista de threads
	getcontext( &lpthreadList[numLpthreads].context );

	// Setea el contexto un stack nuevo
	lpthreadList[numLpthreads].context.uc_link = 0;
	lpthreadList[numLpthreads].context.uc_stack.ss_sp = malloc( THREAD_STACK );
	lpthreadList[numLpthreads].context.uc_stack.ss_size = THREAD_STACK;
	lpthreadList[numLpthreads].context.uc_stack.ss_flags = 0;	
	
	if ( lpthreadList[numLpthreads].context.uc_stack.ss_sp == 0 )
	{
		LF_DEBUG_OUT( "Error: No se le pudo inicializar un stack nuevo.", 0 );
		return LF_MALLOCERROR;
	}
	
	//Crea el contexto, el contexto llama al a funion anterior de start thread
	makecontext( &lpthreadList[ numLpthreads ].context, (void (*)(void)) &Lthread_start, 1, func );
	++ numLpthreads;
	
	return LF_NOERROR;
}


int Lthread_wait()
{
	int threadsRemaining = 0;
	
	// If we are in a fiber, wait for all the *other* fibers to quit
	if ( inLpthread ) threadsRemaining = 1;
	
	LF_DEBUG_OUT( "Esperando a que %d threads terminen...", threadsRemaining );
	
	// Execute the fibers until they quit
	while ( numLpthreads > threadsRemaining )
	{
		Lthread_yield();
	}
	
	return LF_NOERROR;
}


void Lthread_end(){
	lpthreadList[currentlpthread].active = 0;
	return LF_NOERROR;
}


