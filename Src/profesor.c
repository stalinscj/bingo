#include <stdio.h>
#include "libsem.h"

void Buscar_examenes(){};

int main(int argc, char **argv)
{
	int semProf = Semaph_Create("profesor",6);
	int semEst = Semaph_Create("estudiante",1);
	int semTest = Semaph_Create("test",0);
	
	int i=0;
	while(i<7){
		Semaph_Down(semTest);
		if(i==6)
			break;
		printf("\n Soy prof. voy a buscar 6 examenes\n");
		Buscar_examenes();
		Semaph_Set(semProf,6);
		printf("\n Soy prof. ya busque los examenes \n");
		i++;
	}
	
	printf("\n Soy prof. me voy\n");
	
	Semaph_Destroy("profesor",semProf);
	Semaph_Destroy("estudiante",semEst);
	Semaph_Destroy("test",semTest);
	
	return 0;
}

