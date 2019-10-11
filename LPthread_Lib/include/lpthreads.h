#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>



void initLpthreads();
int Lthread_create( void (*func)(void),int argc,int arg1, int arg2);
void Lthread_start(void (*func)(void));
void Lthread_start_A( void (*func)(int,int) , int arg1, int arg2);
int Lthread_end();
void Lthread_yield();
int Lthread_join(int id);
void Lthread_detach();
void Lthread_wait();
void Lthread_yieldSp();

void obtainNext();

void Lmutex_init();
void Lmutex_destroy();
void Lmutex_unlock();
void Lmutex_trylock();
#endif