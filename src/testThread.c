#include "../include/lpthreads.h"
#include <stdio.h>
#include <unistd.h>

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
		int nextFib = fib[0] + fib[1];
		printf( "fibonacchi(%d) = %d\n", i, nextFib );
		fib[0] = fib[1];
		fib[1] = nextFib;
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
	
	Lthread_create( &thread1 );
	Lthread_create( &fibonacchi );
	Lthread_create( &squares );

	Lthread_wait();
	
	return 0;
}
