#include "senales.h"

//Envia una señal a un proceso
void enviarSenal(pid_t destino, int senal){
	if(destino>1)
		kill(destino,senal);
}