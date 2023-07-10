#include "bikeSharingADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct stationData{
    char * stationName; //TENEMOS QUE LEER EL OTRO ARCHIVO PARA CONSEGUIR EL NOMBRE DE LA ESTACION
    size_t memberTrips;
    size_t stationId;
    size_t used;
    int vecMonths[TOTAL_MONTHS]; //todavia no lo usamos, cuando lleguemos al query 3
}stationData;

typedef struct bikeSharingCDT{
    stationData * rankingStations;
    size_t dim; //Dimension de todas las stations sin usar y usadas.
    size_t realDim; //Dimension solo de las stations usadas.
}bikeSharingCDT;


//Funcion compare para tener un criterio de comparacion.
int compare(const void *a, const void *b){
    stationData *station1 = (stationData *)a;
    stationData *station2 = (stationData *)b;
    
    //Me fijo si el primero tiene menor cantidad de viajes o si tienen igual cantidad y son distintos lexicograficamente.
    if (station1->memberTrips < station2->memberTrips || (station1->memberTrips == station2->memberTrips && strcmp(station1->stationName, station2->stationName) > 0)){
        return 1;
    } else if (station1->memberTrips > station2->memberTrips || (station1->memberTrips == station2->memberTrips && strcmp(station1->stationName, station2->stationName) < 0)){
        return -1;
    }

    return 0;
}



bikeSharingADT newBikeSharing(){
    return calloc(1, sizeof(bikeSharingCDT));
}

void addStation(bikeSharingADT bikesh, size_t station1Id, size_t isMember){
    if(isMember){
        if (bikesh->dim < station1Id){
            bikesh->rankingStations = realloc(bikesh->rankingStations, station1Id * sizeof(stationData)); // Agrego memoria si es que el station dado es menor a dim
        
            for(int i=bikesh->dim; i<station1Id; i++){
                bikesh->rankingStations[i].used = 0;
            }
            bikesh->dim = station1Id;
        }
        if (!bikesh->rankingStations[station1Id-1].used){
            bikesh->realDim++;
            bikesh->rankingStations[station1Id-1].used = 1;
            bikesh->rankingStations[station1Id-1].memberTrips = 0;
        }


        bikesh->rankingStations[station1Id-1].memberTrips++;

    }


} //al final de esta funcion, deberiamos tener todos los stationData ordenador por stationId en un vector



static char * copyStr(const char * s){
    return strcpy(malloc(strlen(s)+1), s);
}

//Funcion que ordena el vector previamente asignado con valores. Ordena en base a los viajes realizados de mayor a menor (de miembros)
void tripSort(bikeSharingADT bikesh){

    int k=0;
    for (int i=0; i<bikesh->dim; i++){
        if (bikesh->rankingStations[i].used){

            bikesh->rankingStations[k].stationName = copyStr(bikesh->rankingStations[i].stationName);
            bikesh->rankingStations[k].memberTrips = bikesh->rankingStations[i].memberTrips;
            bikesh->rankingStations[k++].stationId = i+1;
            
        }
    }
    
    bikesh->rankingStations = realloc(bikesh->rankingStations, k * sizeof(stationData));
    
    qsort(bikesh->rankingStations, bikesh->realDim, sizeof(stationData), compare);

}

size_t getRealDim(bikeSharingADT bikesh){
    return bikesh->realDim;
}

size_t getMemberTrips(bikeSharingADT bikesh, int pos){
    return bikesh->rankingStations[pos-1].memberTrips;
}

char * getStationName(bikeSharingADT bikesh, int pos){
    return copyStr(bikesh->rankingStations[pos-1].stationName);
}

void stringcpy(bikeSharingADT bikesh, char * from, size_t stationId, int * flag){
    
    bikesh->rankingStations[stationId-1].stationName = malloc(strlen(from)+1);
    if (bikesh->rankingStations[stationId-1].stationName == NULL){
        (*flag) = MEMERR;
        return;
    }
    
   strcpy(bikesh->rankingStations[stationId-1].stationName, from); 

}

void freeADT(bikeSharingADT bikesh){

    if (bikesh != NULL) {
        if (bikesh->rankingStations != NULL) {
            for (int i = 0; i < bikesh->dim; i++) {
                if (bikesh->rankingStations[i].stationName != NULL) {
                    free(bikesh->rankingStations[i].stationName);
                }
            }
            free(bikesh->rankingStations);
        }
        free(bikesh);
    }

}

