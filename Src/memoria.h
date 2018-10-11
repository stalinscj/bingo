#ifndef MEMORIA_H_INCLUDED
#define MEMORIA_H_INCLUDED

#include <stdlib.h>
#include <sys/shm.h>
#include "defines.h"

//Crea la memoria que compartiran los procesos del tamaño recibido como parametro
//O devuelve el id de esta si ya existe
int crearMemoria(int tamano);

//Devuelve un apuntador a la memoria compartida
void* obtenerMemoria(int idMemoria);

//Destruye/libera la memoria compartida
void destruirMemoria(int idMemoria,void* puntero);

#endif // MEMORIA_H_INCLUDED
