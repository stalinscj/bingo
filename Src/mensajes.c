#include "mensajes.h"

//Crea una una cola de mensaje o devuelve el id si esta ya existe
int crearCola(){

	int claveCola;
	int idCola;

	claveCola  = ftok("/bin/ls",33);
	idCola = msgget(claveCola, 0600 | IPC_CREAT);

	return idCola;
}

//Envia un mensaje a la cola
void enviarMensaje(int idCola, Mensaje *mensaje){

	msgsnd(idCola,(struct msgbuf *) mensaje, sizeof(mensaje->bola),IPC_NOWAIT);
}

//Lee y borra el mensaje en la cola
void recibirMensaje(int idCola, Mensaje mensaje, int canal){
	printf("\nen msj.h bola: %d\n", mensaje.bola);
	
	msgrcv(idCola,(struct msgbuf *) &mensaje,sizeof(mensaje.bola),canal,0);
	
	printf("\nen msj.h bola: %d\n", mensaje.bola);
}

//Destruye la cola de mensajes
void destruirCola(int idCola){

	msgctl(idCola, IPC_RMID, (struct msqid_ds*)NULL);
}