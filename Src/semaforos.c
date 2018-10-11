#include "semaforos.h"

//Crea un semaforo a partir de la cadena recibida como nombre con un valor inicial igual al contador recibido
//O devuelve de este si ya existe
int crearSemaforo(char *nombre, int contador)
{
   	int nuevo = 1;
	int fd, semid;
	key_t key;
	union semun {
		int val;
	} arg;
	char buf[FILENAME_MAX];
 
	sprintf(buf, "/tmp/%s", nombre);
 
 	fd = open(buf, O_CREAT | O_EXCL | O_TRUNC, 0400);
	if (fd == -1) {
		if (errno != EEXIST) {
			perror("open");
			return -1;
		}
		/* semaforo ya existente */
		nuevo = 0;
	}
	close(fd);
 
	key = ftok(buf, 1);
	if (key == -1) {
		perror("ftok");
		return -1;
	}
 
	semid = semget(key, 1, IPC_CREAT | 0600);
	if (semid == -1) {
		perror("semget");
		return -1;
	}
 
	if (nuevo) {
		arg.val =  contador;
		if (semctl(semid, 0, SETVAL, arg) == -1) {
			perror("semctl");
			return -1;
		}
	}
 
	return semid;
}


//Destruye el semaforo que corresponde al nombre e id recibido como parámetros
void destruirSemaforo(char *nombre, int semid)
{
	char buf[FILENAME_MAX];
 
	snprintf(buf, FILENAME_MAX, "/tmp/%s", nombre);
 
	if (unlink(buf) == -1)
		perror("unlink");
 
	if (semctl(semid, 0, IPC_RMID) == -1)
		perror("semctl");
}

//Decrementa en 1 el semaforo despues de pasar por él
int bajarSemaforo(int semid)
{
	struct sembuf buf = { 0, -1, 0 };
 
	if (semop(semid, &buf, 1) == -1) {
		perror("semop");
		return -1;
	}
	return 0;
}

//Incrementa en 1 el semaforo despues de haber pasado por él
int subirSemaforo(int semid)
{
	struct sembuf buf = { 0, 1, 0 };
 
	if (semop(semid, &buf, 1) == -1) {
		perror("semop");
		return -1;
	}
	return 0;
}

//Devuelve el valor actual del semaforo
int getSemaforo(int semid)
{
      return semctl(semid, 0, GETVAL);
}

//Modifica el valor del semaforo por el valor recibido
int modificarSemaforo(int semid, int val)
{
	union semun {
		int val;
	} arg;
       
	arg.val = val;
	if (semctl(semid,0,SETVAL,arg) == -1)
	{
		perror("Error al asignar el valor");
		return -1;
	}
        return 0;
}