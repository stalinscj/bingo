#ifndef MENSAJES_H_INCLUDED
#define MENSAJES_H_INCLUDED

#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

//Estructura de un mensaje
typedef struct MENSAJE{
	long id;
	int bola;
}Mensaje;

//Crea una una cola de mensaje o devuelve el id si esta ya existe
int crearCola();

//Envia un mensaje a la cola
void enviarMensaje(int idCola, Mensaje *mensaje);

//Lee y borra el mensaje en la cola
void recibirMensaje(int idCola, Mensaje mensaje, int canal);

//Destruye la cola de mensajes
void destruirCola(int idCola);

#endif // MENSAJES_H_INCLUDED
