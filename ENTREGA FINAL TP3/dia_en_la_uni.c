#include <stdio.h>
#include <stdbool.h>

const char OLAF = 'O';
const char STITCH = 'S';
const char JASMIN = 'J';
const char RAYO_MCQUEEN = 'R';

const char TRINEO = 'T';
const char NAVE_ESPACIAL = 'N';
const char CAMION = 'C';
const char ALFOMBRA = 'A';

const char BOLAS_NIEVE = 'B';
const char PIUM_PIUM = 'P';
const char KUCHAU = 'K';
const char GENIO = 'G';

const int PUNTAJE_TRINEO = 10;
const int PUNTAJE_NAVE_ESPACIAL = 10;
const int PUNTAJE_CAMION = 20;
const int PUNTAJE_ALFOMBRA = 20;

const int PUNTAJE_BOLAS_NIEVE = 3;
const int PUNTAJE_PIUM_PIUM = 6;
const int PUNTAJE_KUCHAU = 9;
const int PUNTAJE_GENIO = 12;

const int PUNTAJE_MINIMO = 13;
const int PUNTAJE_MEDIO = 41;
const int PUNTAJE_MAXIMO = 68;

// Pre : -
// Post:  Devuelve true si el char 'transporte' recibido es un char valido.
bool es_transporte_valido(char transporte){
    return ( (transporte == TRINEO) || (transporte == NAVE_ESPACIAL) || (transporte == CAMION) || (transporte == ALFOMBRA));
}

// Pre : -
// Post:Devuelve true si el char 'medio' recibido es un char valido.
bool es_medio_valido(char medio){
    return ( (medio == BOLAS_NIEVE) || (medio == PIUM_PIUM) || (medio == KUCHAU) || (medio == GENIO));
}

// Pre : horario tiene que ser positivo
// Post: Devuelve los decimales del float horario recibido.
float minutos(float horario){
    float horario_minutos = horario - (float)(int)horario;
    return horario_minutos;
}

// Pre : horario tiene que ser positivo
// Post: Devuelve TRUE si las horas y los minutos se encuentran en el rango valido.
bool es_horario_valido(float horario){
    bool es_horario_valido = false;    
    float horario_minutos = minutos(horario);
    int horario_hora = (int)horario;
    if((horario_hora >= 0) && (horario_hora <= 23) && (horario_minutos >= 0) && (horario_minutos < 0.60)){
        es_horario_valido = true;
    }
    return es_horario_valido;
}

// Pre : -
// Post: Devuelve TRUE si la nota es valida.
bool es_nota_valido(int nota){
    return ((nota > 0) && (nota <= 10));
}


// Pre : Recibo por referencia el trasnporte
// Post: Pide al usuario que transporte utilizar imprimiendo por pantalla las opciones posibles. 
//       Vuelve a pedir si este transporte ingresado no es valido.
void pregunta_transporte(char* ref_transporte){
    printf("-----------------------------------------------\n\n");
    printf("Hoy rendis un final en Paseo Colon\n");
    printf("Pero hay paro de trenes, que transporte elegis?\n");
    printf("[%c]Trineo [%c]Nave Espacial [%c]Camion [%c]Alfombra\n", TRINEO, NAVE_ESPACIAL, CAMION, ALFOMBRA);
    scanf(" %c", ref_transporte);

    while(!es_transporte_valido(*ref_transporte)) {
        printf("Solo tenes los siguientes medios de transporte:\n");
        printf("[%c]Trineo [%c]Nave Espacial [%c]Camion [%c]Alfombra\n", TRINEO, NAVE_ESPACIAL, CAMION, ALFOMBRA);
        scanf(" %c", ref_transporte);
    }
}


// Pre : Recibe por referencia el medio
// Post: Pide al usuario que medio utilizar imprimiendo por pantalla las opciones posibles.
//      Vuelve a pedir si este medio ingresado no es valido.
void pregunta_medio(char* ref_medio, char transporte){
    printf("-----------------------------------------------\n\n");
    if(transporte == TRINEO){
        printf("Estas yendo en Trineo a la facultad\n\n");
    }else if(transporte == NAVE_ESPACIAL){
        printf("Estas yendo en Nave Espacial a la facultad\n\n");
    }else if(transporte == CAMION){
        printf("Estas yendo en Camion a la facultad\n\n");
    }else if(transporte == ALFOMBRA){
        printf("Estas yendo en Alfombra a la facultad\n");
    }

    printf("Uh, te cruzaste con una manifestacion en el ministerio de defensa,\n y la avenida Paseo Colon esta cortada\n");
    printf("Con que te deshaces de la manifestacion?\n");
    printf("Con: [%c]Bolas de Nieve [%c]Pium Pium [%c]Kuchau [%c]Genio\n", BOLAS_NIEVE, PIUM_PIUM, KUCHAU, GENIO);
    scanf(" %c", ref_medio);

    while(!es_medio_valido(*ref_medio)) {
        printf("Solo tenes los siguientes medios para deshacerte de la manifestacion\n");
        printf("[%c]Bolas de Nieve [%c]Pium Pium [%c]Kuchau [%c]Genio ?\n", BOLAS_NIEVE, PIUM_PIUM, KUCHAU, GENIO);
        scanf(" %c", ref_medio);
    }
}

// Pre : Recibe por referencia el horario
// Post: Pide al usuario que ingrese el horario. 
//      Vuelve a pedir si este medio ingresado no es valido.
void pregunta_horario(float* ref_horario, char medio){
    printf("-----------------------------------------------\n\n");
    if(medio == BOLAS_NIEVE){
        printf("Bien, pudiste deshacerte de la manifestacion con las Bolas de Nieve\n\n");
    }else if(medio == PIUM_PIUM){
        printf("Bien, pudiste deshacerte de la manifestacion con la pistola *Pium Pium*\n\n");
    }else if(medio == KUCHAU){
        printf("Bien, pudiste deshacerte de la manifestacion con el Kuchau\n\n");
    }else if(medio == GENIO){
        printf("Bien, pudiste deshacerte de la manifestacion con el Genio\n\n");
    }

    printf("Al fin llegaste a la facultad\n");
    printf("A que hora llegaste?\n");
    printf("Decime el horario en formato 24h, y separando las horas y los minutos con un punto.\n");
    scanf(" %f", ref_horario);

    while(!es_horario_valido(*ref_horario)){
        printf("El formato del horario que me pasaste es erroneo\n");
        printf("Decime el horario en formato 24h, y separando las horas y los minutos con un punto.\n");
        scanf(" %f", ref_horario);
        
    }
}

// Pre : Recibe por referencia la nota
// Post: Pide al usuario que ingrese la nota que obtuvo. 
//       Vuelve a pedir si la nota ingresado no es valida.
void pregunta_nota(int* ref_nota){
    printf("-----------------------------------------------\n\n");
    printf("Ya subieron las notas del final al campus! \n");
    printf("Que nota te sacaste?\n");
    printf("Decime tu nota del 1 al 10\n");
    scanf(" %i", ref_nota);

    while(!es_nota_valido(*ref_nota)) {
        printf("Decime tu nota del 1 al 10\n"); 
        scanf(" %i", ref_nota);
    }
}

// Pre : -
// Post: A partir del transporte recibido, devuelve la cantidad de puntos correspondientes a ese transporte.
int puntos_transporte(char transporte){
    int puntos_transporte;
    if(transporte == TRINEO){
        puntos_transporte = PUNTAJE_TRINEO;
    }else if(transporte == NAVE_ESPACIAL){
        puntos_transporte = PUNTAJE_NAVE_ESPACIAL;
    }else if(transporte == CAMION){
        puntos_transporte = PUNTAJE_CAMION;
    }else if(transporte == ALFOMBRA){
        puntos_transporte = PUNTAJE_ALFOMBRA;
    }
    return puntos_transporte;
}

// Pre : -
// Post: A partir del medio recibido, devuelve la cantidad de puntos correspondientes a ese medio
int puntos_medio(char medio)  {    
    int puntos_medio;
    if(medio == BOLAS_NIEVE){
        puntos_medio = PUNTAJE_BOLAS_NIEVE;
    }else if(medio == PIUM_PIUM){
        puntos_medio = PUNTAJE_PIUM_PIUM;
    }else if(medio == KUCHAU){
        puntos_medio = PUNTAJE_KUCHAU;
    }else if(medio == GENIO){
        puntos_medio = PUNTAJE_GENIO;
    }
    return puntos_medio;
}

// Pre : -
// Post: A partir de la nota recibida, devuelve el multiplicador correspondiente a esa nota.
int multiplicador_nota(int nota)  {
    int multiplicador_nota;
    if(nota <= 3){
        multiplicador_nota = 1;
    }else if(nota <= 6){
        multiplicador_nota = 2;
    }else if(nota <= 8){
        multiplicador_nota = 3;
    }else if(nota <= 10){
        multiplicador_nota = 4;
    }
    return multiplicador_nota;
}

// Pre : Recibo por referencia el puntaje total
// Post: Calcula este puntaje total usando los char recibidos anteriormente
void calculo_puntaje(int* ref_puntaje_total, char transporte, char medio, float horario, int nota){
    int puntaje_transporte = puntos_transporte(transporte);
    int puntaje_medio = puntos_medio(medio);
    int puntaje_nota = multiplicador_nota(nota);
    *ref_puntaje_total = puntaje_transporte + (puntaje_medio * puntaje_nota);
}

// Pre : Recibo el personaje final por referencia
// Post: Mediante el puntaje total y el horario, se determina cual es el personaje final del ususario.
void determinacion_personaje(char* ref_personaje_final, int puntaje_total, float horario){
    printf("-----------------------------------------------\n\n");
    printf("Procesando informacion...\n");
    printf("Analizando personalidad...\n\n");

    if(puntaje_total >= PUNTAJE_MINIMO && puntaje_total <= PUNTAJE_MEDIO){
        if(horario < 15.31){
            *ref_personaje_final = STITCH;
            printf("Con un puntaje de %i puntos, tu personaje es el Experimento 626, alias -Stitch-!\n\n ",puntaje_total);
        }else{
            *ref_personaje_final = OLAF;
            printf("Con un puntaje de %i puntos, tu personaje es el muñeco de nieve -Olaf-!\n\n",puntaje_total);
        }
    }else if(puntaje_total >= PUNTAJE_MINIMO && puntaje_total <= PUNTAJE_MAXIMO ){
        if(horario < 15.31){
            *ref_personaje_final = JASMIN;
            printf("Con un puntaje de %i puntos, tu personaje es la princesa -Jasmín-!\n\n",puntaje_total);
        }else{
            *ref_personaje_final = RAYO_MCQUEEN;
            printf("Con un puntaje de %i puntos, tu personaje es el ganador de 5 Copas Pistón, -Rayo McQueen-!\n\n",puntaje_total);
        }
    }
}

void preguntar_personaje(char* personaje_tp1){
    	
    char transporte;
    char medio;
    int nota;
    int puntaje_total;
    float horario;

    pregunta_transporte(&transporte);
    pregunta_medio(&medio, transporte);
    pregunta_horario(&horario, medio);
    pregunta_nota(&nota);
    calculo_puntaje(&puntaje_total, transporte, medio, horario, nota);
    determinacion_personaje(personaje_tp1, puntaje_total, horario);
}