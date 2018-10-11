#include "bingo.h"

int main(int argc, char **argv)
{
	//Cambiando señales
	signal(SIGGANADOR ,manejadorJuez);
	signal(SIGIMPOSTOR ,manejadorJuez);
	signal(SIGBINGO ,manejadorJuez);
	signal(SIGVERIFICAR ,manejadorJuez);
	signal(SIGFINALIZAR ,manejadorJuez);
	signal(SIGKILL, manejadorJuez);
	signal(SIGINT, manejadorJuez);
	
	//Definicion de semaforos
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	
	//Manejo de memoria compartida
	int idMemoria;
	Sala* sala;
	
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	
	//Metiendo jueces en la sala
	bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		printf("\nMetiendo jueces en sala...\n");
		usleep(100);
		meterJuezASala(sala);
		usleep(100);
		if(sala->jueces[0]==getpid())
			subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	if(getpid()==sala->jueces[0])
		printf("\nJueces listos...\n");
		
	//Ciclo infinito que se basa en hacer que los jueces se queden pausados hasta que los llamen para verificar un bingo
	while(1){
		pause();
	}
	
	return 0;
}

