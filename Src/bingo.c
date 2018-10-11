#include "bingo.h"

//Verifica si un numero se encuentra en una columna
int numeroEnColumna(int numero, int columna, Tarjeta tarjeta){

	int i;

	for (i = 0; i < MAX_FILA; i++)
		if(numero==tarjeta.numeros[i][columna])
			return 1;

	return 0;
}

//Verifica si la tarjeta está repetida
int tarjetaRepetida(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int posTarjeta){

	int i, j, k, bandera;
	Tarjeta auxiliar;

	auxiliar = tarjetas[0];
	tarjetas[0] = tarjetas[posTarjeta];
	tarjetas[posTarjeta] = auxiliar;

	for(i = 1; i < MAX_PARTICIPANTES * MAX_TARJETAS_JUGADOR; i++){

		bandera = 0;
		for (j = 0; j < MAX_FILA && !bandera; j++){
			for (k = 0; k < MAX_COLUMNA && !bandera; k++){

				if(!numeroEnColumna(tarjetas[0].numeros[j][k], k, tarjetas[i]))
					bandera=1;
			}
		}

		if(!bandera)
			return 1;
	}
	return 0;
}

//Llena las tarjetas de los jugadores con numeros aleatorios distintos entre MIN_NUMERO y MAX_NUMERO
void llenarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

	int i,j,k,aleatorio;

	srand(time(NULL));

	i=0;
	do{
		for (j = 0; j < MAX_FILA; j++){
			for (k = 0; k < MAX_COLUMNA; k++){
				do{
					aleatorio = rand()%15;
					aleatorio += k*15+1;
				} while (numeroEnColumna(aleatorio,k,tarjetas[i]));

				tarjetas[i].numeros[j][k]=aleatorio;
			}
		}

		if(!tarjetaRepetida(tarjetas,i))
			i++;

	}while(i<MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR);
}

//Inicializa las tarjetas colocando 0 en todas sus posiciones y asigandole 1 al propietario
//En cuanto a las tarjetas gráficas las inicializa apuntado a NULL
void inicializarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], G_Tarjeta **G_tarjetas){
	int i,j,k;

	for( i = 0; i < MAX_PARTICIPANTES * MAX_TARJETAS_JUGADOR; i++){

		tarjetas[i].propietario=1;

		for (j = 0; j < MAX_FILA; j++){
			for (k = 0; k < MAX_COLUMNA; k++){

				tarjetas[i].numeros[j][k]=0;
			}
		}
	}

	llenarTarjetas(tarjetas);
	
	*G_tarjetas = NULL;

}

//Desmarca o limpia las tarjetas posiblemente usadas por los jugadores
void desmarcarTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

    int i,j,k;

	for( i = 0; i < MAX_PARTICIPANTES * MAX_TARJETAS_JUGADOR; i++){
		for (j = 0; j < MAX_FILA; j++){
			for (k = 0; k < MAX_COLUMNA; k++){
				if(tarjetas[i].numeros[j][k]<0)
                    tarjetas[i].numeros[j][k]*=-1;
			}
		}
	}
}

//Imprime las tarjetas por consola
void imprimirTarjetas(Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int participantes){

	int i,j,k;

	for (i = 0; i < participantes * MAX_TARJETAS_JUGADOR; i++){
		printf("  B    I     N    G   O :%d\n",tarjetas[i].propietario);
		printf("-------------------------\n");
		for ( j = 0; j < MAX_FILA; j++){
			for (k = 0; k < MAX_COLUMNA; k++){

				printf("| ");
				printf("%d", tarjetas[i].numeros[j][k]);

				if(tarjetas[i].numeros[j][k]<10)
					printf("  ");
				else
					printf(" ");
			}
			printf("|\n");
		}
		printf("-------------------------\n\n");
	}
}

//Verifica si una bola se encuentra en el bombo
int bolaEnBombo(int bola, int bombo[MAX_BOLAS]){

	int i;

	for(i = 0; i < MAX_BOLAS; i++){
		if(bola==bombo[i])
			return 1;
	}
	return 0;
}

//Imprime las bolas del bombo por consola
void imprimirBombo(int bombo[MAX_BOLAS]){
		
	int i;
	
	printf("\nBombo: ");
	
	for(i = 0; i < MAX_BOLAS; i++)
		printf("%d ",bombo[i]);
}

//Inicializa el bombo colocando numeros aleatorios distintos probablemente desordenados entre MIN_NUMERO y MAX_NUMERO
void inicializarBombo(int bombo[MAX_BOLAS]){

	int i,aleatorio;

	srand(time(NULL));

	for(i = 0; i < MAX_BOLAS; i++){
		bombo[i]=0;
	}

	for(i = 0; i < MAX_BOLAS; i++){
		do{
			aleatorio = (rand()%MAX_NUMERO) + MIN_NUMERO;

		}while(bolaEnBombo(aleatorio,bombo));
		bombo[i]=aleatorio;
	}
}

//Inicializa los jueces colocando 1 como su pid
void inicializarJueces(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]){

	int i;
	
	for(i = 0; i < MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1; i++)
		jueces[i]=1;
	
}

//Inicializa los identificadores de los participantes colocando 1 como su pid
void inicializarIdentificadores(pid_t identificadores[MAX_PARTICIPANTES]){
	int i;
	
	for(i = 0; i < MAX_PARTICIPANTES; i++)
		identificadores[i]=1;
}

//Inicialiaza la sala "memoria compartida"
void inicializarSala(Sala* sala){
	
	inicializarTarjetas(sala->tarjetas, &sala->G_tarjetas);
	inicializarBombo(sala->bombo);
	inicializarJueces(sala->jueces);
	inicializarIdentificadores(sala->identificadores);
	sala->ganador=1;
	sala->cantor = getpid();
	sala->participantes=0;

}

//Verifica si el bombo está vacio
int bomboVacio(int bombo[MAX_BOLAS]){
	
	int i;
	
	for(i = 0; i < MAX_BOLAS; i++)
		if(bombo[i]>0)
			return 0;
	
	return 1;
}

//Canta un numero al azar no cantado del bombo
void cantar(int idCola, int participantes, pid_t identificadores[MAX_PARTICIPANTES], int bombo[MAX_BOLAS]){

	int i, aleatorio;
	Mensaje mensaje;
	
	if(!bomboVacio(bombo)){
		do{
			aleatorio = rand()%MAX_BOLAS;
		}while(bombo[aleatorio]<0);
		
		mensaje.bola = bombo[aleatorio];
		
		for (i = 0; i < participantes; ++i){
			mensaje.id = identificadores[i];
			enviarMensaje(idCola,&mensaje);
		}
		printf("\nBola = %d", bombo[aleatorio]);
		bombo[aleatorio]*=-1;
	}else{
		pause();
	}
}

//Devuelve la posición en la que se encuentra la primera tarjeta del jugador 
int posicionDeTarjeta1(int pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

	int i;
	for (i = 0; i < MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR; ++i)
		if(pidPropietario == tarjetas[i].propietario)
			return i;

	return -1;
}

//Verfica si un bingo es válido
int comprobarBingo(int pidGanador, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int bombo[MAX_BOLAS]){

	int i,j,k,bandera,posTarjeta;

	posTarjeta = posicionDeTarjeta1(pidGanador,tarjetas);
	
	for (i = posTarjeta; i < posTarjeta+MAX_TARJETAS_JUGADOR; i++){
		printf("  B    I     N    G   O\n");
		printf("-------------------------\n");
		for ( j = 0; j < MAX_FILA; j++){
			for (k = 0; k < MAX_COLUMNA; k++){

				printf("| ");
				printf("%d", tarjetas[i].numeros[j][k]);

				if(tarjetas[i].numeros[j][k]<10)
					printf("  ");
				else
					printf(" ");
			}
			printf("|\n");
		}
		printf("-------------------------\n\n");
	}

	for( i = posTarjeta; i < posTarjeta+MAX_TARJETAS_JUGADOR; i++){
		
		printf("\nVerificando Tarjeta[%d]\n", i);
		
		bandera = 0;
		for (j = 0; j < MAX_FILA && !bandera; j++){
			for (k = 0; k < MAX_COLUMNA && !bandera; k++){
				if(tarjetas[i].numeros[j][k]>=0){
					printf("\nFalta el numero: %d\n", tarjetas[i].numeros[j][k]);
					bandera = 1;
				}else
					if(!bolaEnBombo(tarjetas[i].numeros[j][k],bombo)){
						printf("\nNo ha salido el numero: %d\n",tarjetas[i].numeros[j][k]*-1);
						bandera=1;
					}
			}
		}
		
		printf("\nTarjeta[%d] R= %d\n", i,!bandera);
		
		if(!bandera)
			break;
	}

	return !bandera;
}

//Devuelve el numero de columna en las tarjetas a la que pertenece un numero
//Ej: la primera columna "B" esta compuesta por numero entre 1 y 15
//Ej: la primera columna "I" esta compuesta por numero entre 16 y 30
//Ej: la primera columna "N" esta compuesta por numero entre 31 y 45
//Ej: la primera columna "G" esta compuesta por numero entre 46 y 60
//Ej: la primera columna "O" esta compuesta por numero entre 61 y 75
int columnaPerteneciente(int numero){

	if(numero<1)
		return -1;

	if (numero<16)
		return 0;

	if (numero<31)
		return 1;

	if (numero<46)
		return 2;

	if (numero<61)
		return 3;

	if (numero<76)
		return 4;

	return -1;
}

//Tacha o marca un numero de las tarjetas del jugador multiplicandolo por -1
int tacharNumero(int numero, int pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

	int i,j,columna;

	columna = columnaPerteneciente(numero);

	for( i = 0; i < MAX_PARTICIPANTES * MAX_TARJETAS_JUGADOR; i++){
		if(pidPropietario==tarjetas[i].propietario){
			for (j = 0; j < MAX_FILA; j++){
				if(tarjetas[i].numeros[j][columna]==numero || tarjetas[i].numeros[j][columna]==numero*-1){
					tarjetas[i].numeros[j][columna]*= -1;
					if(tarjetas[i].numeros[j][columna]>0)
						return 1;
					else
						return -1;
				}
			}
		}
	}
	return 0;
}

//Avisa al juez y cantor que har un bingo y coloca el pid del jugador como posible ganador
void cantarBingo(int *ganador, pid_t pidJuez, pid_t cantor){

	*ganador = getpid();
	enviarSenal(pidJuez,SIGBINGO);
	enviarSenal(cantor,SIGBINGO);
}

//Obtiene las posiciones de todas las tarjetas que posea un jugador
//almacenándolas en un vector recibido como parámetro
void obtenerPosicionDeTarjetas(pid_t pidPropietario, int posTarjetas[MAX_TARJETAS_JUGADOR], Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR]){

	int i,contador;

	contador=0;

	for (i = 0; (i < MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR) && (contador < MAX_TARJETAS_JUGADOR); ++i){
		if(tarjetas[i].propietario==pidPropietario){
			posTarjetas[contador]=i;
			contador++;
		}
	}
}

//Tacha el numero pulsado en pantalla...
//Si el número no estaba tachado (fondo blanco) se tacha colocándole un fondo azul
//Si el número ya estaba tachado (fondo azul) se desmarca colocándole un fondo blanco
G_Tarjeta* miTarjetaGrafica;
 
void G_tacharNumero(GtkWidget* btnNumero, Sala* sala){
	
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	GdkColor color;
	int numero, bandera,i,j,k;
	numero = atoi(gtk_button_get_label(GTK_BUTTON(btnNumero)));

	
	bajarSemaforo(semaforoTarjetas);
		bandera = tacharNumero(numero, getpid(), sala->tarjetas);
		
		//obtenerPosicionDeTarjetas(getpid(),posTarjetas, sala->tarjetas);
		
		
		if(bandera!=0){
			if(bandera==1)
				gdk_color_parse ("white", &color);
			else
				gdk_color_parse ("blue", &color);
				
			bandera = 0;
			for(i = 0; i < MAX_PARTICIPANTES && !bandera; i++){
				
				if(miTarjetaGrafica->propietario==getpid()){
					
					for(j = 1; j < MAX_FILA+1 && !bandera; j++)
						for(k = 0; k < MAX_COLUMNA && !bandera; k++){
							if( atoi(gtk_button_get_label(GTK_BUTTON(miTarjetaGrafica->G_numeros[j][k]))) == numero){
								gtk_widget_modify_bg (GTK_WIDGET(miTarjetaGrafica->G_numeros[j][k]), GTK_STATE_NORMAL, &color);
								//printf("\nLado 1 en F=%d C=%d\n",j,k);
								bandera=1;
							}
						}
					//printf("\nvoy a ver el otro lado\n");
					bandera = 0;
					for(j = 1; j < MAX_FILA+1 && !bandera; j++)
						for(k = 6; (k < ((MAX_COLUMNA*MAX_TARJETAS_JUGADOR) +1)) && !bandera; k++){
							if( atoi(gtk_button_get_label(GTK_BUTTON(miTarjetaGrafica->G_numeros[j][k]))) == numero){
								gtk_widget_modify_bg (GTK_WIDGET(miTarjetaGrafica->G_numeros[j][k]), GTK_STATE_NORMAL, &color);
								//printf("\nLado 2 F=%d C=%d\n",j,k);
								bandera=1;
							}
						}
		
					bandera = 1;
				}
			}
		}else{
			gdk_color_parse ("red", &color);
			gtk_widget_modify_bg (GTK_WIDGET(btnNumero), GTK_STATE_NORMAL, &color);
		}
			
	subirSemaforo(semaforoTarjetas);
}

//Manejo del boton "Bingo!" para cantar bingo
void G_cantarBingo(GtkWidget* btnBingo, Sala* sala){
	
	int semaforoGanador = crearSemaforo("ganador",1);
	bajarSemaforo(semaforoGanador);
		cantarBingo(&sala->ganador, sala->jueces[0], sala->cantor);	
}

//Inicializa las tarjetas graficas de los jugadores
void G_inicialiarTarjeta(G_Tarjeta *G_tarjeta, Tarjeta tarjeta1, Tarjeta tarjeta2, Sala **sala){
	
	int i,j;
	char cadena[5];
	GdkColor color;
	
	G_tarjeta->G_ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(G_tarjeta->G_ventana,TAM_PANTALLA_X,TAM_PANTALLA_Y);
	gtk_window_set_resizable(GTK_WINDOW(G_tarjeta->G_ventana),0);
	gtk_window_set_position(GTK_WINDOW(G_tarjeta->G_ventana),3);
	gtk_window_set_title(GTK_WINDOW(G_tarjeta->G_ventana), "B-I-N-G-O Carrasco_Sanchez");
	gtk_container_set_border_width(GTK_CONTAINER(G_tarjeta->G_ventana), 5);
	
	G_tarjeta->G_contenedor = gtk_table_new(MAX_FILA+1,MAX_TARJETAS_JUGADOR*MAX_COLUMNA +1,0);
	gtk_table_set_row_spacings(GTK_TABLE(G_tarjeta->G_contenedor), 1);
    gtk_table_set_col_spacings(GTK_TABLE(G_tarjeta->G_contenedor), 1);
    gtk_container_add(GTK_CONTAINER(G_tarjeta->G_ventana), G_tarjeta->G_contenedor);
    
    gdk_color_parse ("white", &color);	
    for(i=0;i<MAX_FILA+1;i++)
		for(j=0;j<MAX_TARJETAS_JUGADOR*MAX_COLUMNA +1;j++){
			G_tarjeta->G_numeros[i][j] = gtk_button_new_with_label(" ");
			gtk_widget_modify_bg (GTK_WIDGET(G_tarjeta->G_numeros[i][j]), GTK_STATE_NORMAL, &color);
		}
	
	for(i=0;i<MAX_FILA;i++)
		for(j=0;j<MAX_COLUMNA;j++){
			sprintf(cadena,"%d",tarjeta1.numeros[i][j]);
			G_tarjeta->G_numeros[i+1][j] = gtk_button_new_with_label(cadena);
		}
		
	for(i=0;i<MAX_FILA;i++)
		for(j=0;j<MAX_COLUMNA;j++){
			sprintf(cadena,"%d",tarjeta2.numeros[i][j]);
			G_tarjeta->G_numeros[i+1][j+6]=gtk_button_new_with_label(cadena);
		}
		
		
	for(j=0;j<MAX_TARJETAS_JUGADOR;j++){
		G_tarjeta->G_numeros[0][j*6+0] = gtk_label_new("B");
		G_tarjeta->G_numeros[0][j*6+1] = gtk_label_new("I");
		G_tarjeta->G_numeros[0][j*6+2] = gtk_label_new("N");
		G_tarjeta->G_numeros[0][j*6+3] = gtk_label_new("G");
		G_tarjeta->G_numeros[0][j*6+4] = gtk_label_new("O");
	}
	
		
	G_tarjeta->G_numeros[0][5] = gtk_button_new_with_label("Bingo!");
	gdk_color_parse ("red", &color);
	gtk_widget_modify_bg (GTK_WIDGET(G_tarjeta->G_numeros[0][5]), GTK_STATE_NORMAL, &color);
	g_signal_connect(G_tarjeta->G_numeros[0][5],"clicked", G_CALLBACK(G_cantarBingo),*sala);
	
	
	gdk_color_parse ("black", &color);
	for(j=1;j<MAX_FILA+1;j++)
		gtk_widget_modify_bg (GTK_WIDGET(G_tarjeta->G_numeros[j][5]), GTK_STATE_NORMAL, &color);
			
	
	for(i=1;i<MAX_FILA+1;i++)
		for(j=0;j<MAX_COLUMNA;j++){
			g_signal_connect(G_tarjeta->G_numeros[i][j],"clicked", G_CALLBACK(G_tacharNumero),*sala);
			g_signal_connect(G_tarjeta->G_numeros[i][j+6],"clicked", G_CALLBACK(G_tacharNumero),*sala);
		}
	g_signal_connect(G_tarjeta->G_ventana, "delete-event", G_CALLBACK(gtk_main_quit),NULL);
		
	for(i=0;i<MAX_FILA+1;i++)
		for(j=0;j<MAX_TARJETAS_JUGADOR*MAX_COLUMNA +1;j++){
			gtk_table_attach_defaults(GTK_TABLE(G_tarjeta->G_contenedor), G_tarjeta->G_numeros[i][j], j,j+1,i,i+1);
		}
	
	G_tarjeta->propietario = tarjeta1.propietario;
	miTarjetaGrafica = G_tarjeta;
}

//Mete un jugador en la sala
void meterJugadorASala(pid_t pid, Sala *sala, int posTarjetas[MAX_TARJETAS_JUGADOR], G_Tarjeta* G_tarjeta){

	int i,contador;

	contador = 0;

	for (i = 0; i < sala->participantes*MAX_TARJETAS_JUGADOR + MAX_TARJETAS_JUGADOR; i++){

		if(sala->tarjetas[i].propietario == 1){
			sala->tarjetas[i].propietario = pid;
			contador++;
			if(contador==MAX_TARJETAS_JUGADOR)
				break;
		}
	}

	sala->identificadores[sala->participantes] = pid;
	
	obtenerPosicionDeTarjetas(pid, posTarjetas, sala->tarjetas);
	
	G_inicialiarTarjeta(G_tarjeta,sala->tarjetas[posTarjetas[0]],sala->tarjetas[posTarjetas[1]],&sala);
	
	sala->participantes++;
	
	gtk_widget_show_all(miTarjetaGrafica->G_ventana);
	
}

//Mata los procesos hijos que fueron creados por el juez principal
void matarJuecesHijos(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]){

	int i;
	
	printf("\nMatando jueces hijos...\n");
	
	for(i = 1; i < MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1; i++){
			enviarSenal(jueces[i], SIGKILL);
			jueces[i]=1;		
	}
}

//Mete a los jueces en la sala
void meterJuezASala(Sala *sala){
	
	int i,hijos,pid;

	hijos = MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ;

	if(MAX_PARTICIPANTES%MAX_JUGADORES_JUEZ == 0)
		hijos--;

	sala->jueces[0] = getpid();

	for(i = 1; i <= hijos; i++){

		pid = fork();

		if(pid==0)
			break;
		else
			sala->jueces[i] = pid;
	}
}

//Designa a un juez para verificar un bingo
int designarJuez(pid_t pidGanador, pid_t identificadores[MAX_PARTICIPANTES]){

	int i,juezDesignado;

	for(i = 0; i < MAX_PARTICIPANTES; i++)
		if(identificadores[i]==pidGanador)
			break;

	juezDesignado = 0;

	while(juezDesignado<MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1){

		if(i>=juezDesignado*MAX_JUGADORES_JUEZ && i<(juezDesignado+1)*MAX_JUGADORES_JUEZ)
			return juezDesignado;

		juezDesignado++;
	}

	return -1;
}

//Le quita las tarjetas un jugador cambiando el pid del propietario a 1
void quitarTarjetasAJugador(pid_t pidPropietario, Tarjeta tarjetas[MAX_PARTICIPANTES*MAX_TARJETAS_JUGADOR], int participantes){
	
	int i, bandera;

	bandera = 0;
	
	if(participantes!=MAX_PARTICIPANTES)
		for(i = 0; i < participantes; i++){
			if(tarjetas[i].propietario==pidPropietario || bandera){
				tarjetas[i] = tarjetas[i+1] ;
				bandera=1;
			}
		}
	else{
		for(i = 0; i < participantes-1; i++){
			if(tarjetas[i].propietario==pidPropietario || bandera){
				tarjetas[i] = tarjetas[i+1] ;
				bandera=1;
			}
		}
		tarjetas[MAX_PARTICIPANTES-1].propietario = 1;
	}
	
}


//Libera los recursos usados por el postor
void finalizarPostor(pid_t pidSaliente, Sala *sala){

	int i,bandera;
	
	if(sala->participantes!=MAX_PARTICIPANTES)
		for(i = 0; i < sala->participantes; i++){
			if(sala->identificadores[i]==pidSaliente || bandera){
				sala->identificadores[i] = sala->identificadores[i+1];
				bandera=1;
			}
		}
	else{
		for(i = 0; i < sala->participantes-1; i++){
			if(sala->identificadores[i]==pidSaliente || bandera){
				sala->identificadores[i] = sala->identificadores[i+1];
				bandera=1;
			}
		}
		
		sala->identificadores[MAX_PARTICIPANTES-1]=1;
	}

	quitarTarjetasAJugador(pidSaliente, sala->tarjetas, sala->participantes);

	sala->participantes--;
}

//Destruye todos los semaforos 
void destruirTodosSemaforos(int semaforoGanador, int semaforoTarjetas, int semaforoCantor,
                            int semaforoJueces, int semaforoIdentificadores, int semaforoParticipantes,
                            int semaforoBombo, int semaforoAforo, int semaforoCola){
	
	destruirSemaforo("ganador", semaforoGanador);
	destruirSemaforo("tarjetas", semaforoTarjetas);
	destruirSemaforo("cantor", semaforoCantor);
	destruirSemaforo("jueces", semaforoJueces);
	destruirSemaforo("identificadores", semaforoIdentificadores);
	destruirSemaforo("participantes", semaforoParticipantes);
	destruirSemaforo("bombo", semaforoBombo);
	destruirSemaforo("aforo", semaforoAforo);
	destruirSemaforo("cola", semaforoCola);
}

//Imprime el estado de los semáforos
void imprimirEstado(int semaforoGanador, int semaforoTarjetas, int semaforoCantor,
                            int semaforoJueces, int semaforoIdentificadores, int semaforoParticipantes,
                            int semaforoBombo, int semaforoAforo, int semaforoCola, Sala *sala){
	
	printf("\nParticipantes: %d\n", sala->participantes);
	
	printf("\n\nganador: %d", getSemaforo(semaforoGanador));
	printf("\ntarjetas: %d", getSemaforo(semaforoTarjetas));
	printf("\ncantor: %d", getSemaforo(semaforoCantor));
	printf("\njueces: %d", getSemaforo(semaforoJueces));
	printf("\nidentificadores: %d", getSemaforo(semaforoIdentificadores));
	printf("\nparticipantes: %d", getSemaforo(semaforoParticipantes));
	printf("\nbombo: %d", getSemaforo(semaforoBombo));
	printf("\naforo: %d", getSemaforo(semaforoAforo));
	printf("\ncola: %d", getSemaforo(semaforoCola));
	
}

//Decrementa en 1 el valor de los semaforos de la sala despues de haber pasado por ellos
void bajarSemaforoDeSala(int semaforoGanador, int semaforoTarjetas, int semaforoCantor, int semaforoJueces, 
                         int semaforoIdentificadores, int semaforoParticipantes, int semaforoBombo){
	
	bajarSemaforo(semaforoGanador);
	bajarSemaforo(semaforoTarjetas);
	bajarSemaforo(semaforoCantor);
	bajarSemaforo(semaforoJueces);
	bajarSemaforo(semaforoIdentificadores);
	bajarSemaforo(semaforoParticipantes);
	bajarSemaforo(semaforoBombo);
}

//Incrementa en 1 el valor de los semaforos de la sala despues de haber pasado por ellos
void subirSemaforoDeSala(int semaforoGanador, int semaforoTarjetas, int semaforoCantor, int semaforoJueces, 
                         int semaforoIdentificadores, int semaforoParticipantes, int semaforoBombo){
	
	subirSemaforo(semaforoGanador);
	subirSemaforo(semaforoTarjetas);
	subirSemaforo(semaforoCantor);
	subirSemaforo(semaforoJueces);
	subirSemaforo(semaforoIdentificadores);
	subirSemaforo(semaforoParticipantes);
	subirSemaforo(semaforoBombo);
}

//Libera los recursos usados por en cantador
void finalizarCantador(Sala *sala){
	
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoAforo = crearSemaforo("aforo", MAX_PARTICIPANTES);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	int semaforoCola = crearSemaforo("cola",1);
	
	
	
	int i,idCola,idMemoria;
	
	for(i = 0; i < sala->participantes; i++)
		enviarSenal(sala->identificadores[i], SIGFINALIZAR);
	
	enviarSenal(sala->jueces[0], SIGFINALIZAR);
	
	sala->cantor = -1;
	
	subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
	
	sleep(1);
	
	idMemoria = crearMemoria(sizeof(Sala));
	idCola = crearCola();
	
	destruirCola(idCola);
	destruirMemoria(idMemoria,sala);
	destruirTodosSemaforos(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo, semaforoAforo, semaforoCola);
	exit(1);
}

//Libera los recursos usados por el juez
void finalizarJuez(pid_t jueces[MAX_PARTICIPANTES/MAX_JUGADORES_JUEZ +1]){
	
	matarJuecesHijos(jueces);
}

//Manejador de señales del cantador
void manejadorCantador(int sigNum){

    int idMemoria; 
	Sala* sala; 
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	int i;
	int semaforoGanador = crearSemaforo("ganador",1);
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoIdentificadores = crearSemaforo("identificadores",1);
	int semaforoParticipantes = crearSemaforo("participantes",1);
	int semaforoBombo = crearSemaforo("bombo",1);
	int semaforoAforo = crearSemaforo("aforo",MAX_PARTICIPANTES);

	switch(sigNum){

		case SIGGANADOR:{
			printf("\nYa hay un Ganador...\n");
			if(getSemaforo(semaforoParticipantes)==0)
				modificarSemaforo(semaforoParticipantes, 1);
			if(getSemaforo(semaforoIdentificadores)==0)
				modificarSemaforo(semaforoIdentificadores, 1);
			if(getSemaforo(semaforoBombo)==0)
				modificarSemaforo(semaforoBombo, 1);
				
			bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
				enviarSenal(sala->ganador, SIGFELICITACIONES);
				printf("\nFelicitando al Ganador...\n");
				desmarcarTarjetas(sala->tarjetas);
				inicializarBombo(sala->bombo);

				for(i = 0; i < sala->participantes; i++)
					enviarSenal(sala->identificadores[i],SIGGANADOR);
				printf("Avisando a los jugadores que el juego ha acabado...");
				modificarSemaforo(semaforoAforo, sala->participantes);
			subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
			
			printf("\nEn 25 seg comenzara otra partida si hay participantes en la sala...\n");
			alarm(25);
            pause();
			
		}break;

		case SIGIMPOSTOR:{
			bajarSemaforo(semaforoParticipantes);
			bajarSemaforo(semaforoIdentificadores);
				for(i = 0; i < sala->participantes; i++)
					enviarSenal(sala->identificadores[i],SIGIMPOSTOR);
			subirSemaforo(semaforoParticipantes);
			subirSemaforo(semaforoIdentificadores);
		}break;

		case SIGBINGO:{
			bajarSemaforo(semaforoParticipantes);
			bajarSemaforo(semaforoIdentificadores);
				for(i = 0; i < sala->participantes; i++)
					enviarSenal(sala->identificadores[i],SIGBINGO);
            subirSemaforo(semaforoParticipantes);
			subirSemaforo(semaforoIdentificadores);
            pause();
		}break;
		
		case SIGALRM:{
			bajarSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
				modificarSemaforo(semaforoAforo, 0);
				if(sala->participantes==0)
					finalizarCantador(sala);
			subirSemaforoDeSala(semaforoGanador, semaforoTarjetas, semaforoCantor, semaforoJueces, semaforoIdentificadores, semaforoParticipantes, semaforoBombo);
		}break;
		
		case SIGINT:{
			printf("\nFinalizando cantador...\n");
            finalizarCantador(sala);
		}break;
	}
}

//Manejador de señales de los jueces
void manejadorJuez(int sigNum){

	int idMemoria; 
	Sala* sala; 
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	int juezDesignado = designarJuez(sala->ganador, sala->identificadores);
	
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCantor = crearSemaforo("cantor",1);
	int semaforoJueces = crearSemaforo("jueces",1);
	int semaforoGanador = crearSemaforo("ganador",1);

	switch(sigNum){

		case SIGBINGO:{
			bajarSemaforo(semaforoJueces);
				printf("\nSe va a avisar al juez designado[%d]\n",juezDesignado);
				enviarSenal(sala->jueces[juezDesignado], SIGVERIFICAR);
				sleep(1);
			if(juezDesignado!=0)
				subirSemaforo(semaforoJueces);
			printf("\nSe aviso al juez: %d\n",sala->jueces[juezDesignado]);
		}break;

		case SIGVERIFICAR:{
			
			printf("\nVerificando...\n");
			sleep(1);
			if(juezDesignado==0)
				subirSemaforo(semaforoJueces);
			bajarSemaforo(semaforoTarjetas);
			bajarSemaforo(semaforoCantor);
			bajarSemaforo(semaforoJueces);
			modificarSemaforo(semaforoGanador,1);
			
				if(comprobarBingo(sala->ganador,sala->tarjetas,sala->bombo)){
					if(juezDesignado==0)
						enviarSenal(sala->cantor, SIGGANADOR);
					else
						enviarSenal(sala->jueces[0], SIGGANADOR);
				}else{
					sala->ganador = 1;
					if(juezDesignado==0)
						enviarSenal(sala->cantor, SIGIMPOSTOR);
					else
						enviarSenal(sala->jueces[0], SIGIMPOSTOR);
				}
			printf("\nVerificacion finalizada...\n");
            sleep(1);
            subirSemaforo(semaforoTarjetas);
			subirSemaforo(semaforoCantor);
			subirSemaforo(semaforoJueces);
			
			if(getpid()==sala->jueces[0]){
				printf("\nJueces listos...\n");
				sleep(1);
			}
		}break;

		case SIGGANADOR:{
			bajarSemaforo(semaforoCantor);
				enviarSenal(sala->cantor, SIGGANADOR);
			subirSemaforo(semaforoCantor);
			printf("\nJueces listos...\n");
			sleep(1);
		}break;

		case SIGIMPOSTOR:{
			bajarSemaforo(semaforoCantor);
				enviarSenal(sala->cantor, SIGIMPOSTOR);
			subirSemaforo(semaforoCantor);
			printf("\nJueces listos...\n");
			sleep(1);
		}break;

		case SIGFINALIZAR:{
			bajarSemaforo(semaforoJueces);
				matarJuecesHijos(sala->jueces);
				sala->jueces[1]=1;
            subirSemaforo(semaforoJueces);
            enviarSenal(getpid(), SIGKILL);
		}break;
		
		case SIGINT:{
			if(getpid()==sala->jueces[0]){
				printf("\nFinalizando Juez...\n");
				enviarSenal(getpid(),SIGFINALIZAR);
			}
		}break;
	}
}

//Manejador de señales del postor
void manejadorPostor(int sigNum){

	switch(sigNum){

		case SIGBINGO:{
            printf("\nSe esta verificando un bingo...");
		}break;
		
		case SIGGANADOR:{
            printf("\nYa hay un ganador, el ha juego finalizado...\n");
            printf("\nProximo juego en 20 seg aproximadamente...\n");
		}break;
		
		case SIGIMPOSTOR:{
            printf("\nPuede continuar jugando, el bingo cantado era falso...");
		}break;

		case SIGFELICITACIONES:{
            printf("\n\n¡¡¡Felicitaciones has ganado...!!!\n\n");
		}break;
		
		case SIGINT:{
			printf("\nFinalizando Postor...\n");
			enviarSenal(getpid(),SIGKILL);
		}break;
	}
}

//Escucha los numeros de forma manual
void escucharNumerosManual(){
	
	int idCola;
	Mensaje mensaje;
	
	printf("\nEscuchando...\n");
	
	idCola = crearCola();
	
	while(1){
		msgrcv(idCola,(struct msgbuf *)&mensaje,sizeof(mensaje.bola),getpid(),0);
		printf("\nHa salido el: %d", mensaje.bola);
	}
}

//Verifica si una tarjeta está llena
int tarjetaLlena(Tarjeta tarjeta){
	int i,j;
	
	for(i=0; i < MAX_FILA; i++)
		for(j=0; j<MAX_COLUMNA; j++){
			if(tarjeta.numeros[i][j]>0)
			return 0;
		}
	return 1;
}

//Escucha los nuemros de forma automática, es decir, son marcados tan pronto son cantados
//y se canta bingo de forma automatica una vez que la tarjeta esté llena
void escucharNumerosAutomatico(G_Tarjeta *G_tarjeta){
	
	int semaforoTarjetas = crearSemaforo("tarjetas",1);
	int semaforoCola = crearSemaforo("cola",1);
	int idCola,idMemoria,i,j,numA,numB,bandera,numeroCantado;
	Mensaje mensaje;
	Sala *sala; 
	int posTarjetas[MAX_TARJETAS_JUGADOR];
	GdkColor color;
	
	idMemoria = crearMemoria(sizeof(Sala));
	sala = obtenerMemoria(idMemoria);
	
	bajarSemaforo(semaforoTarjetas);
		obtenerPosicionDeTarjetas(getpid(), posTarjetas, sala->tarjetas);
	subirSemaforo(semaforoTarjetas);
	
	idCola = crearCola();
	
	printf("\nEscuchando...\n");
	
	idCola = crearCola();
	gdk_color_parse ("blue", &color);
	
	while(1){
		bajarSemaforo(semaforoCola);
		
			msgrcv(idCola,(struct msgbuf *)&mensaje,sizeof(mensaje.bola),getpid(),0);
			numeroCantado = mensaje.bola;
		
		subirSemaforo(semaforoCola);
		
		printf("\nHa salido el: %d", numeroCantado);
		
		sleep(10);
		
		bandera = 0;
		
		for(i = 1; i<MAX_FILA+1 && !bandera; i++){
			for(j=0; j<MAX_COLUMNA && !bandera; j++){
				numA = atoi(gtk_button_get_label(GTK_BUTTON(G_tarjeta->G_numeros[i][j])));
				numB = atoi(gtk_button_get_label(GTK_BUTTON(G_tarjeta->G_numeros[i][j+6])));
				
				if(numeroCantado == numA){
					gtk_widget_activate(G_tarjeta->G_numeros[i][j]);
					bandera++;
					break;
				}
				if(numeroCantado == numB){
					gtk_widget_activate(G_tarjeta->G_numeros[i][j+6]);
					bandera++;
					break;
				}
			}
		}
		
		bajarSemaforo(semaforoTarjetas);
		
			for(i=0; i<MAX_TARJETAS_JUGADOR; i++){
	
				if(tarjetaLlena(sala->tarjetas[posTarjetas[i]])){
					
					printf("\nLa trjeta %d está llena\n",posTarjetas[i]);
					gtk_widget_activate(G_tarjeta->G_numeros[0][5]);
					break;
				}
					
			}
		
		subirSemaforo(semaforoTarjetas);
	}
}

