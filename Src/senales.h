#ifndef SENALES_H_INCLUDED
#define SENALES_H_INCLUDED

#include <signal.h>
#include <sys/types.h>

//SIGUSR1	 30,10,16
//SIGUSR2	 31,12,17

//Envia una señal a un proceso
void enviarSenal(pid_t destino, int senal);


#endif // SENALES_H_INCLUDED