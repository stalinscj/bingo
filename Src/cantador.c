#include "bingo.h"


int main(int argc, char* argv[]){
	
	//Cambiando señales
	signal(SIGGANADOR ,manejadorCantador);
	signal(SIGIMPOSTOR ,manejadorCantador);
	signal(SIGBINGO ,manejadorCantador);
	signal(SIGALRM ,manejadorCantador);
	signal(SIGINT, manejadorCantador);
	
	//Definicion de semaforos
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoAforo = crearSemaforo("aforo",MAX_PARTICIPANTES);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	int semaforoCola = crearSemaforo("cola",1);
	
	//Manejo de memoria compartida y cola de mensajes
	Sala *sala;
	int idMemoria, idCola, participantes;
	
	idCola = crearCola();
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	
	//Inicializacion de estructuras
	bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		printf("\nEsperando jugadores 25 seg...");
		printf("\nInicializando estructuras...\n");
		inicializarSala(sala);
		getSemaforo(semaforoAforo);
		getSemaforo(semaforoCola);
	subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	sleep(25);
	
	//Verificacion de participantes
	bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		printf("\nparticipantes = %d\n",sala->participantes);
		modificarSemaforo(semaforoAforo, 0);
		if(sala->participantes==0){
			finalizarCantador(sala);
		}
	subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	//Ciclo infinito que se basa en cantar un numero del bombo cada 10 seg hasta que no queden participantes
	printf("\nComenzamos...\n");
	while(1){
		bajarSemaforo(semaforoParticipantes);
		bajarSemaforo(semaforoIdentificadores);
		bajarSemaforo(semaforoBombo);
			sleep(5);
			cantar(idCola,sala->participantes, sala->identificadores, sala->bombo);
			participantes = sala->participantes;
		subirSemaforo(semaforoParticipantes);
		subirSemaforo(semaforoIdentificadores);
		subirSemaforo(semaforoBombo);
		
		sleep(5);
		if(participantes==0){
			enviarSenal(getpid(),SIGFINALIZAR);
			printf("\nJuego finalizado: Sala vacia..,\n");
		}
		
	}
	
	return 0;
}
