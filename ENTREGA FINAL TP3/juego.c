#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "papeleo.h"
#include "dia_en_la_uni.h"


int main(){
    srand((unsigned)time(NULL));
    juego_t juego;

    char personaje_tp1;
    preguntar_personaje(&personaje_tp1);
	inicializar_juego(&juego, personaje_tp1);
    imprimir_terreno(juego);

    while(estado_juego(juego) == 0){
		realizar_jugada(&juego);
	}

    return 0;

}