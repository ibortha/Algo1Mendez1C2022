#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "papeleo.h"

#define DIMENSION_LVL1   22
#define DIMENSION_LVL2   17
#define DIMENSION_LVL3   12
#define DIMENSION_MAXIMA 25

#define NO_ENCONTRADO -1
#define MAX_OBJETOS  500
#define MAX_INTERRUPTOR 5

#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3

#define PERDIDO -1
#define JUGANDO  0
#define GANADO   1

const bool INTERRUPTOR_APRETADO_NO = false;
const bool INTERRUPTOR_APRETADO_SI = true;
const bool PAPELEO_RECOLECTADO_NO =  false;
const bool PAPELEO_RECOLECTADO_SI =  true;

#define EMOJI_MIKE        "\U0001f47d"
#define EMOJI_MEDIAS      "\U0001f9e6"
#define EMOJI_EXTINTOR    "\U0001f9ef"
#define EMOJI_FUEGO       "\U0001f525"
#define EMOJI_PARED       "\U0001f9f1"
#define EMOJI_MARTILLO    "\u2692\uFE0F"
#define EMOJI_BOTELLA     "\U0001f37e"
#define EMOJI_INTERRUPTOR "\U0001f579\uFE0F"

// PERSONAJES
const char MIKE                   = 'W';
const char PERSONAJE_OLAF         = 'O';
const char PERSONAJE_STITCH       = 'S';
const char PERSONAJE_JASMIN       = 'J';
const char PERSONAJE_RAYO_MCQUEEN = 'R';
const char VACIO                  = ' ';

// OBSTACULOS
const char FUEGO         = 'F';
const char MEDIA         = 'M';
const char PARED         = '#';
const char OBSTACULO     = 'K';

// HERRAMIENTAS 
const char EXTINTOR       = 'D';
const char MARTILLO       = 'E';
const char BOTELLA_GRITO  = 'G';
const char INTERRUPTOR    = 'I';
const char PAPELEO        = 'P';
const char HERRAMIENTA    = 'H';

// VALORES POR NIVEL
const int CANT_MOV_INICIALES_1 = 40;
const int CANT_PAPELEOS_1      = 2;
const int CANT_FUEGOS_1        = 10;
const int CANT_MEDIAS_1        = 5;
const int CANT_BOTELLAS_1      = 4;
const int CANT_INTERRUPTORES_1 = 1;
const int CANT_MARTILLOS_1     = 4;
const int CANT_EXTINTORES_1    = 4;

const int CANT_MOV_INICIALES_2 = 30;
const int CANT_PAPELEOS_2      = 3;
const int CANT_FUEGOS_2        = 5;
const int CANT_MEDIAS_2        = 4;
const int CANT_BOTELLAS_2      = 3;
const int CANT_INTERRUPTORES_2 = 1;
const int CANT_MARTILLOS_2     = 5;
const int CANT_EXTINTORES_2    = 2;

const int CANT_MOV_INICIALES_3 = 20;
const int CANT_PAPELEOS_3      = 4;
const int CANT_FUEGOS_3        = 3;
const int CANT_MEDIAS_3        = 3;
const int CANT_BOTELLAS_3      = 2;
const int CANT_INTERRUPTORES_3 = 0;
const int CANT_MARTILLOS_3     = 6;
const int CANT_EXTINTORES_3    = 2;

// MOVIMIENTO
const char ARRIBA            = 'W'; 
const char ABAJO             = 'S';
const char DERECHA           = 'D';
const char IZQUIERDA         = 'A';
const char USAR_MARTILLO     = 'Z';
const char USAR_EXTINTOR     = 'C';
const char ROTAR_HORARIO     = 'E';
const char ROTAR_ANTIHORARIO = 'Q';

const int MOVIMIENTOS_POR_BOTELLA = 7;
const int MOVIMIENTOS_POR_MEDIA   = 10;
const int MOVIMIENTOS_POR_FUEGO   = 0;
const int MOVIMIENTOS_REALIZADOS_INICIALES = 0;

// VALOR DIVISIBLE DE R
const int R_NIVEL_1 = 7;
const int R_NIVEL_2 = 5;
const int R_NIVEL_3 = 3;

// CANTIDAD DE PAREDES RANDOM
const int PAREDES_RANDOM_1 = 40;
const int PAREDES_RANDOM_2 = 30;
const int PAREDES_RANDOM_3 = 20;

// Devuelve TRUE si las dos coordenadas recibidas son iguales.
bool son_coordenadas_iguales(coordenada_t a, coordenada_t b) {
    return(a.col == b.col && a.fil == b.fil);
}

// Dado al vector de papeleos y una posicion en este vector recibida, esta funcion devuelve el id_papeleo
// determinado en forma de caracter.
char numero_papeleo(juego_t juego, int posicion_niveles, int posicion_vector_papeleos){
    int numero = juego.niveles[posicion_niveles].papeleos[posicion_vector_papeleos].id_papeleo;
    char entero_en_caracter = (char)(numero + '0');
    return entero_en_caracter;
}

// Pre:  Recibe dos enteros positivos.
// Post: Devuelve devuelve una coordenada de tipo coordenada_t con valores aleatorios 
//      entre los enteros recibidos anteriormente
coordenada_t coordenada_aleatoria(int dimension_terreno){
    coordenada_t coordenada_random;
    coordenada_random.fil = rand() % dimension_terreno;
	coordenada_random.col = rand() % dimension_terreno;
return coordenada_random;
}

// Pre: Recibe un iterador y un tope positivos. Recibe una posicion
// Post: Devuelve TRUE si el iterador es menor al tope, y si la posicion recibida es igual a -1.
bool menor_tope_no_encontrado(int iterador, int tope, int posicion_objeto_en_vector){
    return((iterador < tope) && (posicion_objeto_en_vector == NO_ENCONTRADO));
}

// Recibe el  nivel y una coordenada
// Devuelve TRUE si la coordenada recibida es la misma que la posicion inicial de Mike.
bool es_posicion_mike(nivel_t nivel, coordenada_t coordenada_a_comparar){
    return((nivel.pos_inicial_jugador.col == coordenada_a_comparar.col) 
    && (nivel.pos_inicial_jugador.fil == coordenada_a_comparar.fil) );
}

// Funcion que recibe el vector objetos, un tope positivo y una coordenada.
// Recorre el vector hasta que haya un objeto con la misma coordenada recibida y devuelve esa posicion del vector.
int buscar_por_coordenada_objeto(objeto_t objetos[MAX_OBJETOS], int tope, coordenada_t posicion){
    int posicion_objeto_en_vector = NO_ENCONTRADO;
    int i = 0;

    while(menor_tope_no_encontrado(i, tope, posicion_objeto_en_vector)){
        if(son_coordenadas_iguales((objetos[i]).posicion, posicion)){
            posicion_objeto_en_vector = i;

        }
        i++;
    }
    return posicion_objeto_en_vector;
}

// Funcion que recibe el vector papeleos, un tope positivo y una coordenada.
// Recorre el vector hasta que haya un papeleo con la misma coordenada recibida y devuelve esa posicion del vector.
int buscar_por_coordenada_papeleo(papeleo_t papeleos[MAX_PAPELEOS], int tope, coordenada_t posicion){
    int posicion_papeleo_en_vector = NO_ENCONTRADO;
    int i = 0;

    while(menor_tope_no_encontrado(i, tope, posicion_papeleo_en_vector)){
        if(son_coordenadas_iguales((papeleos[i]).posicion, posicion) && (papeleos[i].recolectado == PAPELEO_RECOLECTADO_NO)){
            posicion_papeleo_en_vector = i;
        }
        i++;
    }
    return posicion_papeleo_en_vector;
}

// Funcion que recibe el vector paredes, un tope positivo y una coordenada.
// Recorre el vector hasta que haya una pared con la misma coordenada recibida y devuelve esa posicion del vector.
int buscar_por_coordenada_pared(coordenada_t paredes[MAX_PAREDES], int tope, coordenada_t posicion){
    int posicion_pared_en_vector = NO_ENCONTRADO;
    int i = 0;

    while(menor_tope_no_encontrado(i, tope, posicion_pared_en_vector)){
        if(son_coordenadas_iguales((paredes[i]), posicion)){
            posicion_pared_en_vector = i;

        }
        i++;
    }
    return posicion_pared_en_vector;
}

// Pre: Recibe una coordenada, el vector paredes y un tope positivo.
// Post: Recorre el vector paredes, devuelve TRUE si la coordenada recibida es la misma que una pared.
bool es_coordenada_igual_paredes(coordenada_t coordenada_comparar, coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    bool existe = false;
    int i = 0;

    while ((i < tope_paredes) &&  (existe == false)){
        if (son_coordenadas_iguales(coordenada_comparar, paredes[i])) {
            existe = true;
        }
        i++;
    }
    return existe;
}

// Pre: Recibe una coordenada, el vector paredes y un tope positivo.
// Post: Recorre el vector paredes, devuelve TRUE si la coordenada recibida es la misma que una pared.
bool es_coordenada_igual_objeto(coordenada_t coordenada_comparar, objeto_t objetos[MAX_OBJETOS], int tope_objetos){
    bool existe = false;
    int i = 0;

    while ((i < tope_objetos) &&  (existe == false)){
        if (son_coordenadas_iguales(coordenada_comparar, objetos[i].posicion)) {
            existe = true;
        }
        i++;
    }
    return existe;
}

// Funcion que devuelve TRUE si hay una pared/objeto/papeleo en la cooredenada recibida del nivel indicado recibido.
bool existe_posicion_objetos(coordenada_t coordenada_comparar, nivel_t nivel){
    bool existe = false;

    if((es_coordenada_igual_paredes(coordenada_comparar, nivel.paredes, nivel.tope_paredes))
    || ((buscar_por_coordenada_objeto(nivel.obstaculos, nivel.tope_obstaculos, coordenada_comparar)) > NO_ENCONTRADO)
    || ((buscar_por_coordenada_objeto(nivel.herramientas, nivel.tope_herramientas, coordenada_comparar)) > NO_ENCONTRADO)
    || ((buscar_por_coordenada_papeleo(nivel.papeleos, nivel.tope_papeleos, coordenada_comparar)) > NO_ENCONTRADO)){
        existe = true;
    }
    return existe;
} 

//  Procedimiento que recibe al jugador por referencia, e inicializa todos sus valores 
// dependiendo del nivel y del personaje del tp1 recibido.
void inicializar_jugador(juego_t* juego, jugador_t* ref_jugador, char personaje_tp1){
    int nivel_actual = juego->nivel_actual;
    if(nivel_actual == NIVEL_1){
        (*ref_jugador).movimientos = CANT_MOV_INICIALES_1;
        (*ref_jugador).movimientos_realizados = MOVIMIENTOS_REALIZADOS_INICIALES;
        (*ref_jugador).martillos = CANT_MARTILLOS_1;
        (*ref_jugador).extintores =CANT_EXTINTORES_1;
        (*ref_jugador).ahuyenta_randall = INTERRUPTOR_APRETADO_NO;
        (*ref_jugador).posicion = juego->niveles[0].pos_inicial_jugador;
    }else if(nivel_actual == NIVEL_2){
        (*ref_jugador).movimientos += CANT_MOV_INICIALES_2;
        (*ref_jugador).movimientos_realizados = MOVIMIENTOS_REALIZADOS_INICIALES;
        (*ref_jugador).martillos += CANT_MARTILLOS_2;
        (*ref_jugador).extintores += CANT_EXTINTORES_2;
        (*ref_jugador).ahuyenta_randall = INTERRUPTOR_APRETADO_NO;
        (*ref_jugador).posicion =  juego->niveles[1].pos_inicial_jugador;
    }else if(nivel_actual == NIVEL_3){
        (*ref_jugador).movimientos += CANT_MOV_INICIALES_3;
        (*ref_jugador).movimientos_realizados = MOVIMIENTOS_REALIZADOS_INICIALES;
        (*ref_jugador).martillos += CANT_MARTILLOS_3;
        (*ref_jugador).extintores += CANT_EXTINTORES_3;
        (*ref_jugador).ahuyenta_randall = INTERRUPTOR_APRETADO_SI;
        (*ref_jugador).posicion =  juego->niveles[2].pos_inicial_jugador;
    }
    if (personaje_tp1 == PERSONAJE_JASMIN){
        (*ref_jugador).martillos++; 
    }else if (personaje_tp1 == PERSONAJE_RAYO_MCQUEEN){
        if(nivel_actual == NIVEL_1){
            (*ref_jugador).movimientos += 10;
        }
    }
}

// Devuelve TRUE si la coordenada recibida en el nivel recibido se encuentra adyacente a una pared.
bool es_fuego_proximo_pared(coordenada_t coordenada_comparar, nivel_t nivel){

    bool es_proximo = false;
    coordenada_comparar.col++ ;
    if(es_coordenada_igual_paredes(coordenada_comparar, nivel.paredes, nivel.tope_paredes)){
        es_proximo = true;
    }
    coordenada_comparar.col--;
    coordenada_comparar.col--;

    if(es_coordenada_igual_paredes(coordenada_comparar, nivel.paredes, nivel.tope_paredes)){
        es_proximo = true;
    }
    coordenada_comparar.col++;;
    coordenada_comparar.fil++;;
    if(es_coordenada_igual_paredes(coordenada_comparar, nivel.paredes, nivel.tope_paredes)){
        es_proximo = true;
    }
    coordenada_comparar.fil--;
    coordenada_comparar.fil--;

    if(es_coordenada_igual_paredes(coordenada_comparar, nivel.paredes, nivel.tope_paredes)){
        es_proximo = true;
    }
    return es_proximo;
}

// Devuelve TRUE si la cordenada recibida es parte del borde de la matriz.
bool es_pared_borde(coordenada_t coordenada_comparar, int dimension_terreno){
    dimension_terreno--;
    return((coordenada_comparar.col == 0)
         || (coordenada_comparar.fil == 0) 
         || (coordenada_comparar.col == dimension_terreno) 
         || (coordenada_comparar.fil == dimension_terreno));
}

//
// Pre: recibe el nivel, un tope mayor a 0, un contador y un TIPO
// Post: devuelve el vector cargado con el TIPO recibido.
//
void llenar_vector_obstaculos(nivel_t* nivel, int tope, int* contador_o, char tipo){
    int iterador;
    int posicion;
    

    for( iterador = 0; iterador <= tope; iterador++){
        posicion = *(contador_o) + iterador;
        nivel->obstaculos[posicion].tipo = tipo;

    }
    *(contador_o) = posicion;
}

// Procedimiento que carga el vector obstaculos deppendiendo el nivel recibido con los obstaculos determinados 
// y les asigna una posicion aleatoria.
void cargar_posicion_obstaculos(nivel_t* nivel, int dimension_terreno, int numero_nivel, int fuegos,  int medias, char personaje_tp1){
    
    int cant_fuegos = fuegos;
    int cant_medias = medias;
    int contador_o = 0;
    if (personaje_tp1 == PERSONAJE_OLAF){
        if (numero_nivel == NIVEL_1 ){
            cant_fuegos = fuegos - 2;

        }else if(numero_nivel == NIVEL_2){
            cant_fuegos = fuegos - 1;
        }
    }
    int tope_temporal = (cant_fuegos + medias);
    llenar_vector_obstaculos(nivel, cant_fuegos, &contador_o, FUEGO);
    llenar_vector_obstaculos(nivel, cant_medias, &contador_o, MEDIA);
    coordenada_t coordenada_temporal;
    
    for(int i = 0; i< tope_temporal; i++){

        bool coordenada_incorporada = false;
        while(coordenada_incorporada == false){
           
            coordenada_temporal = coordenada_aleatoria(dimension_terreno);

            if(!(existe_posicion_objetos(coordenada_temporal, *nivel)) 
            && (es_fuego_proximo_pared(coordenada_temporal, *nivel))
            && !(es_posicion_mike(*nivel, coordenada_temporal))){
                nivel->obstaculos[i].posicion = coordenada_temporal;
                coordenada_incorporada = true;
                nivel->tope_obstaculos++;
            }
        }
    }
}


// Pre: recibe el nivel, un tope mayor a 0, un contador y un TIPO
// Post: devuelve el vector cargado con el TIPO recibido.
void llenar_vector_herramientas(nivel_t* nivel, int tope, int* contador_h, char tipo){
    int iterador;
    int posicion;
    for( iterador = 0; iterador <= tope; iterador++){
        posicion = *(contador_h) + iterador;
        nivel->herramientas[posicion].tipo = tipo;

    }
    *(contador_h) = posicion;
}

// Procedimiento que carga el vector herramientas deppendiendo el nivel recibido con las herramientas determinadas 
// y le asigna una posicion aleatoria.
void cargar_posicion_herramientas(nivel_t* nivel, int dimension_terreno, int botellas, int interruptores){

    int tope_temporal = (botellas + interruptores);
    int contador_h = 0;
    llenar_vector_herramientas(nivel, botellas, &contador_h, BOTELLA_GRITO);
    llenar_vector_herramientas(nivel, interruptores, &contador_h, INTERRUPTOR);
    coordenada_t coordenada_temporal;

    for(int i = 0; i< tope_temporal; i++){

        bool coordenada_incorporada = false;
        while(coordenada_incorporada == false){
           
            coordenada_temporal = coordenada_aleatoria(dimension_terreno);

            if(!(existe_posicion_objetos(coordenada_temporal, *nivel)) && !(es_posicion_mike(*nivel, coordenada_temporal))){
                nivel->herramientas[i].posicion = coordenada_temporal;
                coordenada_incorporada = true;
                nivel->tope_herramientas++;
            }
        }
    }
}   

// Pre: recibe el nivel y un tope mayor a 0.
// Post: devuelve el vector cargado con cada id_papeleo.
void llenar_vector_papeleos(nivel_t* nivel, int tope){
    int iterador; 

    for( iterador = 0; iterador <= tope; iterador++){
        nivel->papeleos[iterador].id_papeleo = iterador + 1;
    }
}

// Procedimiento que carga el vector papeleos deppendiendo el nivel recibido con la cantidad de papeleos determinados 
// y le asigna una posicion aleatoria.
void cargar_posicion_papeleos(nivel_t* nivel, int dimension_terreno, int numero_nivel, int papeleos, char personaje_tp1){

    if (personaje_tp1 == PERSONAJE_STITCH){
        if (numero_nivel == NIVEL_3){
            papeleos--;
        }
    }
    int tope_temporal = papeleos;
    llenar_vector_papeleos(nivel, papeleos);
    coordenada_t coordenada_temporal;
 
    for(int i = 0; i< tope_temporal; i++){
        bool coordenada_incorporada = false;
        while(coordenada_incorporada == false){  
            coordenada_temporal = coordenada_aleatoria(dimension_terreno);
            if(!(existe_posicion_objetos(coordenada_temporal, *nivel)) && !(es_posicion_mike(*nivel, coordenada_temporal))){
                nivel->papeleos[i].posicion = coordenada_temporal;
                nivel->papeleos[i].recolectado = PAPELEO_RECOLECTADO_NO;
                coordenada_incorporada = true;
                nivel->tope_papeleos++;
            }
        }
    }
}

// Recibo el nivel, una coordenada y un objeto por referencia.
// En el caso de que haya un objeto con la misma coordenada recibida, esta funcion devuele el tipo del objeto
// que se encuentra en esa coordenada, y la posicion en el vector del mismo.
int hay_objeto_vector(nivel_t* nivel, coordenada_t coordenada_temporal, char* objeto_vector){
    int posicion = NO_ENCONTRADO;
    int posicion_obstaculo = buscar_por_coordenada_objeto(nivel->obstaculos, nivel->tope_obstaculos, coordenada_temporal);
    int posicion_herramienta = buscar_por_coordenada_objeto(nivel->herramientas, nivel->tope_herramientas, coordenada_temporal);
    int posicion_papeleo = buscar_por_coordenada_papeleo(nivel->papeleos, nivel->tope_papeleos, coordenada_temporal);

    if (posicion_obstaculo != NO_ENCONTRADO){
        posicion = posicion_obstaculo;
        (*objeto_vector) = OBSTACULO;
    }else if(posicion_herramienta != NO_ENCONTRADO){
        posicion = posicion_herramienta;
        (*objeto_vector) = HERRAMIENTA;
    }else if(posicion_papeleo != NO_ENCONTRADO){
        posicion = posicion_papeleo;
        (*objeto_vector) = PAPELEO;
    }
    return posicion;
}

/* 
 * Inicializará un nivel, cargando toda la información inicial, las paredes,
 * los objetos, los papeleos y la posición inicial del jugador en dicho nivel.
 */
void inicializar_nivel(nivel_t* nivel, int nivel_actual, char personaje_tp1){
    nivel->tope_paredes = 0;
    nivel->tope_obstaculos = 0;
    nivel->tope_herramientas = 0;
    nivel->tope_papeleos = 0;
    nivel->papeleos->id_papeleo = 0;
    nivel->papeleos->recolectado = false;
    nivel->papeleos->posicion.col = 0;
    nivel->papeleos->posicion.fil = 0;
    nivel->paredes->col = 0;
    nivel->paredes->fil = 0;

    int dimension_terreno;
    if( nivel_actual == NIVEL_1 ){
        dimension_terreno = DIMENSION_LVL1;
    }else if( nivel_actual == NIVEL_2){
        dimension_terreno = DIMENSION_LVL2;
    }else if ( nivel_actual == NIVEL_3 ){
        dimension_terreno = DIMENSION_LVL3;
    }
    if(nivel_actual == NIVEL_1){
        obtener_paredes(NIVEL_1, nivel->paredes, &(nivel->tope_paredes), &(nivel->pos_inicial_jugador));
        cargar_posicion_obstaculos(nivel, dimension_terreno, nivel_actual, CANT_FUEGOS_1, CANT_MEDIAS_1, personaje_tp1);
        cargar_posicion_herramientas(nivel, dimension_terreno, CANT_BOTELLAS_1, CANT_INTERRUPTORES_1);
        cargar_posicion_papeleos(nivel, dimension_terreno, nivel_actual, CANT_PAPELEOS_1, personaje_tp1);
    }else if(nivel_actual == NIVEL_2){
        obtener_paredes(NIVEL_2, nivel->paredes, &(nivel->tope_paredes), &(nivel->pos_inicial_jugador));
        cargar_posicion_obstaculos(nivel, dimension_terreno, nivel_actual, CANT_FUEGOS_2, CANT_MEDIAS_2, personaje_tp1);
        cargar_posicion_herramientas(nivel, dimension_terreno, CANT_BOTELLAS_2, CANT_INTERRUPTORES_2);
        cargar_posicion_papeleos(nivel, dimension_terreno, nivel_actual, CANT_PAPELEOS_2, personaje_tp1);   
    }else if(nivel_actual == NIVEL_3){
        obtener_paredes(NIVEL_3, nivel->paredes, &(nivel->tope_paredes), &(nivel->pos_inicial_jugador));
        cargar_posicion_obstaculos(nivel, dimension_terreno, nivel_actual, CANT_FUEGOS_3, CANT_MEDIAS_3, personaje_tp1);
        cargar_posicion_herramientas(nivel, dimension_terreno, CANT_BOTELLAS_3, CANT_INTERRUPTORES_3);
        cargar_posicion_papeleos(nivel, dimension_terreno, nivel_actual, CANT_PAPELEOS_3, personaje_tp1);
    }
}

/*
 * Inicializará el juego, cargando toda la información inicial, los datos del jugador, 
 * el personaje resultado del tp anterior, y los 3 niveles. El campo "nivel_actual"
 * comienza en 1.
 */
void inicializar_juego(juego_t* juego, char personaje_tp1){
    juego->nivel_actual = NIVEL_1;
    juego->jugador.movimientos_realizados = 0;
    juego->jugador.martillos = 0;
    juego->jugador.extintores = 0;
    juego->jugador.movimientos = 0;

    juego->personaje_tp1 = personaje_tp1;
    for(int i = 0; i < 3; i++){
        int nivel = i + 1;
        inicializar_nivel(&(juego->niveles[i]), nivel, personaje_tp1);
    }
    inicializar_jugador(juego, &(juego->jugador), personaje_tp1);
}

// Recibe un movimiento, devuelve TRUE si este es valido.
bool es_movimiento_valido(char movimiento){
    return((movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == DERECHA) ||(movimiento == IZQUIERDA)
    || (movimiento == USAR_EXTINTOR) ||(movimiento == USAR_MARTILLO) || (movimiento == ROTAR_ANTIHORARIO) ||(movimiento == ROTAR_HORARIO));
}


// Recibo el caracter del movimiento por referencia
// Procedimiento que le pide al usuario ingresar por consola el proximo movimiento a realizar.
// Si este no es valido se lo vuelve a pedir.
void pedir_movimiento(char* ref_movimiento){
    printf("Cual es tu proxima accion?: ");
    printf("[%c]Derecha [%c]Izquierda\n[%c]Utilizar Martillo [%c]Utilizar Extintor [%c]Rotar Horariamente [%c]Rotar Antihorariamente \n", DERECHA, IZQUIERDA, USAR_MARTILLO, USAR_EXTINTOR, ROTAR_HORARIO, ROTAR_ANTIHORARIO);
    scanf("  %c", ref_movimiento);
    
    while(!(es_movimiento_valido(*ref_movimiento))){
        printf("\n Por favor ingrese correctamente una de las siguientes 4 opciones.\n");
        printf("[%c]Derecha [%c]Izquierda\n [%c]Utilizar Martillo [%c]Utilizar Extintor [%c]Rotar Horariamente [%c]Rotar Antihorariamente\n", DERECHA, IZQUIERDA, USAR_MARTILLO, USAR_EXTINTOR, ROTAR_HORARIO, ROTAR_ANTIHORARIO);
        scanf("  %c", ref_movimiento);
    }
}

// Recibe un movimiento, devuelve TRUE si este es valido.
bool es_direccion_valida(char movimiento){
    return((movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == DERECHA) ||(movimiento == IZQUIERDA));
}

// Recibo el caracter del movimiento por referencia
// Procedimiento que le pide al usuario ingresar por consola la direccion del movimiento a realizar.
// Si este no es valido se lo vuelve a pedir.
void pedir_direccion_accion(char* ref_movimiento){
    printf("Hacia donde queres martillar/extinguir?: ");
    printf("[%c]Arriba [%c]Abajo [%c]Derecha [%c]Izquierda \n", ARRIBA, ABAJO, DERECHA, IZQUIERDA);
    scanf(" %c", ref_movimiento);
    
    while(!(es_direccion_valida(*ref_movimiento))){
        printf("Por favor, ingresa una direccion valida\n");
        printf("Hacia donde queres martillar/extinguir?: ");
        printf("[%c]Arriba [%c]Abajo [%c]Derecha [%c]Izquierda\n", ARRIBA, ABAJO, DERECHA, IZQUIERDA);
        scanf(" %c", ref_movimiento);
    }
}

// Recibo el vector objetos, una posicion del vector y un tope positivo.
// Elimino del vector el objeto en la posicion indicada.
void eliminar_objeto_vector(objeto_t objetos[MAX_OBJETOS], int posicion, int* tope){
    // objetos[posicion] = objetos[((*tope) - 1)];
    // (*tope)--;
    int tope_temporal = *tope;
    tope_temporal--;
    for(int i = posicion; i <= tope_temporal; i++){
        objetos[i] = objetos[i+1];
    }
    (*tope)--;
}

// Recibo el vector paredes, una posicion del vector y un tope positivo.
// Elimino del vector la pared en la posicion indicada.
void eliminar_pared_vector(coordenada_t paredes[MAX_PAREDES], int posicion, int* tope){
    // paredes[posicion] = paredes[((*tope) - 1)];
    // (*tope)--;
    int tope_temporal = *tope;
    tope_temporal--;
    for(int i = posicion; i <= tope_temporal; i++){
        paredes[i] = paredes[i+1];
    }
    (*tope)--;
}


// Recibo el nivel y una coordenada.
// Recorro el vector paredes hasta que haya una pared con la coordenada recibida.
// Elimino la pared en esa posicion del vector.
void eliminar_pared(nivel_t* nivel, coordenada_t coordenada_pared_eliminar){
    int posicion_pared_vector = NO_ENCONTRADO;
        posicion_pared_vector = buscar_por_coordenada_pared(nivel->paredes, nivel->tope_paredes, coordenada_pared_eliminar);
        eliminar_pared_vector(nivel->paredes, posicion_pared_vector, &(nivel->tope_paredes));
}

// Recibo el nivel y una coordenada.
// Recorro el vector objetos hasta que haya un fuego con la coordenada recibida.
// Elimino el fuego en esa posicion del vector.
void eliminar_fuego_vector(nivel_t* nivel, coordenada_t coordenada_fuego_eliminar){
    int posicion_fuego_vector = NO_ENCONTRADO;
        posicion_fuego_vector = buscar_por_coordenada_objeto(nivel->obstaculos, nivel->tope_obstaculos, coordenada_fuego_eliminar);
        eliminar_objeto_vector(nivel->obstaculos, posicion_fuego_vector, &(nivel->tope_obstaculos));
}

// Recibiendo un movimiento y una coordenada.
// Esta funcion que devuelve TRUE si segun el movimiento se encuentra un FUEGO en esa posicion. De lo contrario devuelve FALSE
bool hay_fuego_direccion(juego_t* juego, char direccion, coordenada_t* coordenada_eliminar, int dimension_terreno, int posicion_niveles){
    bool hay_fuego = false;
    if(direccion == ABAJO){
        coordenada_eliminar->fil++;
        if(es_coordenada_igual_objeto(*coordenada_eliminar, juego->niveles[posicion_niveles].obstaculos, juego->niveles[posicion_niveles].tope_obstaculos)){
            hay_fuego = true;
        }
    }else if(direccion == ARRIBA){
        coordenada_eliminar->fil--;
        if(es_coordenada_igual_objeto(*coordenada_eliminar, juego->niveles[posicion_niveles].obstaculos, juego->niveles[posicion_niveles].tope_obstaculos)){
            hay_fuego = true;
        }
    }else if(direccion == DERECHA){
        coordenada_eliminar->col++;
        if(es_coordenada_igual_objeto(*coordenada_eliminar, juego->niveles[posicion_niveles].obstaculos, juego->niveles[posicion_niveles].tope_obstaculos)){
            hay_fuego = true;
        }
    }else if(direccion == IZQUIERDA){
        coordenada_eliminar->col--;
        if(es_coordenada_igual_objeto(*coordenada_eliminar, juego->niveles[posicion_niveles].obstaculos, juego->niveles[posicion_niveles].tope_obstaculos)){
            hay_fuego = true;
        }
    }
    return hay_fuego;
}

// Recibiendo un movimiento y una coordenada.
// Esta funcion que devuelve TRUE si segun el movimiento se encuentra una PARED en esa posicion. De lo contrario devuelve FALSE
bool hay_pared_direccion(juego_t* juego, char direccion, coordenada_t* coordenada_eliminar, int dimension_terreno, int posicion_niveles){
    bool hay_pared = false;
    if(direccion == ABAJO){
        coordenada_eliminar->fil++;
        if(es_coordenada_igual_paredes(*coordenada_eliminar, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes)){
            hay_pared = true;
        }
    }else if(direccion == ARRIBA){
        coordenada_eliminar->fil--;
        if(es_coordenada_igual_paredes(*coordenada_eliminar, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes)){
            hay_pared = true;
        }
    }else if(direccion == DERECHA){
        coordenada_eliminar->col++;
        if(es_coordenada_igual_paredes(*coordenada_eliminar, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes)){
            hay_pared = true;
        }
    }else if(direccion == IZQUIERDA){
        coordenada_eliminar->col--;
        if(es_coordenada_igual_paredes(*coordenada_eliminar, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes)){
            hay_pared = true;
        }
    }
    return hay_pared;
}

// Procedimiento que se ocupa de las interacciones con la utilizacion del martillo o del extintor
// segun el movimiento que reciba. Si se usa un martillo o un extintor, este se elimina del vector.
void realizar_accion(juego_t* juego, char movimiento, int dimension_terreno, int posicion_niveles){
    coordenada_t coordenada_eliminar = juego->jugador.posicion;

    if((movimiento == USAR_EXTINTOR) && (juego->jugador.extintores > 0)){
        pedir_direccion_accion(&movimiento);
        if(hay_fuego_direccion(juego, movimiento, &coordenada_eliminar, dimension_terreno, posicion_niveles)){
            eliminar_fuego_vector(&(juego->niveles[posicion_niveles]), coordenada_eliminar);
            juego->jugador.extintores--;
        }
    }
    else if((movimiento == USAR_MARTILLO) && (juego->jugador.martillos > 0)){
        pedir_direccion_accion(&movimiento);
        if(hay_pared_direccion(juego, movimiento, &coordenada_eliminar, dimension_terreno, posicion_niveles)){
            if(!(es_pared_borde(coordenada_eliminar, dimension_terreno))){
                eliminar_pared(&(juego->niveles[posicion_niveles]), coordenada_eliminar);
                juego->jugador.martillos--;
            }
        }
    }
    imprimir_terreno(*juego);
}

// Procedimiento que le añade los movimientos debidos al jugador por estar en la misma posicion de una Botella de Gritos.
void misma_posicion_botella(juego_t* juego){
    juego->jugador.movimientos = juego->jugador.movimientos + MOVIMIENTOS_POR_BOTELLA;
}
// Procedimiento que le elimina los movimientos al jugador por estar en la misma posicion de un Fuego.
void misma_posicion_fuego(juego_t* juego){
    juego->jugador.movimientos = MOVIMIENTOS_POR_FUEGO;

}
// Procedimiento que le resta los movimientos debidos al jugador por estar en la misma posicion de una Media.
void misma_posicion_media(juego_t* juego){
    juego->jugador.movimientos = juego->jugador.movimientos - MOVIMIENTOS_POR_MEDIA;
}
// Procedimiento que alterna el valor del interruptor por estar en la misma posicion del Interruptor Ahuyenta Randall.
void misma_posicion_interruptor(juego_t* juego){
    if(juego->jugador.ahuyenta_randall == INTERRUPTOR_APRETADO_SI){
        juego->jugador.ahuyenta_randall = INTERRUPTOR_APRETADO_NO;
    }else{
        juego->jugador.ahuyenta_randall = INTERRUPTOR_APRETADO_SI;
    }
}

//Procedimiento que recolecta el papeleo en la posicion recibida si el papeleo con id anterior ya fue recolectado.
void misma_posicion_papeleo(juego_t* juego, int posicion_papeleo_vector, int posicion_niveles){
    int id_papeleo_posicion = juego->niveles[posicion_niveles].papeleos[posicion_papeleo_vector].id_papeleo;
    int posicion_anterior = posicion_papeleo_vector-1;
    if(id_papeleo_posicion == 1){
        juego->niveles[posicion_niveles].papeleos[posicion_papeleo_vector].recolectado = PAPELEO_RECOLECTADO_SI;
    }else if(juego->niveles[posicion_niveles].papeleos[posicion_anterior].recolectado == PAPELEO_RECOLECTADO_SI){
        juego->niveles[posicion_niveles].papeleos[posicion_papeleo_vector].recolectado = PAPELEO_RECOLECTADO_SI;

    }
}

// Procedimiento principal de las interacciones del personaje con los elementos en la matriz
// Por cada posicion en el que se encuentre el personaje, si al mismo timepo hay un elemento
// se realizan las acciones debidas por cada elemento.
void interacciones_matriz_mike(juego_t* juego, coordenada_t* coordenada_temporal_mike, int dimension_terreno, int posicion_niveles){
    char objeto_vector = VACIO;
    char tipo_objeto = VACIO;
    int posicion_objeto = hay_objeto_vector(&(juego->niveles[posicion_niveles]), *coordenada_temporal_mike, &objeto_vector);

    if(es_coordenada_igual_paredes(*coordenada_temporal_mike, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes)){
        *coordenada_temporal_mike = juego->jugador.posicion;
        juego->jugador.movimientos++;
        juego->jugador.movimientos_realizados--;
    }else if(posicion_objeto != NO_ENCONTRADO){
        if(objeto_vector == HERRAMIENTA){
            tipo_objeto = juego->niveles[posicion_niveles].herramientas[posicion_objeto].tipo;
            if(tipo_objeto == BOTELLA_GRITO){
                misma_posicion_botella(juego);
            }else if(tipo_objeto == INTERRUPTOR){
                misma_posicion_interruptor(juego);
            }
        }else if(objeto_vector == OBSTACULO){
            tipo_objeto = juego->niveles[posicion_niveles].obstaculos[posicion_objeto].tipo;
            if(tipo_objeto == FUEGO){
                misma_posicion_fuego(juego);
            }else if(tipo_objeto == MEDIA){
                misma_posicion_media(juego);
            }
        }
        else if(objeto_vector == PAPELEO){
            misma_posicion_papeleo(juego, posicion_objeto, posicion_niveles);
        }
    }
}

// Procedimiento que segun el movimiento y la dimension recibida, rota horariamente o antihorariamente 
// las coordenadas del personaje, de las paredes, de los objetos y de los papeleos.
void rotar_matriz(juego_t* juego, nivel_t* nivel, char movimiento, int dimension_terreno){
    dimension_terreno--;
    coordenada_t posicion_temporal;
    printf("%i", juego->jugador.posicion.col);
    printf("   %i\n", juego->jugador.posicion.fil);
    if(movimiento == ROTAR_HORARIO){   
        int x = juego->jugador.posicion.fil;
        int y = juego->jugador.posicion.col;
        posicion_temporal.fil = y;
        posicion_temporal.col = abs(x-dimension_terreno);
        juego->jugador.posicion = posicion_temporal;        
        for(int i = 0; i < nivel->tope_paredes; i++){
            int x = nivel->paredes[i].fil;
            int y = nivel->paredes[i].col;
            posicion_temporal.fil = y;
            posicion_temporal.col = abs(x-dimension_terreno);
            nivel->paredes[i] = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_herramientas; i++){
            int x = nivel->herramientas[i].posicion.fil;
            int y = nivel->herramientas[i].posicion.col;
            posicion_temporal.fil = y;
            posicion_temporal.col = abs(x-dimension_terreno);
            nivel->herramientas[i].posicion = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_obstaculos; i++){
            int x = nivel->obstaculos[i].posicion.fil;
            int y = nivel->obstaculos[i].posicion.col;
            posicion_temporal.fil = y;
            posicion_temporal.col = abs(x-dimension_terreno);
            nivel->obstaculos[i].posicion = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_papeleos; i++){
            int x = nivel->papeleos[i].posicion.fil;
            int y = nivel->papeleos[i].posicion.col;
            posicion_temporal.fil = y;
            posicion_temporal.col = abs(x-dimension_terreno);
            nivel->papeleos[i].posicion = posicion_temporal;
        }        
    }else if(movimiento == ROTAR_ANTIHORARIO){
        int x = juego->jugador.posicion.fil;
        int y = juego->jugador.posicion.col;
        posicion_temporal.fil = abs(y-dimension_terreno);
        posicion_temporal.col = x;
        juego->jugador.posicion = posicion_temporal;
      for (int i = 0; i < nivel->tope_paredes; i++){
            int x = nivel->paredes[i].fil;
            int y = nivel->paredes[i].col;
            posicion_temporal.fil = abs(y-dimension_terreno);
            posicion_temporal.col = x;
            nivel->paredes[i] = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_herramientas; i++){
            int x = nivel->herramientas[i].posicion.fil;
            int y = nivel->herramientas[i].posicion.col;
            posicion_temporal.fil = abs(y-dimension_terreno);
            posicion_temporal.col = x;
            nivel->herramientas[i].posicion = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_obstaculos; i++){
            int x = nivel->obstaculos[i].posicion.fil;
            int y = nivel->obstaculos[i].posicion.col;
            posicion_temporal.fil = abs(y-dimension_terreno);
            posicion_temporal.col = x;
            nivel->obstaculos[i].posicion = posicion_temporal;
        }
        for (int i = 0; i < nivel->tope_papeleos; i++){
            int x = nivel->papeleos[i].posicion.fil;
            int y = nivel->papeleos[i].posicion.col;
            posicion_temporal.fil = abs(y-dimension_terreno);
            posicion_temporal.col = x;
            nivel->papeleos[i].posicion = posicion_temporal;
        }
    }
    juego->jugador.movimientos--;
    juego->jugador.movimientos_realizados++;
    imprimir_terreno(*juego);
}

// Procedimiento que simula la gravedad del personaje
// Verifica cada interaccion en cada posicion mientras que se suma de uno las filas.
// Termina de caer cuando se encuentra con una pared debajo.
void gravedad_mike(juego_t* juego, int dimension_terreno, int posicion_niveles){
    coordenada_t posicion_temporal = juego->jugador.posicion;
    posicion_temporal.fil++;
    while(!(es_coordenada_igual_paredes(posicion_temporal, juego->niveles[posicion_niveles].paredes, juego->niveles[posicion_niveles].tope_paredes))
            && (juego->jugador.movimientos > 0)){
        juego->jugador.posicion = posicion_temporal;
        imprimir_terreno(*juego);
        interacciones_matriz_mike(juego, &posicion_temporal, dimension_terreno, posicion_niveles);
        posicion_temporal.fil++;
        // detener_el_tiempo(0.5);
    }
}

// Procedimiento que segun el movimiento recibido, mueva el jugador, y realize las acciones
// debidas dependiendo en la posicion al que se movio. Si esta posicion es una pared, el personaje no se mueve.
void mover_jugador(juego_t* juego, char movimiento, int dimension_terreno, int posicion_niveles){
    coordenada_t coordenada_temporal_mike = juego->jugador.posicion;

    if(movimiento == IZQUIERDA){
        if (coordenada_temporal_mike.col > 0 ){
		    coordenada_temporal_mike.col--;
		}
    }else if(movimiento == DERECHA){
        if (coordenada_temporal_mike.col < dimension_terreno-1) {
			coordenada_temporal_mike.col++;
		}
    }

    interacciones_matriz_mike(juego, &coordenada_temporal_mike, dimension_terreno, posicion_niveles);
    juego->jugador.posicion = coordenada_temporal_mike;
    juego->jugador.movimientos--;
    juego->jugador.movimientos_realizados++;
    imprimir_terreno(*juego);
    // detener_el_tiempo(0.5);
} 

// Recibo el vector de papeleos y su tope positivo
// Devuelve un papeleo aleatorio no recolectado entre todos los papeleos del vector.
int papeleo_aleatorio_no_recolectado(papeleo_t papeleos[MAX_PAPELEOS], int tope_papeleos){
    int papeleo_aleatorio = rand() % tope_papeleos;    
    while(papeleos[papeleo_aleatorio].recolectado == PAPELEO_RECOLECTADO_SI){
        papeleo_aleatorio = rand() % tope_papeleos;
    }
    return papeleo_aleatorio;
}

// Funcion que devuelve true si el movimiento fue logrado o no.
// Devuelve TRUE si los movimientos realizados es mayor a los movimientos del comienzo de la accion.
bool movimiento_logrado(juego_t* juego, int movimientos_comienzo){
    return(movimientos_comienzo < juego->jugador.movimientos_realizados);
}

// Procedimiento que se ocupa del Ahuyenta Randall, si el interruptor se encuentra apagado y los movimientos realizados 
// son divisibles por su R correspondiente por nivel,  un papeleo aleatorio cambiara su posicion.
void ahuyenta_randall(juego_t* juego, nivel_t* nivel, int dimension_terreno, int nivel_actual, int posicion_niveles){
    int movimientos = juego->jugador.movimientos_realizados;
    int R_NIVEL;
    if( nivel_actual == NIVEL_1 ){
        R_NIVEL = R_NIVEL_1;
    }else if( nivel_actual == NIVEL_2){
        R_NIVEL = R_NIVEL_2;
    }else if ( nivel_actual == NIVEL_3){
        R_NIVEL = R_NIVEL_3;
    }

    if((juego->jugador.ahuyenta_randall == INTERRUPTOR_APRETADO_NO ) 
        && (movimientos % R_NIVEL == 0)){
        int papeleto_aleatorio = papeleo_aleatorio_no_recolectado(nivel->papeleos, juego->niveles[posicion_niveles].tope_papeleos );;
        coordenada_t coordenada_temporal;
        bool coordenada_incorporada = false;
        while(coordenada_incorporada == false){
            coordenada_temporal = coordenada_aleatoria(dimension_terreno);
            if(!(existe_posicion_objetos(coordenada_temporal, *nivel)) && !(es_posicion_mike(*nivel, coordenada_temporal))){
                nivel->papeleos[papeleto_aleatorio].posicion = coordenada_temporal;
                coordenada_incorporada = true;
            }
        }
    }
}

// Procedimiento que genera una pared random en la matriz por cada movimiento logrado.
// La cantidad de paredes a agregar es diferente por cada nivel.
void nuevas_paredes_random(juego_t* juego, nivel_t* nivel, int dimension_terreno, int nivel_actual){
    int movimientos = juego->jugador.movimientos_realizados;
    int paredes_random;
    if( nivel_actual == NIVEL_1 ){
        paredes_random = PAREDES_RANDOM_1;
    }else if( nivel_actual == NIVEL_2){
        paredes_random = PAREDES_RANDOM_2;
    }else if ( nivel_actual == NIVEL_3){
        paredes_random = PAREDES_RANDOM_3;
    }

    if((movimientos < paredes_random)){
        int tope_temporal= nivel->tope_paredes;
        nivel->tope_paredes++;
        coordenada_t coordenada_temporal;
        bool coordenada_incorporada = false;
        while(coordenada_incorporada == false){
            coordenada_temporal = coordenada_aleatoria(dimension_terreno);
            if(!(existe_posicion_objetos(coordenada_temporal, *nivel)) && !(es_posicion_mike(*nivel, coordenada_temporal))){
                nivel->paredes[tope_temporal] = coordenada_temporal;
                coordenada_incorporada = true;
            }
        }
    }
}

// Procedimiento que se ocupa del cambio de nivel
// Añade 1 al nivel actual e inicializa al jugador en el nivel correspondiente.
void siguiente_nivel(juego_t* juego, int posicion_niveles){
    juego->nivel_actual++;
    inicializar_jugador(juego, &(juego->jugador), juego->personaje_tp1);
    imprimir_terreno(*juego);
}

/*
 * Moverá el personaje, y realizará la acción necesaria en caso de chocar con un elemento
 */
void realizar_jugada(juego_t* juego){
    char movimiento = VACIO;
    int nivel_actual = (juego->nivel_actual);
    int posicion_niveles = (juego->nivel_actual-1);
    int movimientos_realizados = juego->jugador.movimientos_realizados;
    int dimension_terreno;
    if(nivel_actual == NIVEL_1 ){
        dimension_terreno = DIMENSION_LVL1;
    }else if(nivel_actual == NIVEL_2){
        dimension_terreno = DIMENSION_LVL2;
    }else if (nivel_actual == NIVEL_3){
        dimension_terreno = DIMENSION_LVL3;
    }
    pedir_movimiento(&movimiento);  
    if((movimiento == USAR_EXTINTOR) || (movimiento == USAR_MARTILLO)){
        realizar_accion(juego, movimiento, dimension_terreno, posicion_niveles);
        gravedad_mike(juego, dimension_terreno, posicion_niveles);
    }else
     if((movimiento == ROTAR_HORARIO) || (movimiento == ROTAR_ANTIHORARIO)){
        rotar_matriz(juego, &(juego->niveles[posicion_niveles]),  movimiento, dimension_terreno);
        gravedad_mike(juego, dimension_terreno, posicion_niveles);
    } else{
        mover_jugador(juego, movimiento, dimension_terreno, posicion_niveles);
        gravedad_mike(juego, dimension_terreno, posicion_niveles);
    }
    if(movimiento_logrado(juego, movimientos_realizados)){
        ahuyenta_randall(juego, &(juego->niveles[posicion_niveles]), dimension_terreno, nivel_actual, posicion_niveles);
        nuevas_paredes_random(juego, &(juego->niveles[posicion_niveles]), dimension_terreno, nivel_actual);
    }

     if(estado_nivel(juego->niveles[posicion_niveles].papeleos, juego->niveles[posicion_niveles].tope_papeleos)  == GANADO && (juego->nivel_actual != 3)){
        siguiente_nivel(juego, posicion_niveles);
    }
    // estado_juego(*juego);
}

// Procedimiento que carga las posiciones del terreno con su elemento indicando, recorriendo los vectores
// de objetos, paredes y papeleos, en el nivel conveniente.
void llenar_terreno(juego_t juego, char terreno[DIMENSION_MAXIMA][DIMENSION_MAXIMA],int dimension_terreno, int posicion_niveles){
    coordenada_t posicion_temporal;
    int posicion_vector_obstaculos;
    int posicion_vector_herramientas;
    int posicion_vector_papeleos;

    for(int i = 0; i < dimension_terreno; i++){
        for(int j = 0; j < dimension_terreno; j++){

            posicion_temporal.fil = i;
            posicion_temporal.col = j;
            posicion_vector_obstaculos = buscar_por_coordenada_objeto(juego.niveles[posicion_niveles].obstaculos, juego.niveles[posicion_niveles].tope_obstaculos, posicion_temporal);
            posicion_vector_herramientas = buscar_por_coordenada_objeto(juego.niveles[posicion_niveles].herramientas, juego.niveles[posicion_niveles].tope_herramientas, posicion_temporal);
            posicion_vector_papeleos = buscar_por_coordenada_papeleo(juego.niveles[posicion_niveles].papeleos, juego.niveles[posicion_niveles].tope_papeleos, posicion_temporal);

            if(posicion_vector_obstaculos != NO_ENCONTRADO){
                terreno[posicion_temporal.fil][posicion_temporal.col] = juego.niveles[posicion_niveles].obstaculos[posicion_vector_obstaculos].tipo;
            }
            else if(posicion_vector_herramientas != NO_ENCONTRADO){
                terreno[posicion_temporal.fil][posicion_temporal.col] = juego.niveles[posicion_niveles].herramientas[posicion_vector_herramientas].tipo;
            }
            else if(posicion_vector_papeleos != NO_ENCONTRADO){
                terreno[posicion_temporal.fil][posicion_temporal.col] = numero_papeleo(juego, posicion_niveles, posicion_vector_papeleos);
            }
             else if (es_coordenada_igual_paredes(posicion_temporal, juego.niveles[posicion_niveles].paredes, juego.niveles[posicion_niveles].tope_paredes)){
				terreno[i][j] = PARED;
            }else{
                terreno[i][j] = VACIO;
            }
        }
    }
    terreno[juego.jugador.posicion.fil][juego.jugador.posicion.col] = MIKE;
}

/* 
 * Procedimiento que recibe el juego e imprime toda su información por pantalla.
 */
void imprimir_terreno(juego_t juego){
    system( "clear");
    int posicion_niveles = (juego.nivel_actual-1);
    int nivel_actual = (juego.nivel_actual);
    char terreno[DIMENSION_MAXIMA][DIMENSION_MAXIMA];

    int dimension_terreno;
    if( nivel_actual == NIVEL_1 ){
        dimension_terreno = DIMENSION_LVL1;
    }else if( nivel_actual == NIVEL_2){
        dimension_terreno = DIMENSION_LVL2;
    }else if ( nivel_actual == NIVEL_3){
        dimension_terreno = DIMENSION_LVL3;
    }

    llenar_terreno(juego, terreno, dimension_terreno, posicion_niveles);

    printf("\n   MIKE EN EL MULTIVERSO DE LOS PAPELEOS\n   NIVEL %i\n", nivel_actual);
	for (int i = 0; i < dimension_terreno; i++) {
		for (int j = 0; j < dimension_terreno; j++) {
			if(terreno[i][j] == MIKE){
                printf("%s", EMOJI_MIKE);
            }else if(terreno[i][j] == PARED){
                printf("%s", EMOJI_PARED);
            }else if(terreno[i][j] == FUEGO){
                printf("%s", EMOJI_FUEGO);
            }else if(terreno[i][j] == BOTELLA_GRITO){
                printf("%s", EMOJI_BOTELLA);
            }else if(terreno[i][j] == INTERRUPTOR){
                printf("%s ", EMOJI_INTERRUPTOR);
            }else if(terreno[i][j] == MEDIA){
                printf("%s", EMOJI_MEDIAS);
            }else {
                printf("%c", terreno[i][j]);
            }
        }
        printf("\n");
	}
    printf("   Movimientos: %i || Movimientos Realizados: %i \n %sExtintores:  %i  || %sMartillos: %i\n", 
        juego.jugador.movimientos, juego.jugador.movimientos_realizados, EMOJI_EXTINTOR, juego.jugador.extintores, 
        EMOJI_MARTILLO, juego.jugador.martillos);
    char interruptor[MAX_INTERRUPTOR];
    if(juego.jugador.ahuyenta_randall == INTERRUPTOR_APRETADO_SI){
        strcpy(interruptor,"SI");
    }else{
        strcpy(interruptor,"NO");
    }
    printf(" %s Ahuyenta Randall Activado: %s \n",EMOJI_INTERRUPTOR, interruptor);
    printf("\n");
}

/*
 * El nivel se dará por terminado, si se reolectan todos los papeleos en el mismo.
 * Devolverá:
 * -> 0 si el estado es jugando.
 * -> 1 si el estado es ganado.
 */
int estado_nivel(papeleo_t papeleos[MAX_PAPELEOS],int tope_papeleos){ 
    bool todos_recolectados = true;
    for(int i = 0; i<tope_papeleos;i++){
        if(papeleos[i].recolectado == PAPELEO_RECOLECTADO_NO){
            todos_recolectados = false;
        }
    }
    if(todos_recolectados == true){
        return GANADO;
    }else{
        return JUGANDO; 
    }
}

/*
 * Recibe un juego con todas sus estructuras válidas.
 *
 * El juego se dará por ganado, si terminó todos los niveles. O perdido, si el personaje queda
 * sin movimientos. 
 * Devolverá:
 * -> 0 si el estado es jugando. 
 * -> -1 si el estado es perdido.
 * -> 1 si el estado es ganado.
 */
int estado_juego(juego_t juego){
    int posicion_niveles = (juego.nivel_actual-1);
    int estado;
    if(juego.jugador.movimientos <= 0){
        estado = PERDIDO;
             printf("\
                __________ _____________________ ________   .___   _______________________________      \n \
                \\______   \\_   _____/\\______   \\______  \\  |   | /   _____/\\__    ___/\\_   _____/ \n \
                |     ___/ |    __)_  |       _/ |    |  \\ |   | \\_____  \\   |    |    |    __)_     \n \
                |    |     |        \\ |    |   \\ |    `   \\|   | /        \\  |    |    |       \\   \n \
                |____|    /_______  / |____|_  //_______  /|___|/_______  /  |____|   /_______  /       \n \
                                  \\/         \\/         \\/              \\/                    \\/     \n");
    }else if((juego.nivel_actual == NIVEL_3) && (estado_nivel( juego.niveles[posicion_niveles].papeleos , juego.niveles[posicion_niveles].tope_papeleos) == GANADO)){
            printf("\
                    ________    _____    _______      _____     _______________________________          \n \
                   /  _____/   /  _  \\   \\      \\    /  _  \\   /   _____/\\__    ___/\\_   _____/    \n \
                  /   \\  ___  /  /_\\  \\  /   |   \\  /  /_\\  \\  \\_____  \\   |    |    |    __)_   \n \
                  \\    \\_\\  \\/    |    \\/    |    \\/    |    \\ /        \\  |    |    |        \\ \n \
                   \\______  /\\____|__  /\\____|__  /\\____|__  //_______  /  |____|   /_______  /      \n \
                          \\/         \\/         \\/         \\/         \\/                    \\/     \n");
        estado = GANADO;
    }else{
        estado = JUGANDO;
    }
    return estado;
}
