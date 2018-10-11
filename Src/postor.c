#include "bingo.h"


int main(int argc, char **argv)
{
	//Cambiando señales
	signal(SIGGANADOR ,manejadorPostor);
	signal(SIGIMPOSTOR ,manejadorPostor);
	signal(SIGBINGO ,manejadorPostor);
	signal(SIGFELICITACIONES ,manejadorPostor);
	
	//Definicion de semaforos
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	int semaforoAforo = crearSemaforo("aforo",MAX_PARTICIPANTES);
	

	int idMemoria, posTarjetas[MAX_TARJETAS_JUGADOR], modo;
	Sala* sala; 
	pthread_t hilo;
	G_Tarjeta G_tarjeta; 
	
	//printf("mi pid = %d", getpid());
	
	//Verificando que la sala no esté llena
	bajarSemaforo(semaforoAforo);

	//Manejo de memoria compartida
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	
	
	//Verificar modo de juego si no se pasan parametros en la consola al momento de ejecutar el postor se asume que es modo manual
	//de lo contrario se verifica que modo de juego se eligió, 0=Manual 1= automatico
	//Ej(Automático): usuario@Debian: ~ $ ./postor 1 
	//Ej(Manual): usuario@Debian: ~ $ ./postor
	//Ej(Manual): usuario@Debian: ~ $ ./postor 0 
	if(argc==1)
		modo = MANUAL;
	else
		modo = atoi(argv[1]);
	
	//Metiendo jugador en sala
	bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		//Inicializando recursos para gráficos
		gtk_init(&argc, &argv);
		
		meterJugadorASala(getpid(), sala, posTarjetas,&G_tarjeta);
		
		//Definiendo modo de juego
		if(modo==MANUAL)
			pthread_create (&hilo, NULL, (void*)escucharNumerosManual, (void*)NULL);
		else
			pthread_create (&hilo, NULL, (void*)escucharNumerosAutomatico, (void*)&G_tarjeta);

	subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	gtk_main();
	
	//Liberando recursos usados por el postor
	bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		finalizarPostor(getpid(), sala);
	subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	//Dejando libre el puesto en la sala
	if(getSemaforo(semaforoAforo)!=0)
		subirSemaforo(semaforoAforo);
	
	return 0;
}

