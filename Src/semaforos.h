#ifndef SEMAFOROS_H_INCLUDED
#define SEMAFOROS_H_INCLUDED

#include <stdio.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

//Crea un semaforo a partir de la cadena recibida como nombre con un valor inicial igual al contador recibido
//O devuelve de este si ya existe
int crearSemaforo(char *nombre, int contador);


//Destruye el semaforo que corresponde al nombre e id recibido como parámetros
void destruirSemaforo(char *nombre, int semid);

//Decrementa en 1 el semaforo despues de pasar por él
int bajarSemaforo(int semid);

//Incrementa en 1 el semaforo despues de haber pasado por él
int subirSemaforo(int semid);

//Devuelve el valor actual del semaforo
int getSemaforo(int semid);

//Modifica el valor del semaforo por el valor recibido
int modificarSemaforo(int semid, int val);

#endif // SEMAFOROS_H_INCLUDED
