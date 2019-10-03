#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>



void initLpthreads();
int Lthread_create( void (*func)(void) );
void Lthread_start( void (*func)(void) );
void Lthread_end();
void Lthread_yield();
void Lthread_join();
void Lthread_detach();
void Lthread_wait();

void Lmutex_init();
void Lmutex_destroy();
void Lmutex_unlock();
void Lmutex_trylock();

#endif