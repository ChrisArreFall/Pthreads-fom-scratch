#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>


int Lthread_create();
void Lthread_end();
void Lthread_yield();
void Lthread_join();
void Lthread_detach();
void Lmutex_init();
void Lmutex_destroy();
void Lmutex_unlock();
void Lmutex_trylock();




#endif