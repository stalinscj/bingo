#include "bingo.h"

int bandera = 1;
void manejador(int signum){
	
	switch(signum){
		case SIGINT:{
			bandera--;
			exit(1);
		}break;
	}
}

int main(int argc, char **argv)
{
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	int semaforoAforo = crearSemaforo("aforo",1);
	int semaforoCola = crearSemaforo("cola",1);
	
	Sala *sala = obtenerMemoria(crearMemoria(sizeof(Sala)));
	
	destruirTodosSemaforos(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo, semaforoAforo, semaforoCola);
	
	
	imprimirEstado(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, 
		           semaforoParticipantes, semaforoBombo, semaforoAforo, semaforoCola, sala);

	destruirMemoria(crearMemoria(sizeof(Sala)), sala);
		
	printf("\n\nSemaforos  y memoria destruidos..\n");
	
	return 0;
}

