#include "memoria.h"

//Crea la memoria que compartiran los procesos del tamaño recibido como parametro
//O devuelve el id de esta si ya existe
int crearMemoria(int tamano){
	
	key_t claveMemoria;
	int idMemoria; 
	
	
	claveMemoria = ftok("/bin/ls" , 33);
	idMemoria = shmget(claveMemoria,tamano, 0777 | IPC_CREAT);
	
	return idMemoria;
}

//Devuelve un apuntador a la memoria compartida
void* obtenerMemoria(int idMemoria){
	
	return shmat(idMemoria,0,0);
}

//Destruye/libera la memoria compartida
void destruirMemoria(int idMemoria,void* puntero){
		
	shmdt ((char *) puntero);
	shmctl (idMemoria, IPC_RMID, (struct shmid_ds *) NULL);

}