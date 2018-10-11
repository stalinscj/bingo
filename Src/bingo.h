#ifndef BINGO_H_INCLUDED
#define BINGO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include "defines.h"
#include "mensajes.h"
#include "senales.h"
#include "memoria.h"
#include "semaforos.h"

//Estructura de las tarjetas de los jugadores
typedef struct TARJETA{

	int numeros[MAX_FILA][MAX_COLUMNA];
	pid_t propietario;

}Tarjeta;

//Estructura de las tarjetas gráficas de los jugadores
typedef struct G_TARJETA{

	GtkWidget* G_ventana;
	GtkWidget* G_contenedor;
	GtkWidget* G_btnBingo;
	GtkWidget* G_numeros[MAX_FILA+1][MAX_TARJETAS_JUGADOR*MAX_COLUMNA+1];
	pid_t propietario;
	struct G_TARJETA *sig;

}G_Tarjeta;

//Estructura de la sala
typedef struct SALA{

	pid_t identificadores[MAX_PARTICIPANTES];
	pid_t ganador;
	pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ+1];
	pid_t cantor;
	int participantes;
	Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR];
	G_Tarjeta *G_tarjetas;
	int bombo[MAX_BOLAS];

}Sala;

//Verifica si un numero se encuentra en una columna
int numeroEnColumna(int numero, int columna, Tarjeta tarjeta);

//Verifica si la tarjeta está repetida
int tarjetaRepetida(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int posTarjeta);

//Llena las tarjetas de los jugadores con numeros aleatorios distintos entre MIN_NUMERO y MAX_NUMERO
void llenarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]);

//Inicializa las tarjetas colocando 0 en todas sus posiciones y asigandole 1 al propietario
//En cuanto a las tarjetas gráficas las inicializa apuntado a NULL
void inicializarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], G_Tarjeta *G_tarjetas[MAX_PARTICIPANTES]);

//Desmarca o limpia las tarjetas posiblemente usadas por los jugadores
void desmarcarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]);

//Imprime las tarjetas por consola
void imprimirTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int participantes);

//Verifica si una bola se encuentra en el bombo
int bolaEnBombo(int bola, int bombo[MAX_BOLAS]);

//Imprime las bolas del bombo por consola
void imprimirBombo(int bombo[MAX_BOLAS]);

//Inicializa el bombo colocando numeros aleatorios distintos probablemente desordenados entre MIN_NUMERO y MAX_NUMERO
void inicializarBombo(int bombo[MAX_BOLAS]);

//Inicializa los jueces colocando 1 como su pid
void inicializarJueces(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]);

//Inicializa los identificadores de los participantes colocando 1 como su pid
void inicializarIdentificadores(pid_t identificadores[MAX_PARTICIPANTES]);

//Inicialiaza la sala "memoria compartida"
void inicializarSala(Sala* sala);

//Verifica si el bombo está vacio
int bomboVacio(int bombo[MAX_BOLAS]);

//Canta un numero al azar no cantado del bombo
void cantar(int idCola, int participantes, pid_t identificadores[MAX_PARTICIPANTES], int bombo[MAX_BOLAS]);

//Devuelve la posición en la que se encuentra la primera tarjeta del jugador 
int posicionDeTarjeta1(int pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]);

//Verfica si un bingo es válido
int comprobarBingo(int pidGanador, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int bombo[MAX_BOLAS]);

//Devuelve el numero de columna en las tarjetas a la que pertenece un numero
//Ej: la primera columna "B" esta compuesta por numero entre 1 y 15
//Ej: la primera columna "I" esta compuesta por numero entre 16 y 30
//Ej: la primera columna "N" esta compuesta por numero entre 31 y 45
//Ej: la primera columna "G" esta compuesta por numero entre 46 y 60
//Ej: la primera columna "O" esta compuesta por numero entre 61 y 75
int columnaPerteneciente(int numero);

//Tacha o marca un numero de las tarjetas del jugador multiplicandolo por -1
int tacharNumero(int numero, int pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]);


/*int tacharNumero2(int numero, int posTarjetas[MAX_TARJETAS_JUGADOR], Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

	int i,j,columna;

	columna = columnaPerteneciente(numero);

	for( i = 0; i < MAX_TARJETAS_JUGADOR; i++){
		for (j = 0; j < MAX_FILA; j++){
			if(tarjetas[posTarjetas[i]].numeros[j][columna]==numero){
				
				tarjetas[posTarjetas[i]].numeros[j][columna]*= -1;
				
				if(tarjetas[posTarjetas[i]].numeros[j][columna]>0)
					return 1;
				else
					return -1;
			}
		}
	}
	
	return 0;
}*/

//Avisa al juez y cantor que har un bingo y coloca el pid del jugador como posible ganador
void cantarBingo(int *ganador, pid_t pidJuez, pid_t cantor);

//Obtiene las posiciones de todas las tarjetas que posea un jugador
//almacenándolas en un vector recibido como parámetro
void obtenerPosicionDeTarjetas(pid_t pidPropietario, int posTarjetas[MAX_TARJETAS_JUGADOR], Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]);

//Tacha el numero pulsado en pantalla...
//Si el número no estaba tachado (fondo blanco) se tacha colocándole un fondo azul
//Si el número ya estaba tachado (fondo azul) se desmarca colocándole un fondo blanco
void G_tacharNumero(GtkWidget* btnNumero, Sala* sala);

//Manejo del boton "Bingo!" para cantar bingo
void G_cantarBingo(GtkWidget* btnBingo, Sala* sala);

//Inicializa las tarjetas graficas de los jugadores
void G_inicialiarTarjeta(G_Tarjeta *G_tarjeta, Tarjeta tarjeta1, Tarjeta tarjeta2, Sala **sala);


//Mete un jugador en la sala
void meterJugadorASala(pid_t pid, Sala *sala, int posTarjetas[MAX_TARJETAS_JUGADOR], G_Tarjeta* G_tarjeta);

//Mata los procesos hijos que fueron creados por el juez principal
void matarJuecesHijos(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]);

//Mete a los jueces en la sala
void meterJuezASala(Sala *sala);

//Designa a un juez para verificar un bingo
int designarJuez(pid_t pidGanador, pid_t identificadores[MAX_PARTICIPANTES]);

//Le quita las tarjetas un jugador cambiando el pid del propietario a 1
void quitarTarjetasAJugador(pid_t pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int participantes);

//Libera los recursos usados por el postor
void finalizarPostor(pid_t pidSaliente, Sala *sala);

//Destruye todos los semaforos 
void destruirTodosSemaforos(int semaforoGanador, int semaforoTarjetas, int semaforoCantor,
                            int semaforoJueces, int semaforoIdentificadores, int semaforoParticipantes,
                            int semaforoBombo, int semaforoAforo, int semaforoCola);

//Imprime el estado de los semáforos
void imprimirEstado(int semaforoGanador, int semaforoTarjetas, int semaforoCantor,
                            int semaforoJueces, int semaforoIdentificadores, int semaforoParticipantes,
                            int semaforoBombo, int semaforoAforo, int semaforoCola, Sala *sala);

//Decrementa en 1 el valor de los semaforos de la sala despues de haber pasado por ellos
void bajarSemaforoDeSala(int semaforoGanador, int semaforoTarjetas, int semaforoCantor, int semaforoJueces, 
                         int semaforoIdentificadores, int semaforoParticipantes, int semaforoBombo);

//Incrementa en 1 el valor de los semaforos de la sala despues de haber pasado por ellos
void subirSemaforoDeSala(int semaforoGanador, int semaforoTarjetas, int semaforoCantor, int semaforoJueces, 
                         int semaforoIdentificadores, int semaforoParticipantes, int semaforoBombo);

//Libera los recursos usados por en cantador
void finalizarCantador(Sala *sala);

//Libera los recursos usados por el juez
void finalizarJuez(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]);

//Manejador de señales del cantador
void manejadorCantador(int sigNum);

//Manejador de señales de los jueces
void manejadorJuez(int sigNum);

//Manejador de señales del postor
void manejadorPostor(int sigNum);

//Escucha los numeros de forma manual
void escucharNumerosManual();

//Verifica si una tarjeta está llena
int tarjetaLlena(Tarjeta tarjeta);

//Escucha los nuemros de forma automática, es decir, son marcados tan pronto son cantados
//y se canta bingo de forma automatica una vez que la tarjeta esté llena
void escucharNumerosAutomatico(G_Tarjeta *G_tarjeta);

#endif // BINGO_H_INCLUDED
