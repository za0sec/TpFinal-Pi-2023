#include "bikeSharingADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct stationData{
    char * stationName;
    size_t memberTrips;
    size_t stationId; // Id de estacion de salida
    size_t used;
    //int vecMonths[TOTAL_MONTHS]; //todavia no lo usamos, cuando lleguemos al query 3
}stationData;

typedef struct bikeSharingCDT{
    stationData * rankingStations;
    size_t dim; //Dimension de todas las stations sin usar y usadas.
    size_t realDim; //Dimension solo de las stations usadas.
    size_t ** mat; // Matriz de adyacencia
    size_t dimMat; // Filas y columnas de la matriz ( es cuadrada ) 
}bikeSharingCDT;


//Funcion compare para tener un criterio de comparacion.
int compare(const void *a, const void *b){
    stationData *station1 = (stationData *)a;
    stationData *station2 = (stationData *)b;

    int cmp = 0;

    //Me fijo si el primero tiene menor cantidad de viajes o si tienen igual cantidad y son distintos lexicograficamente.
    if (station1->memberTrips < station2->memberTrips){
        cmp = 1;
    } else if (station1->memberTrips > station2->memberTrips){
        cmp = -1;
    }

    if (!cmp)
        cmp = strcasecmp(station1->stationName, station2->stationName);
    
    return cmp;
}

bikeSharingADT newBikeSharing(void){
    return calloc(1, sizeof(bikeSharingCDT));
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember){
    if (bikesh->dim < station1Id){
        bikesh->rankingStations = realloc(bikesh->rankingStations, station1Id * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
    
        for(int i=bikesh->dim; i<station1Id; i++){
            bikesh->rankingStations[i].memberTrips = 0;
            bikesh->rankingStations[i].stationName = NULL;
            bikesh->rankingStations[i].stationId = 0;
            bikesh->rankingStations[i].used = 0;
        }
        bikesh->dim = station1Id;
    }
    if (!bikesh->rankingStations[station1Id-1].used){
        bikesh->realDim++;
        bikesh->rankingStations[station1Id-1].used = 1;
        bikesh->rankingStations[station1Id-1].memberTrips = 0;
    }
    if(isMember)
        bikesh->rankingStations[station1Id-1].memberTrips++;
    
} //al final de esta funcion, deberiamos tener todos los stationData ordenador por stationId en un vector

static char * copyStr(const char * s){
    char * copy = malloc(strlen(s) + 1);
    if(copy == NULL) {
        return NULL; // or handle the error appropriately
    }
    return strcpy(copy, s);
}

//Funcion que ordena el vector previamente asignado con valores. Ordena en base a los viajes realizados de mayor a menor (de miembros)
void tripSort(bikeSharingADT bikesh){

    int k=0;
    for (size_t i=0; i<bikesh->dim; i++){

        if (bikesh->rankingStations[i].used){
            free(bikesh->rankingStations[k].stationName);  // liberar la cadena original
            bikesh->rankingStations[k].stationName = copyStr(bikesh->rankingStations[i].stationName);
            bikesh->rankingStations[k].memberTrips = bikesh->rankingStations[i].memberTrips;
            bikesh->rankingStations[k++].stationId = i+1;
        }    
    }
    
    bikesh->rankingStations = realloc(bikesh->rankingStations, k * sizeof(stationData)); //Con este realloc eliminamos del vector todas las estaciones que tengan el used en 0.
    
    qsort(bikesh->rankingStations, bikesh->realDim, sizeof(stationData), compare);

}

size_t getDim(bikeSharingADT bikesh){
    return bikesh->dim;
}

size_t getRealDim(bikeSharingADT bikesh){
    return bikesh->realDim;
}

size_t getMemberTrips(bikeSharingADT bikesh, int pos){
    return bikesh->rankingStations[pos].memberTrips;
}

char * getStationName(bikeSharingADT bikesh, int pos){
    return copyStr(bikesh->rankingStations[pos].stationName);
}

void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag){
 
    bikesh->rankingStations[stationId-1].stationName = realloc(bikesh->rankingStations[stationId-1].stationName, (strlen(from)+1) * sizeof(char));
    if (bikesh->rankingStations[stationId-1].stationName == NULL){
        (*flag) = MEMERR;
        return;
    }
    strcpy(bikesh->rankingStations[stationId-1].stationName, from); 

}

void addMatrix(bikeSharingADT bikesh, size_t station1Id, size_t station2Id, size_t * flagError){ // Crea la matriz de adyacencia

    size_t size = MAX_SIZE(station1Id, station2Id);

    if (bikesh->dimMat < size){
        // Agrego memoria para las filas
        bikesh->mat = realloc(bikesh->mat, size * sizeof(size_t*));
        if (bikesh->mat == NULL) {
            (*flagError) = MEMERR;
            return;
        }

        // Para cada fila agrego memoria a las columnas
        for (size_t i = 0; i < size; i++) {
            if (i >= bikesh->dimMat) {
                bikesh->mat[i] = NULL;  // Asegúrate de que bikesh->mat[i] esté inicializado
            }
            bikesh->mat[i] = realloc(bikesh->mat[i], size * sizeof(size_t));
            if (bikesh->mat[i] == NULL) {
                (*flagError) = MEMERR;
                return;
            }
        }

        // Inicializamos una parte de la matriz con 0
        for (size_t i = 0; i < bikesh->dimMat; i++) {
            for (size_t j = bikesh->dimMat; j < size; j++) {
                bikesh->mat[i][j] = 0;
            }
        }

        for (size_t i = bikesh->dimMat; i < size; i++){
            for (size_t j = 0; j < size; j++){
                bikesh->mat[i][j] = 0;
            }
        }
        bikesh->dimMat = size; // Actualizamos el tamano de la matriz
    }

    // Llenamos la matriz con los datos
    bikesh->mat[station1Id-1][station2Id-1]++;

}

void printmat(bikeSharingADT bikesh){

    for( int i = 0; i < bikesh->dimMat; i++ ){
        for( int j = 0; j < bikesh->dimMat; j++ ){
            printf("%ld  ", bikesh->mat[i][j] );
        }
        puts("");
    }

}


void freeADT(bikeSharingADT bikesh){

    if (bikesh != NULL) {
        if (bikesh->rankingStations != NULL) {
            for (int i = 0; i < bikesh->dimMat; i++){
                free(bikesh->mat[i]);
            }
            free(bikesh->mat);
            for (int i = 0; i < bikesh->realDim; i++) {
                if (bikesh->rankingStations[i].stationName != NULL) {
                    free(bikesh->rankingStations[i].stationName);
                }
            }
            free(bikesh->rankingStations);
        }
        free(bikesh);
    }


}

